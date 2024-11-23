// pa_ard_utils
//
// Copyright (c) 2024, Framework Labs.

#pragma once

#include "priv/pa_ard_utils_priv.h"

namespace proto_activities::ard_utils {

// Timing

#define pa_delay_m(time_m) pa_delay_s (time_m * 60);
#define pa_every_m(time_m) pa_every_s (time_m * 60)

/// An activity which delays for the given milliseconds.
pa_activity_sig (Delay_ms, pa_time_t ms);

// Button

/// An activity to debounce a physical button.
pa_activity_sig (ButtonRecognizer, uint8_t pin, pa_sig& was_pressed, pa_sig& was_released, const ButtonRecognizerConfig& config = {});

// An activity which prints to serial when the pressed or released status change.
pa_activity_sig (ButtonInspector, const char* msg, bool was_pressed, bool was_released);

// Press

/// An activity to recognize presses, double presses and long presses of a physical button.
pa_activity_sig (PressRecognizer, uint8_t pin, Press& press, const PressRecognizerConfig& config = {});

// An activity which prints to serial when the press status changes.
pa_activity_sig (PressInspector, const char* msg, Press press);

} // namespace proto_activities::ard_utils
