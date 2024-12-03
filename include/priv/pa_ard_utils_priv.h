// pa_utils_cpp
//
// Copyright (c) 2024, Framework Labs.

#pragma once

#include "pa_ard_utils_types.h"

#include "pa_ard_utils_base.h"

#include <proto_activities.h>

namespace proto_activities { namespace ard_utils {

// Logical

namespace internal {

pa_activity_decl (LevelInspectorImpl, pa_ctx(), bool level, bool rising, bool falling, const char* high_msg, const char* low_msg);

} // namespace

pa_activity_ctx (LevelInspector, pa_co_res(2); pa_signal_res; pa_use(LevelToEdgeConverter); pa_use_ns(internal, LevelInspectorImpl); pa_def_signal(raising); pa_def_signal(falling));

// Button

pa_activity_ctx (ButtonInspector);
pa_activity_ctx (ButtonRecognizer, pa_co_res(2); pa_use(LogicalButtonRecognizer); pa_use(ButtonInspector));

// Press

pa_activity_ctx (PressInspector);
pa_activity_ctx (PressRecognizer, pa_co_res(2); pa_use(LogicalPressRecognizer); pa_use(PressInspector));

} } // namespace proto_activities::ard_utils
