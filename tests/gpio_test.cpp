#include "pins.h"
#include "test_base.h"

class GPIOTest : public TestBase { };

TEST_F(GPIOTest, pwm_pinset_duty_cycle)
{
    float dutyCycle = 50.0f;
    aPwmPin.set_duty_cycle(dutyCycle);
    ASSERT_FLOAT_EQ(aPwmPin.get_current_duty_cycle(), dutyCycle);
}

TEST_F(GPIOTest, pwm_pinInvalidDutyCycle)
{
    aPwmPin.set_duty_cycle(110);
    ASSERT_TRUE(aPwmPin.get_current_duty_cycle() >= 0.0f && aPwmPin.get_current_duty_cycle() <= 100.0f);
}

TEST_F(GPIOTest, adc_pinread)
{
    uint16_t adcValue = aMotorAdcPin.read();
    ASSERT_GE(adcValue, 0);
}

TEST_F(GPIOTest, gpio_pinwritereadget_state)
{
    stby1Pin.write(HAL::gpio_pin_state::HIGH);
    ASSERT_EQ(stby1Pin.get_state(), HAL::gpio_pin_state::HIGH);

    stby1Pin.write(HAL::gpio_pin_state::LOW);
    ASSERT_EQ(stby1Pin.get_state(), HAL::gpio_pin_state::LOW);

    stby1Pin.write(HAL::gpio_pin_state::HIGH);
    ASSERT_EQ(stby1Pin.get_state(), HAL::gpio_pin_state::HIGH);
}
