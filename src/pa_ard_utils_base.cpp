// pa_utils_cpp
//
// Copyright (c) 2024, Framework Labs.

#include "pa_ard_utils_base.h"

namespace proto_activities { namespace ard_utils {

// Timing

pa_activity_def (Delay_ms, pa_time_t delay) {
    pa_delay_ms (delay);
} pa_end;

// Logical

pa_activity_def (LevelToEdgeConverter, bool level, pa_signal& raising, pa_signal& falling) {
    pa_self.prev_level = level;
    pa_every (level != pa_self.prev_level) {
        if (level) {
            pa_emit (raising);
        } else {
            pa_emit (falling);
        }
        pa_self.prev_level = level;
    } pa_every_end
} pa_end

pa_activity_def (EdgeToLevelConverter, bool raising, bool falling, bool& level) {
    pa_await_immediate ((raising && !falling) || (falling && !raising));
    if (falling) {
        level = false;
    }
    pa_repeat {
        pa_await_immediate (raising && !falling);
        level = true;
        pa_await (falling && !raising);
        level = false;
    }
} pa_end

// Button

pa_activity_def (LogicalButtonRecognizer, bool button, ButtonSignal& action, const ButtonRecognizerConfig& config) {
    pa_repeat {
        pa_await_immediate (button);
        pa_emit_val (action, ButtonAction::PRESS);
        pa_delay_ms (config.debounce_ms);

        pa_await_immediate (!button);
        pa_emit_val (action, ButtonAction::RELEASE);
        pa_delay_ms (config.debounce_ms);
    }
} pa_end

// Press

namespace internal {

pa_activity_def (ReleasePressDetector, const ButtonSignal& action, bool& was_pressed, bool& was_released) {
    was_pressed = false;
    was_released = false;
    pa_await (action && action.val() == ButtonAction::RELEASE);
    was_released = true;
    pa_await (action && action.val() == ButtonAction::PRESS);
    was_pressed = true;
} pa_end

pa_activity_def (PressSustainer, Press press, PressSignal& sig) {
    pa_sustain_val (sig, std::move(press));
} pa_end

pa_activity_def (LogicalPressRecognizerImpl, const PressRecognizerConfig& config, const ButtonSignal& action, PressSignal& press) {
    pa_repeat {
        pa_await_immediate (action && action.val() == ButtonAction::PRESS);

        pa_after_ms_abort (config.double_tap_time_ms, ReleasePressDetector, action, pa_self.was_pressed, pa_self.was_released);

        if (pa_self.was_pressed) {
            pa_emit_val (press, Press::DOUBLE);
            pa_pause;
        } else if (pa_self.was_released) {
            pa_emit_val (press, Press::SHORT);
            pa_pause;
        } else {
            pa_when_abort (action && action.val() == ButtonAction::RELEASE, PressSustainer, Press::LONG, press);
        }
    }
} pa_end

} // namespace internal

pa_activity_def (LogicalPressRecognizer, bool button, PressSignal& press, const PressRecognizerConfig& config) {
    using namespace internal;
    pa_co(2) {
        pa_with (LogicalButtonRecognizer, button, pa_self.action, config.button_config);
        pa_with (LogicalPressRecognizerImpl, config, pa_self.action, press);
    } pa_co_end
} pa_end

} } // namespace proto_activities::ard_utils
