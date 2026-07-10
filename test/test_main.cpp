/**
 * @file test_main.cpp
 * @brief file that will be to test things
 * @date 2026-03-07
 * 
 */
#include <Arduino.h>
#include <unity.h>
#include "transmitter.h"

/**
 * @brief Mock serial class for testing without hardware.
 */
class MockSerial {
public:
    void begin(unsigned long) {}
    void println(const char*) {}
    void println(long) {}
    void print(const char*) {}
};

/**
 * @brief Global transmitter instance using mock serial.
 */
MockSerial mockSerial;
Transmitter<MockSerial> radio(mockSerial, 18, 1);

/**
 * @brief Runs before each test.
 */
void setUp(void) {
}

/**
 * @brief Runs after each test.
 */
void tearDown(void) {
}

/**
 * @brief Basic math test example.
 */
void test_basic_math(void) {
    TEST_ASSERT_EQUAL_INT(5, 2 + 3);
}

/**
 * @brief Test transmitter begin function.
 *
 * Ensures the function executes without crashing.
 */
void test_transmitter_begin(void) {
    radio.begin();
    TEST_ASSERT_TRUE(true);
}

/**
 * @brief Arduino-style test runner.
 */
void setup() {
    UNITY_BEGIN();

    RUN_TEST(test_basic_math);
    RUN_TEST(test_transmitter_begin);

    UNITY_END();
}

void loop() {
}
