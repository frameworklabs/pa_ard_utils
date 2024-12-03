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
    pa_emit_val (edge, Edge::RAISING);
    pa_pause;

    level = true;
    pa_pause;

    // same if level did not change
    pa_pause;

    level = false;
    pa_emit_val (edge, Edge::FALLING);
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
    pa_emit_val (edge, Edge::FALLING);
    pa_pause;

    level = false;
    pa_pause;

    // same if level did not change
    pa_pause;

    level = true;
    pa_emit_val (edge, Edge::RAISING);
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
    pa_emit_val (expected_edge, Edge::FALLING);
    expected_level = false;
    pa_pause;

    pa_emit_val (expected_edge, Edge::RAISING);
    expected_level = true;
    pa_pause;

    // raising should have no effect
    pa_pause;

    pa_emit_val (expected_edge, Edge::FALLING);
    expected_level = false;
    pa_pause;

    // Test 2 - level primed with false - immediate raise

    pa_emit (proceed);

    initial_level = false;
    pa_emit_val (expected_edge, Edge::RAISING);
    expected_level = true;
    pa_pause;

    pa_emit_val (expected_edge, Edge::FALLING);
    expected_level = false;
    pa_pause;

    // Test 3 - level primed with false - immediate fall

    pa_emit (proceed);

    initial_level = false;
    pa_emit_val (expected_edge, Edge::FALLING);
    expected_level = false;
    pa_pause;

    pa_emit_val (expected_edge, Edge::RAISING);
    expected_level = true;
    pa_pause;

    // Test 4 - level primed with true

    pa_emit (proceed);

    initial_level = true;
    expected_level = true;
    pa_pause;

    pa_emit_val (expected_edge, Edge::FALLING);
    expected_level = false;
    pa_pause;

    // Test 5 - level primed with true - immediate fall

    pa_emit (proceed);

    initial_level = true;
    pa_emit_val (expected_edge, Edge::FALLING);
    expected_level = false;
    pa_pause;

    pa_emit_val (expected_edge, Edge::RAISING);
    expected_level = true;
    pa_pause;

    // Test 6 - level primed with true - immediate raise

    pa_emit (proceed);

    initial_level = true;
    pa_emit_val (expected_edge, Edge::RAISING);
    expected_level = true;
    pa_pause;

    pa_emit_val (expected_edge, Edge::FALLING);
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

// Test Driver

#define run_test(nm) \
    pa_use(nm); \
    while (pa_tick(nm) == PA_RC_WAIT) {}

int main(int argc, char* argv[]) {
    std::cout << "Start" << std::endl;

    run_test(TestLevelToEdgeConverter);
    run_test(TestEdgeToLevelConverter);

    std::cout << "Done" << std::endl;

    return 0;
}
