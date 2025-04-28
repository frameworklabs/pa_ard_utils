// tests.cpp

// Includes

#include "pa_ard_utils_base.h"

#include <iostream>
#include <assert.h>

// Tests

using namespace proto_activities::ard_utils;

// LevelToEdgeConverter Tests

pa_activity (TestLevelToEdgeConverterSpec, pa_ctx(), pa_signal& proceed, bool& level, EdgeSignal& edge) {
    
    // Test 1 - start with low level

    level = false;
    pa_pause;
    
    // same if level did not change
    pa_pause;
    
    level = true;
    pa_emit_val (edge, Edge::rising);
    pa_pause;

    level = true;
    pa_pause;

    // same if level did not change
    pa_pause;

    level = false;
    pa_emit_val (edge, Edge::falling);
    pa_pause;

    level = false;
    pa_pause;

    // same if level did not change
    pa_pause;
    
    // Test 2 - start with high level

    pa_emit (proceed);

    level = true;
    pa_pause;

    // same if level did not change
    pa_pause;

    level = false;
    pa_emit_val (edge, Edge::falling);
    pa_pause;

    level = false;
    pa_pause;

    // same if level did not change
    pa_pause;

    level = true;
    pa_emit_val (edge, Edge::rising);
    pa_pause;

    level = true;
    pa_pause;

    // same if level did not change
    pa_pause;

    // Finish tests

    pa_emit (proceed);
    
} pa_end

pa_activity (TestLevelToEdgeConverterTest, pa_ctx(pa_use(LevelToEdgeConverter)), bool proceed, bool level, EdgeSignal& edge) {
    pa_when_abort (proceed, LevelToEdgeConverter, level, edge);
    pa_when_abort (proceed, LevelToEdgeConverter, level, edge);
} pa_end

pa_activity (TestLevelToEdgeConverterCheck, pa_ctx(), const EdgeSignal& actual, const EdgeSignal& expected) {
    pa_always {
        assert(actual == expected);
        if (actual) {
            assert(actual.val() == expected.val());
        }
    } pa_always_end
} pa_end

pa_activity (TestLevelToEdgeConverter, pa_ctx(pa_co_res(4); pa_signal_res;
                                              pa_use(TestLevelToEdgeConverterSpec); pa_use(TestLevelToEdgeConverterTest); pa_use(TestLevelToEdgeConverterCheck);
                                              pa_def_signal(proceed); bool level; pa_def_val_signal(Edge, expected_edge); pa_def_val_signal(Edge, actual_edge))) {
    pa_co(3) {
        pa_with (TestLevelToEdgeConverterSpec, pa_self.proceed, pa_self.level, pa_self.expected_edge);
        pa_with (TestLevelToEdgeConverterTest, pa_self.proceed, pa_self.level, pa_self.actual_edge);
        pa_with_weak (TestLevelToEdgeConverterCheck, pa_self.actual_edge, pa_self.expected_edge);
    } pa_co_end
} pa_end

// EdgeToLevelConverter Tests

pa_activity (TestEdgeToLevelConverterSpec, pa_ctx(), pa_signal& proceed, bool& initial_level, EdgeSignal& expected_edge, bool& expected_level) {

    // Test 1 - level primed with false

    initial_level = false;
    expected_level = false;
    pa_pause;

    // same if level did not change
    pa_pause;
    
    // falling should have no effect
    pa_emit_val (expected_edge, Edge::falling);
    expected_level = false;
    pa_pause;

    pa_emit_val (expected_edge, Edge::rising);
    expected_level = true;
    pa_pause;

    // rising should have no effect
    pa_pause;

    pa_emit_val (expected_edge, Edge::falling);
    expected_level = false;
    pa_pause;

    // Test 2 - level primed with false - immediate rise

    pa_emit (proceed);

    initial_level = false;
    pa_emit_val (expected_edge, Edge::rising);
    expected_level = true;
    pa_pause;

    pa_emit_val (expected_edge, Edge::falling);
    expected_level = false;
    pa_pause;

    // Test 3 - level primed with false - immediate fall

    pa_emit (proceed);

    initial_level = false;
    pa_emit_val (expected_edge, Edge::falling);
    expected_level = false;
    pa_pause;

    pa_emit_val (expected_edge, Edge::rising);
    expected_level = true;
    pa_pause;

    // Test 4 - level primed with true

    pa_emit (proceed);

    initial_level = true;
    expected_level = true;
    pa_pause;

    pa_emit_val (expected_edge, Edge::falling);
    expected_level = false;
    pa_pause;

    // Test 5 - level primed with true - immediate fall

    pa_emit (proceed);

    initial_level = true;
    pa_emit_val (expected_edge, Edge::falling);
    expected_level = false;
    pa_pause;

    pa_emit_val (expected_edge, Edge::rising);
    expected_level = true;
    pa_pause;

    // Test 6 - level primed with true - immediate rise

    pa_emit (proceed);

    initial_level = true;
    pa_emit_val (expected_edge, Edge::rising);
    expected_level = true;
    pa_pause;

    pa_emit_val (expected_edge, Edge::falling);
    expected_level = false;
    pa_pause;

    // Finish tests

    pa_emit (proceed);

} pa_end

pa_activity (TestEdgeToLevelConverterTest, pa_ctx(pa_use(EdgeToLevelConverter)), bool proceed, const EdgeSignal& edge, bool& level) {
    pa_when_abort (proceed, EdgeToLevelConverter, edge, level);
    pa_when_abort (proceed, EdgeToLevelConverter, edge, level);
    pa_when_abort (proceed, EdgeToLevelConverter, edge, level);
    pa_when_abort (proceed, EdgeToLevelConverter, edge, level);
    pa_when_abort (proceed, EdgeToLevelConverter, edge, level);
    pa_when_abort (proceed, EdgeToLevelConverter, edge, level);
} pa_end

