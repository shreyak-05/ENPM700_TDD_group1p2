#pragma once
#include <memory>
#include <stdexcept>

class PIDImpl;

/**
 * @brief PID controller implementation using the Pimpl idiom.
 *
 * This class implements a Proportional-Integral-Derivative (PID) controller,
 * which continuously calculates an error value as the difference between a
 * desired setpoint and a measured process variable, and applies a correction
 * based on proportional, integral, and derivative terms.
 *
 * The implementation uses the Pimpl (Pointer to Implementation) idiom to
 * hide implementation details and reduce compilation dependencies.
 *
 * @note This class is move-only (non-copyable).
 */
class PID {
 public:
  /**
   * @brief Constructs a PID controller with the specified parameters.
   *
   * @param dt Loop interval time in seconds. Must be positive.
   * @param max Maximum value of the manipulated variable (upper saturation
   * limit).
   * @param min Minimum value of the manipulated variable (lower saturation
   * limit).
   * @param Kp Proportional gain. Controls the response to the current error.
   * @param Kd Derivative gain. Controls the response to the rate of error
   * change.
   * @param Ki Integral gain. Controls the response to accumulated error over
   * time.
   *
   * @throws std::invalid_argument if dt <= 0 or max < min
   */
  PID(double dt, double max, double min, double Kp, double Kd, double Ki);

  /**
   * @brief Calculates the manipulated variable based on setpoint and process
   * variable.
   *
   * Computes the PID output by evaluating the proportional, integral, and
   * derivative terms based on the error (setpoint - pv). The output is
   * clamped to the [min, max] range specified during construction.
   *
   * @param setpoint The desired target value.
   * @param pv The current process variable (measured value).
   * @return The calculated manipulated variable, clamped to [min, max].
   */
  double calculate(double setpoint, double pv);

  /**
   * @brief Destructor.
   */
  ~PID() noexcept;

  /**
   * @brief Deleted copy constructor.
   *
   * PID controllers maintain internal state and cannot be copied.
   */
  PID(const PID&) = delete;

  /**
   * @brief Deleted copy assignment operator.
   *
   * PID controllers maintain internal state and cannot be copied.
   */
  PID& operator=(const PID&) = delete;

  /**
   * @brief Move constructor.
   *
   * @param other The PID object to move from.
   */
  PID(PID&&) noexcept;

  /**
   * @brief Move assignment operator.
   *
   * @param other The PID object to move from.
   * @return Reference to this object.
   */
  PID& operator=(PID&&) noexcept;

 private:
  std::unique_ptr<PIDImpl> pimpl;  ///< Pointer to implementation.
};