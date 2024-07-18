#include "dc_motor.h"

using namespace modules;

dc_motor::dc_motor(HAL::pwm_pin& _pwm_pin, HAL::gpio_pin& _stby_pin, HAL::gpio_pin& _in1_pin, HAL::gpio_pin& _in2_pin)
    : _pwm_pin(_pwm_pin)
    , _stby_pin(_stby_pin)
    , _in1_pin(_in1_pin)
    , _in2_pin(_in2_pin)
{
}

void dc_motor::panic()
{
    _stby_pin.write(HAL::gpio_pin_state::LOW);
}

void dc_motor::start(motor_direction d, float pwmPercent)
{
    set_direction(d);
    _stby_pin.write(HAL::gpio_pin_state::HIGH);
    _pwm_pin.set_duty_cycle(pwmPercent);
}

void dc_motor::stop()
{
    _pwm_pin.set_duty_cycle(0);
}

motor_direction dc_motor::get_direction() const
{
    return _direction;
}

void dc_motor::change_direction()
{
    _in1_pin.toggle();
    _in2_pin.toggle();
}

void dc_motor::set_direction(motor_direction d)
{
    _direction = d;
    _in1_pin.write(_direction == motor_direction::FORWARD ? HAL::gpio_pin_state::HIGH : HAL::gpio_pin_state::LOW);
    _in2_pin.write(_direction == motor_direction::FORWARD ? HAL::gpio_pin_state::LOW : HAL::gpio_pin_state::HIGH);
}
