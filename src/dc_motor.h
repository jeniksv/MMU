#pragma once

#include "pins.h"

namespace modules {

/**
 * @enum motor_direction
 * @brief Represents the direction of the motor.
 */
enum class motor_direction {
    FORWARD,
    BACKWARD
};

/**
 * @class dc_motor
 * @brief Controls a DC motor.
 *
 * This class provides methods to control a DC motor, including starting, stopping,
 * changing direction, and handling emergency stop (panic).
 */
class dc_motor {
public:
    /**
     * @brief Constructor for the dc_motor class.
     *
     * Initializes the DC motor with the given control pins.
     *
     * @param _pwm_pin Reference to the PWM pin used to control motor speed.
     * @param _stby_pin Reference to the standby pin used to enable/disable the motor.
     * @param _in1_pin Reference to the IN1 pin used to control motor direction.
     * @param _in2_pin Reference to the IN2 pin used to control motor direction.
     */
    dc_motor(HAL::pwm_pin& _pwm_pin, HAL::gpio_pin& _stby_pin, HAL::gpio_pin& _in1_pin, HAL::gpio_pin& _in2_pin);

    /**
     * @brief Stops the motor immediately.
     *
     * This method is used in emergency situations to stop the motor.
     */
    void panic();

    /**
     * @brief Starts the motor in the specified direction with a given PWM percentage.
     *
     * @param d The direction to start the motor.
     * @param pwmPercent The PWM percentage to control motor speed (default is 50%).
     */
    void start(motor_direction d, float pwmPercent = 50);

    /**
     * @brief Stops the motor.
     *
     * This method stops the motor by disabling the PWM signal and setting the standby pin.
     */
    void stop();

    /**
     * @brief Gets the current direction of the motor.
     *
     * @return The current direction of the motor.
     */
    motor_direction get_direction() const;

    /**
     * @brief Changes the direction of the motor.
     *
     * This method toggles the direction of the motor between FORWARD and BACKWARD.
     */
    void change_direction();

    /**
     * @brief Sets the direction of the motor.
     *
     * @param d The direction to set for the motor.
     */
    void set_direction(motor_direction d);

private:
    motor_direction _direction;

    HAL::pwm_pin& _pwm_pin;
    HAL::gpio_pin& _stby_pin;
    HAL::gpio_pin& _in1_pin;
    HAL::gpio_pin& _in2_pin;
};

} // namespace modules
