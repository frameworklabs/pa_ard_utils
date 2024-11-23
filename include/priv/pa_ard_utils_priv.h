// pa_utils_cpp
//
// Copyright (c) 2024, Framework Labs.

#pragma once

#include "pa_ard_utils_types.h"

#include <proto_activities.h>

namespace proto_activities::ard_utils {

// Timing

pa_activity_ctx_tm (Delay_ms);

// Button

namespace internal {

pa_activity_decl (ButtonRecognizerImpl, pa_ctx_tm(), uint8_t pin, pa_sig& was_pressed, pa_sig& was_released, const ButtonRecognizerConfig& config);

} // namespace

pa_activity_ctx (ButtonInspector);
pa_activity_ctx (ButtonRecognizer, pa_co_res(2); pa_use_ns(internal, ButtonRecognizerImpl); pa_use(ButtonInspector));

// Press

namespace internal {

pa_activity_decl (ReleasePressDetector, pa_ctx(), bool btn_was_pressed, bool btn_was_released, bool& was_pressed, bool& was_released);
pa_activity_decl (PressRecognizerImpl, pa_ctx_tm(pa_use_ns(internal, ReleasePressDetector); bool was_pressed; bool was_released), const PressRecognizerConfig& config, bool btn_was_pressed, bool btn_was_released, Press& press);

} // namespace

pa_activity_ctx (PressInspector);
pa_activity_ctx (PressRecognizer, pa_co_res(3); pa_sig_res; pa_use(ButtonRecognizer); pa_use_ns(internal, PressRecognizerImpl); pa_use(PressInspector); pa_def_sig(btn_was_pressed); pa_def_sig(btn_was_released));

} // namespace proto_activities::ard_utils
