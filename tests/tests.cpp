// tests.cpp

// Includes

#include "pa_ard_utils.h"

#include "Arduino.h"

#include <iostream>
#include <assert.h>

// Arduino Mocks

namespace {

uint8_t theDigitalReadValue = LOW;

}

void pinMode(uint8_t, uint8_t) {
}

uint8_t digitalRead(uint8_t) {
    return theDigitalReadValue;
}

void SerialMock::print(char) {}
void SerialMock::print(const char*) {}
void SerialMock::println(const char*) {}

SerialMock Serial;

// Tests

using namespace proto_activities::ard_utils;

// LevelToEdgeConverter Tests

pa_activity (TestLevelToEdgeConverterSpec, pa_ctx(), pa_sig& proceed, bool& level, bool& expected_raising, bool& expected_falling) {
    
    // Test 1 - start with low level

    level = LOW;
    expected_raising = false;
    expected_falling = false;
    pa_pause;
    
    // same if level did not change
    pa_pause;
    
    level = HIGH;
    expected_raising = true;
    expected_falling = false;
    pa_pause;

    level = HIGH;
    expected_raising = false;
    expected_falling = false;
    pa_pause;

    // same if level did not change
    pa_pause;

    level = LOW;
    expected_raising = false;
    expected_falling = true;
    pa_pause;

    level = LOW;
    expected_raising = false;
    expected_falling = false;
    pa_pause;

    // same if level did not change
    pa_pause;
    
    // Test 2 - start with high level

    pa_emit (proceed);

    level = HIGH;
    expected_raising = false;
    expected_falling = false;
    pa_pause;

    // same if level did not change
    pa_pause;

    level = LOW;
    expected_raising = false;
    expected_falling = true;
    pa_pause;

    level = LOW;
    expected_raising = false;
    expected_falling = false;
    pa_pause;

    // same if level did not change
    pa_pause;

    level = HIGH;
    expected_raising = true;
    expected_falling = false;
    pa_pause;

    level = HIGH;
    expected_raising = false;
    expected_falling = false;
    pa_pause;

    // same if level did not change
    pa_pause;

    // Finish tests

    pa_emit (proceed);
    
} pa_end

pa_activity (TestLevelToEdgeConverterTest, pa_ctx(pa_use(LevelToEdgeConverter)), bool proceed, bool level, pa_sig& raising, pa_sig& falling) {
    pa_when_abort (proceed, LevelToEdgeConverter, level, raising, falling);
    pa_when_abort (proceed, LevelToEdgeConverter, level, raising, falling);
} pa_end

pa_activity (TestLevelToEdgeConverterCheck, pa_ctx(), bool actual_raising, bool actual_falling, bool expected_raising, bool expected_falling) {
    pa_always {
        assert(actual_raising == expected_raising);
        assert(actual_falling == expected_falling);
    } pa_always_end
} pa_end

pa_activity (TestLevelToEdgeConverter, pa_ctx(pa_co_res(4); pa_sig_res;
                                              pa_use(TestLevelToEdgeConverterSpec); pa_use(TestLevelToEdgeConverterTest); pa_use(TestLevelToEdgeConverterCheck);
                                              pa_def_sig(proceed); bool level; bool expected_raising; bool expected_falling; pa_def_sig(actual_raising); pa_def_sig(actual_falling))) {
    pa_co(3) {
        pa_with (TestLevelToEdgeConverterSpec, pa_self.proceed, pa_self.level, pa_self.expected_raising, pa_self.expected_falling);
        pa_with (TestLevelToEdgeConverterTest, pa_self.proceed, pa_self.level, pa_self.actual_raising, pa_self.actual_falling);
        pa_with_weak (TestLevelToEdgeConverterCheck, pa_self.actual_raising, pa_self.actual_falling, pa_self.expected_raising, pa_self.expected_falling);
    } pa_co_end
} pa_end

// EdgeToLevelConverter Tests

