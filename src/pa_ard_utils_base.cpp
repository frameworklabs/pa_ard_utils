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

pa_activity_def (LevelToEdgeConverter, bool level, EdgeSignal& edge) {
    pa_self.prev_level = level;
    pa_every (level != pa_self.prev_level) {
        if (level) {
            pa_emit_val (edge, Edge::rising);
        } else {
            pa_emit_val (edge, Edge::falling);
        }
        pa_self.prev_level = level;
    } pa_every_end
} pa_end

pa_activity_def (EdgeToLevelConverter, const EdgeSignal& edge, bool& level) {
    pa_await_immediate (edge);
    if (edge.val() == Edge::falling) {
        level = false;
    }
    pa_repeat {
        pa_await_immediate (edge && edge.val() == Edge::rising);
        level = true;
        pa_await (edge && edge.val() == Edge::falling);
        level = false;
    }
} pa_end

// Button

pa_activity_def (LogicalButtonRecognizer, bool button, ButtonSignal& action, const ButtonRecognizerConfig& config) {
    pa_repeat {
        pa_await_immediate (button);
        pa_emit_val (action, ButtonAction::press);
        pa_delay_ms (config.debounce_ms);

        pa_await_immediate (!button);
        pa_emit_val (action, ButtonAction::release);
        pa_delay_ms (config.debounce_ms);
    }
} pa_end

// Press

namespace internal {

pa_activity_def (ReleasePressDetector, const ButtonSignal& action, bool& was_pressed, bool& was_released) {
    was_pressed = false;
    was_released = false;
    pa_await (action && action.val() == ButtonAction::release);
    was_released = true;
    pa_await (action && action.val() == ButtonAction::press);
    was_pressed = true;
} pa_end

pa_activity_def (PressSustainer, Press press, PressSignal& sig) {
    pa_sustain_val (sig, std::move(press));
} pa_end

pa_activity_def (LogicalPressRecognizerImpl, const PressRecognizerConfig& config, const ButtonSignal& action, PressSignal& press) {
    pa_repeat {
        pa_await_immediate (action && action.val() == ButtonAction::press);

        pa_after_ms_abort (config.double_tap_time_ms, ReleasePressDetector, action, pa_self.was_pressed, pa_self.was_released);

        if (pa_self.was_pressed || (action && action.val() == ButtonAction::press)) {
            pa_emit_val (press, Press::double_press);
            pa_pause;
        } else if (pa_self.was_released || (action && action.val() == ButtonAction::release)) {
            pa_emit_val (press, Press::short_press);
            pa_pause;
        } else {
            pa_when_abort (action && action.val() == ButtonAction::release, PressSustainer, Press::long_press, press);
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
