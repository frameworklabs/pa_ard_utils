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

// TestLevelToEdgeConverter Tests

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
} pa_end;

pa_activity (TestLevelToEdgeConverter, pa_ctx(pa_co_res(4); pa_sig_res;
                                              pa_use(TestLevelToEdgeConverterSpec); pa_use(TestLevelToEdgeConverterTest); pa_use(TestLevelToEdgeConverterCheck);
                                              pa_def_sig(proceed); bool level; bool expected_raising; bool expected_falling; pa_def_sig(actual_raising); pa_def_sig(actual_falling))) {
    pa_co(3) {
        pa_with (TestLevelToEdgeConverterSpec, pa_self.proceed, pa_self.level, pa_self.expected_raising, pa_self.expected_falling);
        pa_with (TestLevelToEdgeConverterTest, pa_self.proceed, pa_self.level, pa_self.actual_raising, pa_self.actual_falling);
        pa_with_weak (TestLevelToEdgeConverterCheck, pa_self.actual_raising, pa_self.actual_falling, pa_self.expected_raising, pa_self.expected_falling);
    } pa_co_end
} pa_end

// Test Driver

#define run_test(nm) \
    pa_use(nm); \
    while (pa_tick(nm) == PA_RC_WAIT) {}

int main(int argc, char* argv[]) {
    std::cout << "Start" << std::endl;

    run_test(TestLevelToEdgeConverter);

    std::cout << "Done" << std::endl;

    return 0;
}