pa_activity (TestEdgeToLevelConverterCheck, pa_ctx(), bool actual, bool expected) {
    pa_always {
        assert(actual == expected);
    } pa_always_end
} pa_end

pa_activity (TestEdgeToLevelConverter, pa_ctx(pa_co_res(4); pa_signal_res;
                                              pa_use(TestEdgeToLevelConverterSpec); pa_use(TestEdgeToLevelConverterTest); pa_use(TestEdgeToLevelConverterCheck);
                                              pa_def_signal(proceed); pa_def_val_signal(Edge, edge); bool expected_level; bool actual_level)) {
    pa_co(3) {
        pa_with (TestEdgeToLevelConverterSpec, pa_self.proceed, pa_self.actual_level, pa_self.edge, pa_self.expected_level);
        pa_with (TestEdgeToLevelConverterTest, pa_self.proceed, pa_self.edge, pa_self.actual_level);
        pa_with_weak (TestEdgeToLevelConverterCheck, pa_self.actual_level, pa_self.expected_level);
    } pa_co_end
} pa_end

// LogicalPressRecognizer Tests

pa_activity (TestLogicalPressRecognizerSpec, pa_ctx(), ButtonSignal& button, PressSignal& press, pa_time_t& time_ms) {
    
    // short press with release within timeout
    time_ms = 0;
    pa_emit_val (button, ButtonAction::press);
    pa_pause;
    
    time_ms = 100;
    pa_emit_val (button, ButtonAction::release);
    pa_pause;

    time_ms = 300;
    pa_emit_val (press, Press::short_press);
    pa_pause;
    
    time_ms = 1000;
    pa_pause;

    // short press with release on timeout
    time_ms = 0;
    pa_emit_val (button, ButtonAction::press);
    pa_pause;

    time_ms = 300;
    pa_emit_val (button, ButtonAction::release);
    pa_emit_val (press, Press::short_press);
    pa_pause;

    time_ms = 1000;
    pa_pause;
    
    // double press with second press within timeout
    time_ms = 0;
    pa_emit_val (button, ButtonAction::press);
    pa_pause;

    time_ms = 100;
    pa_emit_val (button, ButtonAction::release);
    pa_pause;

    time_ms = 200;
    pa_emit_val (button, ButtonAction::press);
    pa_emit_val (press, Press::double_press);
    pa_pause;
    
    time_ms = 1000;
    pa_pause;

    // double press with second press on timeout
    time_ms = 0;
    pa_emit_val (button, ButtonAction::press);
    pa_pause;

    time_ms = 100;
    pa_emit_val (button, ButtonAction::release);
    pa_pause;

    time_ms = 300;
    pa_emit_val (button, ButtonAction::press);
    pa_emit_val (press, Press::double_press);
    pa_pause;
    
    time_ms = 1000;
    pa_pause;

    // long press
    time_ms = 0;
    pa_emit_val (button, ButtonAction::press);
    pa_pause;

    time_ms = 300;
    pa_emit_val (press, Press::long_press);
    pa_pause;

    time_ms = 400;
    pa_emit_val (press, Press::long_press);
    pa_pause;

    time_ms = 500;
    pa_emit_val (button, ButtonAction::release);
    pa_pause;

    time_ms = 1000;
    pa_pause;

} pa_end

pa_activity (TestLogicalPressRecognizerTest, pa_ctx(pa_enter_res; pa_use_ns(internal, LogicalPressRecognizerImpl)), const ButtonSignal& button, pa_time_t time_ms, PressSignal& press) {
    pa_enter {
        pa_current_time_ms = time_ms;
    };
    pa_run (LogicalPressRecognizerImpl, {}, button, press);
} pa_end

pa_activity (TestLogicalPressRecognizerCheck, pa_ctx(), const PressSignal& actual, const PressSignal& expected) {
    pa_always {
        assert(bool(actual) == bool(expected));
        if (actual) {
            assert(actual.val() == expected.val());
        }
    } pa_always_end
} pa_end

pa_activity (TestLogicalPressRecognizer, pa_ctx(pa_co_res(4); pa_signal_res;
                                                pa_use(TestLogicalPressRecognizerSpec); pa_use(TestLogicalPressRecognizerTest); pa_use(TestLogicalPressRecognizerCheck);
                                                pa_time_t time_ms;
                                                pa_def_val_signal(ButtonAction, button); pa_def_val_signal(Press, expected_press); pa_def_val_signal(Press, actual_press))) {
    pa_co(3) {
        pa_with (TestLogicalPressRecognizerSpec, pa_self.button, pa_self.expected_press, pa_self.time_ms);
        pa_with_weak (TestLogicalPressRecognizerTest, pa_self.button, pa_self.time_ms, pa_self.actual_press);
        pa_with_weak (TestLogicalPressRecognizerCheck, pa_self.actual_press, pa_self.expected_press);
    } pa_co_end
} pa_end

// Test Driver

#define run_test(nm) \
    pa_use(nm); \
    while (pa_tick(nm) == PA_RC_WAIT) {}

int main(int argc, char* argv[]) {
    std::cout << "Start" << std::endl;

    run_test(TestLevelToEdgeConverter);
    run_test(TestEdgeToLevelConverter);
    run_test(TestLogicalPressRecognizer);

    std::cout << "Done" << std::endl;

    return 0;
}
