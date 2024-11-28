// pa_utils_cpp
//
// Copyright (c) 2024, Framework Labs.

#include "pa_ard_utils.h"

#include <Arduino.h>

namespace proto_activities::ard_utils {

// Timing

pa_activity_def (Delay_ms, pa_time_t delay) {
    pa_delay_ms (delay);
} pa_end;

// Logical

pa_activity_def (LevelToEdgeConverter, bool level, pa_sig& raising, pa_sig& falling) {
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
    pa_await_immediate (raising || falling);
    if (falling) {
        level = false;
    }
    pa_repeat {
        pa_await_immediate (raising);
        level = true;
        pa_await (falling);
        level = false;
    }
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

pa_activity_def (ButtonRecognizerImpl, uint8_t pin, pa_sig& was_pressed, pa_sig& was_released, const ButtonRecognizerConfig& config) {
    if (config.low_is_pressed) {
        pinMode(pin, INPUT_PULLUP);
    } else {
        pinMode(pin, INPUT_PULLDOWN);
    }

    pa_repeat {
        if (is_pressed(pin, config)) {
            pa_emit (was_pressed);
        } else {
            pa_await (is_pressed(pin, config));
            pa_emit (was_pressed);
            pa_delay_ms (config.debounce_ms);
        }

        if (!is_pressed(pin, config)) {
            pa_emit (was_released);
        } else {
            pa_await (!is_pressed(pin, config));
            pa_emit (was_released);
            pa_delay_ms (config.debounce_ms);
        }
    }
} pa_end

} // namespace

pa_activity_def (ButtonRecognizer, uint8_t pin, pa_sig& was_pressed, pa_sig& was_released, const ButtonRecognizerConfig& config) {
    using namespace internal;
    if (config.inspect_msg == nullptr) {
        pa_run (ButtonRecognizerImpl, pin, was_pressed, was_released, config);
    } else {
        pa_co(2) {
            pa_with (ButtonRecognizerImpl, pin, was_pressed, was_released, config);
            pa_with (ButtonInspector, config.inspect_msg, was_pressed, was_released);
        } pa_co_end
    }
} pa_end

pa_activity_def (ButtonInspector, const char* msg, bool was_pressed, bool was_released) {
    pa_every (was_pressed || was_released) {
        Serial.print(msg);
        Serial.print(' ');
        if (was_pressed) {
            Serial.println("was pressed");
        }
        if (was_released) {
            Serial.println("was released");
        }
    } pa_every_end
} pa_end

// Press

namespace internal {

pa_activity_def (ReleasePressDetector, bool btn_was_pressed, bool btn_was_released, bool& was_pressed, bool& was_released) {
    was_pressed = false;
    was_released = false;
    pa_await (btn_was_released);
    was_released = true;
    pa_await (btn_was_pressed);
    was_pressed = true;
} pa_end

pa_activity_def (PressRecognizerImpl, const PressRecognizerConfig& config, bool btn_was_pressed, bool btn_was_released, Press& press) {
    pa_repeat {
        press = Press::NO;

        pa_await_immediate (btn_was_pressed);

        pa_after_ms_abort (config.double_tap_time_ms, ReleasePressDetector, btn_was_pressed, btn_was_released, pa_self.was_pressed, pa_self.was_released);

        if (pa_self.was_pressed) {
            press = Press::DOUBLE;
            pa_pause;
        }
        else if (pa_self.was_released) {
            press = Press::SHORT;
            pa_pause;
        }
        else {
            press = Press::LONG;
            pa_await (btn_was_released);
        }
    }
} pa_end

} // namespace

pa_activity_def (PressRecognizer, uint8_t pin, Press& press, const PressRecognizerConfig& config) {
    using namespace internal;
    if (config.button_config.inspect_msg == nullptr) {
        pa_co(2) {
            pa_with (ButtonRecognizer, pin, pa_self.btn_was_pressed, pa_self.btn_was_released, config.button_config);
            pa_with (PressRecognizerImpl, config, pa_self.btn_was_pressed, pa_self.btn_was_released, press);
        } pa_co_end
    } else {
        pa_co(3) {
            pa_with (ButtonRecognizer, pin, pa_self.btn_was_pressed, pa_self.btn_was_released, config.button_config);
            pa_with (PressRecognizerImpl, config, pa_self.btn_was_pressed, pa_self.btn_was_released, press);
            pa_with (PressInspector, config.button_config.inspect_msg, press);
        } pa_co_end
    }
} pa_end

pa_activity_def (PressInspector, const char* msg, Press press) {
    pa_every (press != Press::NO) {
        Serial.print(msg);
        Serial.print(" recognized ");
        switch (press) {
            case Press::SHORT: Serial.println("SHORT press"); break;
            case Press::LONG: Serial.println("LONG press begin"); break;
            case Press::DOUBLE: Serial.println("DOUBLE press"); break;
            default: break;
        }
        if (press == Press::LONG) {
            pa_await (press == Press::NO);
            Serial.print(msg);
            Serial.println(" recognized LONG press end");
        }
    } pa_every_end
} pa_end

} // namespace proto_activities::ard_utils
