// pa_ard_utils
//
// Copyright (c) 2024, Framework Labs.

#pragma once

#include "priv/pa_ard_utils_base_priv.h"

#include "pa_ard_utils_types.h"

#include <proto_activities.h>

namespace proto_activities { namespace ard_utils {

// Timing

/// A statement to dalay for the given multiple of minutes.
#define pa_delay_m(time_m) pa_delay_s (time_m * 60);

/// A statement to run the body once every mulitple of minuts given.
#define pa_every_m(time_m) pa_every_s (time_m * 60)

/// An activity which delays for the given milliseconds.
pa_activity_sig (Delay_ms, pa_time_t ms);

// Logical

/// An activity which converts a binary level into signals for its rising and falling edges.
pa_activity_sig (LevelToEdgeConverter, bool level, EdgeSignal& edge);

/// An activity which converts rising and falling edges into a binary  level.
pa_activity_sig (EdgeToLevelConverter, const EdgeSignal& edge, bool& level);

// Button

/// An activity to debounce a logical button.
pa_activity_sig (LogicalButtonRecognizer, bool button, ButtonSignal& action, const ButtonRecognizerConfig& config = {});

// Press

/// An activity to recognize presses, double presses and long presses of a logical button.
pa_activity_sig (LogicalPressRecognizer, bool button, PressSignal& press, const PressRecognizerConfig& config = {});

} } // namespace proto_activities::ard_utils