pa_activity (TestEdgeToLevelConverterSpec, pa_ctx(), pa_sig& proceed, bool& initial_level, bool& raising, bool& falling, bool& expected_level) {

    // Test 1 - level primed with false

    initial_level = false;
    raising = false;
    falling = false;
    expected_level = false;
    pa_pause;

    // same if level did not change
    pa_pause;
    
    // falling should have no effect
    raising = false;
    falling = true;
    expected_level = false;
    pa_pause;

    raising = true;
    falling = false;
    expected_level = true;
    pa_pause;

    // raising should have no effect
    pa_pause;

    // both raising and falling should have no effect on high level
    raising = true;
    falling = true;
    expected_level = true;
    pa_pause;

    raising = false;
    falling = true;
    expected_level = false;
    pa_pause;

    // both raising and falling should have no effect on low level
    raising = true;
    falling = true;
    expected_level = false;
    pa_pause;

    // Test 2 - level primed with false - immediate raise

    pa_emit (proceed);

    initial_level = false;
    raising = true;
    falling = false;
    expected_level = true;
    pa_pause;

    raising = false;
    falling = true;
    expected_level = false;
    pa_pause;

    // Test 3 - level primed with false - immediate fall

    pa_emit (proceed);

    initial_level = false;
    raising = false;
    falling = true;
    expected_level = false;
    pa_pause;

    raising = true;
    falling = false;
    expected_level = true;
    pa_pause;

    // Test 4 - level primed with false - immediate raise and fall

    pa_emit (proceed);

    initial_level = false;
    raising = true;
    falling = true;
    expected_level = false;
    pa_pause;

    raising = true;
    falling = false;
    expected_level = true;
    pa_pause;

    // Test 5 - level primed with true

    pa_emit (proceed);

    initial_level = true;
    raising = false;
    falling = false;
    expected_level = true;
    pa_pause;

    raising = false;
    falling = true;
    expected_level = false;
    pa_pause;

    // Test 6 - level primed with true - immediate fall

    pa_emit (proceed);

    initial_level = true;
    raising = false;
    falling = true;
    expected_level = false;
    pa_pause;

    raising = true;
    falling = false;
    expected_level = true;
    pa_pause;

    // Test 7 - level primed with true - immediate raise

    pa_emit (proceed);

    initial_level = true;
    raising = true;
    falling = false;
    expected_level = true;
    pa_pause;

    raising = false;
    falling = true;
    expected_level = false;
    pa_pause;

    // Test 8 - level primed with true - immediate raise and fall

    pa_emit (proceed);

    initial_level = true;
    raising = true;
    falling = true;
    expected_level = true;
    pa_pause;

    raising = false;
    falling = true;
    expected_level = false;
    pa_pause;

    // Finish tests

    pa_emit (proceed);

} pa_end

pa_activity (TestEdgeToLevelConverterTest, pa_ctx(pa_use(EdgeToLevelConverter)), bool proceed, bool raising, bool falling, bool& level) {
    pa_when_abort (proceed, EdgeToLevelConverter, raising, falling, level);
    pa_when_abort (proceed, EdgeToLevelConverter, raising, falling, level);
    pa_when_abort (proceed, EdgeToLevelConverter, raising, falling, level);
    pa_when_abort (proceed, EdgeToLevelConverter, raising, falling, level);
    pa_when_abort (proceed, EdgeToLevelConverter, raising, falling, level);
    pa_when_abort (proceed, EdgeToLevelConverter, raising, falling, level);
    pa_when_abort (proceed, EdgeToLevelConverter, raising, falling, level);
    pa_when_abort (proceed, EdgeToLevelConverter, raising, falling, level);
} pa_end

pa_activity (TestEdgeToLevelConverterCheck, pa_ctx(), bool actual, bool expected) {
    pa_always {
        assert(actual == expected);
    } pa_always_end
} pa_end

pa_activity (TestEdgeToLevelConverter, pa_ctx(pa_co_res(4); pa_sig_res;
                                              pa_use(TestEdgeToLevelConverterSpec); pa_use(TestEdgeToLevelConverterTest); pa_use(TestEdgeToLevelConverterCheck);
                                              pa_def_sig(proceed); bool raising; bool falling; bool expected_level; bool actual_level)) {
    pa_co(3) {
        pa_with (TestEdgeToLevelConverterSpec, pa_self.proceed, pa_self.actual_level, pa_self.raising, pa_self.falling, pa_self.expected_level);
        pa_with (TestEdgeToLevelConverterTest, pa_self.proceed, pa_self.raising, pa_self.falling, pa_self.actual_level);
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
