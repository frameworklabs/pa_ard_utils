// pa_ard_utils
//
// Copyright (c) 2024, Framework Labs.

#pragma once

#include "priv/pa_ard_utils_priv.h"

namespace proto_activities { namespace ard_utils {

// Timing

/// A statement to dalay for the given multiple of minutes.
#define pa_delay_m(time_m) pa_delay_s (time_m * 60);

/// A statement to run the body once every mulitple of minuts given.
#define pa_every_m(time_m) pa_every_s (time_m * 60)

/// An activity which delays for the given milliseconds.
pa_activity_sig (Delay_ms, pa_time_t ms);

// Logical

/// An activity which converts a binary level into signals for its raising and falling edges.
pa_activity_sig (LevelToEdgeConverter, bool level, pa_signal& raising, pa_signal& falling);

/// An activity which converts raising and falling edges into a binary  level.
pa_activity_sig (EdgeToLevelConverter, bool raising, bool falling, bool& level);

/// An activity which prints to serial when the level changes.
pa_activity_sig (LevelInspector, bool level, const char* high_msg, const char* low_msg);

// Button

/// An activity to debounce a physical button.
pa_activity_sig (ButtonRecognizer, uint8_t pin, pa_signal& was_pressed, pa_signal& was_released, const ButtonRecognizerConfig& config = {});

/// An activity which prints to serial when the pressed or released status change.
pa_activity_sig (ButtonInspector, const char* msg, bool was_pressed, bool was_released);

// Press

/// An activity to recognize presses, double presses and long presses of a physical button.
pa_activity_sig (PressRecognizer, uint8_t pin, PressSignal& press, const PressRecognizerConfig& config = {});

/// An activity which prints to serial when the press status changes.
pa_activity_sig (PressInspector, const char* msg, const PressSignal& press);

} } // namespace proto_activities::ard_utils
