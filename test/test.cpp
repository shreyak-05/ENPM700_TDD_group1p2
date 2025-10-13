#include <gtest/gtest.h>

#include <cmath>
#include <stdexcept>

#include "pid.hpp"

// Test 1: PID calculation
TEST(PIDControllerTest, SanityCheck) {
  PID pid(0.1, 100, -100, 1.0, 0.5, 0.1);
  // error = 2
  // P = 1.0 * 2 = 2.0
  // I = 0.1 * (2 * 0.1) = 0.02
  // D = 0.5 * (2 / 0.1) = 10.0
  // Total = 2.0 + 0.02 + 10.0 = 12.02
  double output = pid.calculate(10.0, 8.0);
  EXPECT_NEAR(output, 12.02, 0.01);
}

// Test 2: Output clamping
TEST(PIDControllerTest, OutputClamping) {
  PID pid(0.1, 10.0, -10.0, 100.0, 0.0, 0.0);

  EXPECT_DOUBLE_EQ(pid.calculate(100.0, 0.0), 10.0);    // Should clamp to max
  EXPECT_DOUBLE_EQ(pid.calculate(-100.0, 0.0), -10.0);  // Should clamp to min
}

// Test 3: Zero error case
TEST(PIDControllerTest, ZeroError) {
  PID pid(0.1, 100, -100, 1.0, 0.0, 0.0);
  EXPECT_DOUBLE_EQ(pid.calculate(10.0, 10.0), 0.0);
}

// Test 4: Invalid constructor parameters
TEST(PIDControllerTest, InvalidConstructorParameters) {
  EXPECT_THROW(PID(0.0, 100, -100, 1, 1, 1), std::invalid_argument);   // dt = 0
  EXPECT_THROW(PID(-0.1, 100, -100, 1, 1, 1), std::invalid_argument);  // dt < 0
  EXPECT_THROW(PID(0.1, -100, 100, 1, 1, 1),
               std::invalid_argument);  // max < min
}

// Test 5: Move semantics (constructor and assignment)
TEST(PIDControllerTest, MoveSemantics) {
  PID pid1(0.1, 100, -100, 1.0, 0.5, 0.1);

  // Test move constructor
  PID pid2(std::move(pid1));
  EXPECT_NO_THROW(pid2.calculate(10, 8));
  EXPECT_THROW(pid1.calculate(10, 8), std::runtime_error);

  // Test move assignment
  PID pid3(0.1, 50, -50, 2.0, 1.0, 0.5);
  pid3 = std::move(pid2);
  EXPECT_NO_THROW(pid3.calculate(10, 8));
  EXPECT_THROW(pid2.calculate(10, 8), std::runtime_error);
}
