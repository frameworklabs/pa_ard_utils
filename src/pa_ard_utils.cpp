// pa_utils_cpp
//
// Copyright (c) 2024, Framework Labs.

#include "pa_ard_utils.h"

#include <Arduino.h>

namespace proto_activities { namespace ard_utils {

// Timing

pa_activity_def (Delay_ms, pa_time_t delay) {
    pa_delay_ms (delay);
} pa_end;

// Logical

namespace internal {

pa_activity_def (LevelInspectorImpl, bool level, bool rising, bool falling, const char* high_msg, const char* low_msg) {
    if (level) {
        Serial.println(high_msg);
    } else {
        Serial.println(low_msg);
    }
    pa_every (rising || falling) {
        if (rising) {
            Serial.println(high_msg);
        } else /* falling */ {
            Serial.println(low_msg);
        }
    } pa_every_end
} pa_end

} // namespace internal

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

pa_activity_def (LevelInspector, bool level, const char* high_msg, const char* low_msg) {
    using namespace internal;
    pa_co(2) {
        pa_with (LevelToEdgeConverter, level, pa_self.raising, pa_self.falling);
        pa_with (LevelInspectorImpl, level, pa_self.raising, pa_self.falling, high_msg, low_msg);
    } pa_co_end
} pa_end

// Button

namespace internal {

bool is_pressed(uint8_t pin, const ButtonRecognizerConfig& config) {
    if (config.low_is_pressed) {
        return digitalRead(pin) == LOW;
    } else {
        return digitalRead(pin) == HIGH;
    }
}

pa_activity_def (ButtonRecognizerImpl, const ButtonRecognizerConfig& config, uint8_t pin, ButtonSignal& action) {
    if (config.low_is_pressed) {
        pinMode(pin, INPUT_PULLUP);
    } else {
        pinMode(pin, INPUT_PULLDOWN);
    }

    pa_repeat {
        pa_await_immediate (is_pressed(pin, config));
        pa_emit_val (action, ButtonAction::PRESS);
        pa_delay_ms (config.debounce_ms);

        pa_await_immediate (!is_pressed(pin, config));
        pa_emit_val (action, ButtonAction::RELEASE);
        pa_delay_ms (config.debounce_ms);
    }
} pa_end

} // namespace internal

pa_activity_def (ButtonRecognizer, uint8_t pin, ButtonSignal& action, const ButtonRecognizerConfig& config) {
    using namespace internal;
    if (config.inspect_msg == nullptr) {
        pa_run (ButtonRecognizerImpl, config, pin, action);
    } else {
        pa_co(2) {
            pa_with (ButtonRecognizerImpl, config, pin, action);
            pa_with (ButtonInspector, config.inspect_msg, action);
        } pa_co_end
    }
} pa_end

pa_activity_def (ButtonInspector, const char* msg, const ButtonSignal& action) {
    pa_every (action) {
        Serial.print(msg);
        Serial.print(' ');
        if (action.val() == ButtonAction::PRESS) {
            Serial.println("was pressed");
        } else {
            Serial.println("was released");
        }
    } pa_every_end
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

pa_activity_def (PressRecognizerImpl, const PressRecognizerConfig& config, const ButtonSignal& action, PressSignal& press) {
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

pa_activity_def (PressRecognizer, uint8_t pin, PressSignal& press, const PressRecognizerConfig& config) {
    using namespace internal;
    if (config.button_config.inspect_msg == nullptr) {
        pa_co(2) {
            pa_with (ButtonRecognizer, pin, pa_self.action, config.button_config);
            pa_with (PressRecognizerImpl, config, pa_self.action, press);
        } pa_co_end
    } else {
        pa_co(3) {
            pa_with (ButtonRecognizer, pin, pa_self.action, config.button_config);
            pa_with (PressRecognizerImpl, config, pa_self.action, press);
            pa_with (PressInspector, config.button_config.inspect_msg, press);
        } pa_co_end
    }
} pa_end

pa_activity_def (PressInspector, const char* msg, const PressSignal& press) {
    pa_every (press) {
        Serial.print(msg);
        Serial.print(" recognized ");
        switch (press.val()) {
            case Press::SHORT: Serial.println("SHORT press"); break;
            case Press::LONG: Serial.println("LONG press begin"); break;
            case Press::DOUBLE: Serial.println("DOUBLE press"); break;
        }
        if (press.val() == Press::LONG) {
            pa_await (!press);
            Serial.print(msg);
            Serial.println(" recognized LONG press end");
        }
    } pa_every_end
} pa_end

} } // namespace proto_activities::ard_utils
