// pa_ard_utils
//
// Copyright (c) 2024, Framework Labs.

#pragma once

#include <proto_activities.h> // for pa_time_t

namespace proto_activities { namespace ard_utils {

// Button

/// The detected type of button action.
enum class ButtonAction : uint8_t {
    PRESS,  ///< The button got pressed
    RELEASE ///< The button got released
};

/// The button action type as a signal.
using ButtonSignal = pa_val_signal<ButtonAction>;

/// A Configuration container for the ButtonRecognizer.
struct ButtonRecognizerConfig {
    ButtonRecognizerConfig(pa_time_t debounce_ms_ = 50, bool low_is_pressed_ = true, const char* inspect_msg_ = {}) : debounce_ms(debounce_ms_), low_is_pressed(low_is_pressed_), inspect_msg(inspect_msg_) {}

    /// How many milliseconds to debounce the button both on press and release.
    pa_time_t debounce_ms;

    /// A LOW on the pin indicates a pressed button - this is typical for pulled-up buttons.
    bool low_is_pressed;

    /// If this is a non null string, a button (plus press) inspector will be active.
    const char* inspect_msg;
};

// Press

/// The detected type of press.
enum class Press : uint8_t {
    SHORT,  ///< A short single press detected
    DOUBLE, ///< A double press detected
    LONG    ///< A long single press detected
};

/// The press type as a signal.
using PressSignal = pa_val_signal<Press>;

/// A configuration container for the PressRecognizer.
struct PressRecognizerConfig {
    PressRecognizerConfig(const ButtonRecognizerConfig& button_config_ = {}, pa_time_t double_tap_time_ms_ = 300) : button_config(button_config_), double_tap_time_ms(double_tap_time_ms_) {}

    /// The configuration for the button debouncing.
    ButtonRecognizerConfig button_config;

    /// The duration within which two presses on the button are interpreted as double taps.
    pa_time_t double_tap_time_ms;
};

} } // namespace proto_activities::ard_utils
