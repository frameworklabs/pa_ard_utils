// pa_utils_cpp
//
// Copyright (c) 2024, Framework Labs.

#include "pa_ard_utils.h"

#include <Arduino.h>

namespace proto_activities { namespace ard_utils {

// Logical

namespace internal {

pa_activity_def (LevelInspectorImpl, bool level, const EdgeSignal& edge, const char* high_msg, const char* low_msg) {
    if (level) {
        Serial.println(high_msg);
    } else {
        Serial.println(low_msg);
    }
    pa_every (edge) {
        if (edge.val() == Edge::RAISING) {
            Serial.println(high_msg);
        } else /* falling */ {
            Serial.println(low_msg);
        }
    } pa_every_end
} pa_end

} // namespace internal

pa_activity_def (LevelInspector, bool level, const char* high_msg, const char* low_msg) {
    using namespace internal;
    pa_co(2) {
        pa_with (LevelToEdgeConverter, level, pa_self.edge);
        pa_with (LevelInspectorImpl, level, pa_self.edge, high_msg, low_msg);
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

pa_activity_def (PressRecognizer, uint8_t pin, PressSignal& press, const PressRecognizerConfig& config) {
    using namespace internal;

    setup_pin_mode(pin, config.button_config);

    if (config.button_config.inspect_msg == nullptr) {
        pa_run (LogicalPressRecognizer, is_pressed(pin, config.button_config), press, config);
    } else {
        pa_co(2) {
            pa_with (LogicalPressRecognizer, is_pressed(pin, config.button_config), press);
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
