// pa_ard_utils
//
// Copyright (c) 2024, Framework Labs.

#pragma once

#include "priv/pa_ard_utils_priv.h"

#include "pa_ard_utils_base.h"

namespace proto_activities { namespace ard_utils {

// Logical

/// An activity which prints to serial when the level changes.
pa_activity_sig (LevelInspector, bool level, const char* high_msg, const char* low_msg);

// Button

/// An activity to debounce a physical button.
pa_activity_sig (ButtonRecognizer, uint8_t pin, ButtonSignal& action, const ButtonRecognizerConfig& config = {});

/// An activity which prints to serial when the pressed or released status change.
pa_activity_sig (ButtonInspector, const char* msg, const ButtonSignal& action);

// Press

/// An activity to recognize presses, double presses and long presses of a physical button.
pa_activity_sig (PressRecognizer, uint8_t pin, PressSignal& press, const PressRecognizerConfig& config = {});

/// An activity which prints to serial when the press status changes.
pa_activity_sig (PressInspector, const char* msg, const PressSignal& press);

} } // namespace proto_activities::ard_utils
