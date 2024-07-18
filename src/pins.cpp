#include "pins.h"

namespace HAL {

#ifndef UNITTEST
pwm_pin::pwm_pin(GPTIMER_Regs* _gptimer, DL_TIMER_CC_INDEX _timer_index)
    : _gptimer(_gptimer)
    , _timer_index(_timer_index)
    , _current_duty_cycle(0)
{
}
#endif

void pwm_pin::set_duty_cycle(float percent)
{
    if (percent < 0 || percent > 100) {
        _current_duty_cycle = 0;
    } else {
        _current_duty_cycle = percent;
    }
    uint16_t pwmValue = static_cast<uint16_t>(255 * (_current_duty_cycle / 100));
#ifndef UNITTEST
    DL_Timer_setCaptureCompareValue(_gptimer, _current_duty_cycle, _timer_index);
#endif
}

float pwm_pin::get_current_duty_cycle() const
{
    return _current_duty_cycle;
}

#ifndef UNITTEST
adc_pin::adc_pin(ADC12_Regs* _register_ptr, DL_ADC12_MEM_IDX _memory_index)
    : _register_ptr(_register_ptr)
    , _memory_index(_memory_index)
{
}
#endif

uint16_t adc_pin::read() const
{
#ifndef UNITTEST
    DL_ADC12_startConversion(_register_ptr);
    return DL_ADC12_getMemResult(_register_ptr, _memory_index);
#else
    return 0;
#endif
}

#ifndef UNITTEST
gpio_pin::gpio_pin(GPIO_Regs* _register_ptr, uint32_t _pin)
    : _register_ptr(_register_ptr)
    , _pin(_pin)
    , _state(gpio_pin_state::LOW)
{
}
#endif

void gpio_pin::write(gpio_pin_state s)
{
    _state = s;
#ifndef UNITTEST
    if (_state == gpio_pin_state::HIGH) {
        DL_GPIO_setPins(_register_ptr, _pin);
    } else {
        DL_GPIO_clearPins(_register_ptr, _pin);
    }
#endif
}

void gpio_pin::toggle()
{
    _state = _state == gpio_pin_state::LOW ? gpio_pin_state::HIGH : gpio_pin_state::LOW;
#ifndef UNITTEST
    DL_GPIO_togglePins(_register_ptr, _pin);
#endif
}

gpio_pin_state gpio_pin::get_state() const
{
    return _state;
}
}
