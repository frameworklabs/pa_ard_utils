// pa_utils_cpp
//
// Copyright (c) 2024, Framework Labs.

#pragma once

#include "pa_ard_utils_types.h"

#include <proto_activities.h>

// Extensions

#ifndef pa_sustain_val
#define pa_sustain_val(sig, val) \
    pa_always { \
        pa_emit_val(sig, val); \
    } pa_always_end
#endif

namespace proto_activities { namespace ard_utils {

// Timing

pa_activity_ctx_tm (Delay_ms);

// Logical

pa_activity_ctx (LevelToEdgeConverter, bool prev_level);
pa_activity_ctx (EdgeToLevelConverter);

// Button

pa_activity_ctx_tm (LogicalButtonRecognizer);

// Press

namespace internal {

pa_activity_decl (ReleasePressDetector, pa_ctx(), const ButtonSignal& action, bool& was_pressed, bool& was_released);
pa_activity_decl (PressSustainer, pa_ctx(), Press press, PressSignal& sig);
pa_activity_decl (LogicalPressRecognizerImpl, pa_ctx_tm(pa_use(ReleasePressDetector); pa_use(PressSustainer); bool was_pressed; bool was_released),
                                              const PressRecognizerConfig& config, const ButtonSignal& action, PressSignal& press);

} // namespace

pa_activity_ctx (LogicalPressRecognizer, pa_co_res(2); pa_signal_res; pa_use(LogicalButtonRecognizer); pa_use_ns(internal, LogicalPressRecognizerImpl);
                                         pa_def_val_signal(ButtonAction, action));

} } // namespace proto_activities::ard_utils
