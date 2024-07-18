#pragma once

#include "hardware_component_base.h"
#include <stdint.h>

#ifndef UNITTEST
#include "ti_msp_dl_config.h"
#endif

namespace HAL {

/**
 * @enum gpio_pin_state
 * @brief Represents the state of a GPIO pin.
 */
enum class gpio_pin_state {
    LOW,
    HIGH
};

/**
 * @class pwm_pin
 * @brief Represents a PWM (Pulse Width Modulation) pin.
 *
 * This class provides methods to control the duty cycle of a PWM pin.
 */
class pwm_pin : public hardware_component_base {
public:
#ifndef UNITTEST
    /**
     * @brief Constructor for the pwm_pin class.
     *
     * @param gptimer Pointer to the GPTIMER registers.
     * @param timer_index Index of the timer compare/capture register.
     */
    pwm_pin(GPTIMER_Regs* gptimer, DL_TIMER_CC_INDEX timer_index);
#endif

    /**
     * @brief Sets the duty cycle of the PWM pin.
     *
     * @param duty_cycle The duty cycle to set (range: 0.0 to 1.0).
     */
    void set_duty_cycle(float duty_cycle);

    /**
     * @brief Gets the current duty cycle of the PWM pin.
     *
     * @return The current duty cycle (range: 0.0 to 1.0).
     */
    float get_current_duty_cycle() const;

private:
#ifndef UNITTEST
    GPTIMER_Regs* _gptimer;
    DL_TIMER_CC_INDEX _timer_index;
#endif
    float _current_duty_cycle;
};

/**
 * @class adc_pin
 * @brief Represents an ADC (Analog to Digital Converter) pin.
 *
 * This class provides methods to read values from an ADC pin.
 */
class adc_pin : public hardware_component_base {
public:
#ifndef UNITTEST
    /**
     * @brief Constructor for the adc_pin class.
     *
     * @param register_ptr Pointer to the ADC12 registers.
     * @param memory_index Index of the ADC12 memory register.
     */
    adc_pin(ADC12_Regs* register_ptr, DL_ADC12_MEM_IDX memory_index);
#endif

    /**
     * @brief Reads the value from the ADC pin.
     *
     * @return The 12-bit ADC conversion result.
     */
    uint16_t read() const;

private:
#ifndef UNITTEST
    ADC12_Regs* _register_ptr;
    DL_ADC12_MEM_IDX _memory_index;
#endif
};

/**
 * @class gpio_pin
 * @brief Represents a GPIO (General Purpose Input/Output) pin.
 *
 * This class provides methods to control and read the state of a GPIO pin.
 */
class gpio_pin : public hardware_component_base {
public:
#ifndef UNITTEST
    /**
     * @brief Constructor for the gpio_pin class.
     *
     * @param register_ptr Pointer to the GPIO registers.
     * @param pin Pin number.
     */
    gpio_pin(GPIO_Regs* register_ptr, uint32_t pin);
#endif

    /**
     * @brief Writes a state to the GPIO pin.
     *
     * @param state The state to write (LOW or HIGH).
     */
    void write(gpio_pin_state state);

    /**
     * @brief Toggles the state of the GPIO pin.
     */
    void toggle();

    /**
     * @brief Gets the current state of the GPIO pin.
     *
     * @return The current state (LOW or HIGH).
     */
    gpio_pin_state get_state() const;

private:
#ifndef UNITTEST
    GPIO_Regs* _register_ptr;
    uint32_t _pin;
#endif
    gpio_pin_state _state;
};

} // namespace HAL
