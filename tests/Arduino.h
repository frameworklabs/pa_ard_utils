// pa_ard_utils
//
// Copyright (c) 2024, Framework Labs.

#pragma once

#define LOW 0
#define HIGH 1

#define INPUT_PULLDOWN 0
#define INPUT_PULLUP 1

void pinMode(uint8_t, uint8_t);
uint8_t digitalRead(uint8_t);

struct SerialMock {
    void print(char);
    void print(const char*);
    void println(const char*);
};

extern SerialMock Serial;
