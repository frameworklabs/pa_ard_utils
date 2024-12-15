// pa_utils_cpp
//
// Copyright (c) 2024, Framework Labs.

#include "pa_ard_utils.h"

#include <Arduino.h>

namespace proto_activities { namespace ard_utils {

// Logical

pa_activity_def (LevelInspector, bool level, const char* high_msg, const char* low_msg) {
    if (level) {
        Serial.println(high_msg);
    } else {
        Serial.println(low_msg);
    }
    pa_co(2) {
        pa_with (LevelToEdgeConverter, level, pa_self.edge);
        pa_with (EdgeInspector, pa_self.edge, high_msg, low_msg);
    } pa_co_end
} pa_end

pa_activity_def (EdgeInspector, const EdgeSignal& edge, const char* rising_msg, const char* falling_msg) {
    pa_every (edge) {
        if (edge.val() == Edge::rising) {
            Serial.println(rising_msg);
        } else /* falling */ {
            Serial.println(falling_msg);
        }
    } pa_every_end
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

void setup_pin_mode(uint8_t pin, const ButtonRecognizerConfig& config) {
    if (config.low_is_pressed) {
        pinMode(pin, INPUT_PULLUP);
    } else {
        pinMode(pin, INPUT_PULLDOWN);
    }
}

} // namespace internal

pa_activity_def (ButtonRecognizer, uint8_t pin, ButtonSignal& action, const ButtonRecognizerConfig& config) {
    using namespace internal;

    setup_pin_mode(pin, config);

    if (config.inspect_msg == nullptr) {
        pa_run (LogicalButtonRecognizer, is_pressed(pin, config), action, config);
    } else {
        pa_co(2) {
            pa_with (LogicalButtonRecognizer, is_pressed(pin, config), action, config);
            pa_with (ButtonInspector, action, config.inspect_msg);
        } pa_co_end
    }
} pa_end

pa_activity_def (ButtonInspector, const ButtonSignal& action, const char* msg) {
    pa_every (action) {
        Serial.print(msg);
        Serial.print(' ');
        if (action.val() == ButtonAction::press) {
            Serial.println("was pressed");
        } else {
            Serial.println("was released");
        }
    } pa_every_end
} pa_end

// Press

pa_activity_def (PressRecognizer, uint8_t pin, PressSignal& press, const PressRecognizerConfig& config) {
    using namespace internal;

    setup_pin_mode(pin, config.button_config);

    if (config.button_config.inspect_msg == nullptr) {
        pa_run (LogicalPressRecognizer, is_pressed(pin, config.button_config), press, config);
    } else {
        pa_co(2) {
            pa_with (LogicalPressRecognizer, is_pressed(pin, config.button_config), press);
            pa_with (PressInspector, press, config.button_config.inspect_msg);
        } pa_co_end
    }
} pa_end

pa_activity_def (PressInspector, const PressSignal& press, const char* msg) {
    pa_every (press) {
        Serial.print(msg);
        Serial.print(" recognized ");
        switch (press.val()) {
            case Press::short_press: Serial.println("short press"); break;
            case Press::long_press: Serial.println("long press begin"); break;
            case Press::double_press: Serial.println("double press"); break;
        }
        if (press.val() == Press::long_press) {
            pa_await (!press);
            Serial.print(msg);
            Serial.println(" recognized long press end");
        }
    } pa_every_end
} pa_end

} } // namespace proto_activities::ard_utils
