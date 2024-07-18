#pragma once

#ifdef UNITTEST
#include <cstddef>
#endif
#include "config.h"
#include "pins.h"

namespace modules {

template <typename T, uint16_t SIZE>
class sliding_window {
public:
    sliding_window()
        : _sum(0)
        , _index(0)
        , _count(0)
    {
    }

    void update_with(T value)
    {
        if (_count == SIZE) {
            _sum -= _values[_index];
        } else {
            _count++;
        }

        _values[_index] = value;
        _sum += value;

        _index = (_index + 1) % SIZE;
    }

    T get_average() const
    {
        if (_count == 0)
            return 0;
        return _sum / _count;
    }

private:
    T _values[SIZE];
    T _sum;
    uint16_t _index;
    uint16_t _count;
};

class exponential_smoothing {
public:
    exponential_smoothing(float _alpha) : _alpha(_alpha), _smoothed_value(0), _initialized(false) {}

    float update_with(float observation) {
	_smoothed_value = _alpha * observation + (1 - _alpha) * _smoothed_value;
        
	if (!_initialized) {
            _smoothed_value = observation;
            _initialized = true;
	}
        
        return _smoothed_value;
    }

    float get_smoothed_value() const {
        return _smoothed_value;
    }

private:
    float _alpha;
    float _smoothed_value;
    bool _initialized;
};

template <uint16_t SLIDING_WINDOW_SIZE, uint16_t THRESHOLD_LOWER_BOUND, uint16_t THRESHOLD_UPPER_BOUND>
class hall_effect_sensor {
public:
    hall_effect_sensor(HAL::adc_pin& adcPin)
        : adcPin(adcPin)
    {
    }

    bool is_triggered()
    {
#ifdef UNITTEST
        return value;
#else
        _sliding_window.update_with(adcPin.read());
        uint16_t value = _sliding_window.get_average();
        return value > THRESHOLD_LOWER_BOUND && value < THRESHOLD_UPPER_BOUND;
#endif
    }

#ifdef UNITTEST
    void set_return_value(bool v)
    {
        value = v;
    }
#endif

private:
    HAL::adc_pin& adcPin;
    sliding_window<uint16_t, SLIDING_WINDOW_SIZE> _sliding_window;
#ifdef UNITTEST
    uint16_t value;
#endif
};

using feeding_system_filament_sensor = hall_effect_sensor<200, config::FILAMENT_SENSOR_LOWER_BOUND, config::FILAMENT_SENSOR_UPPER_BOUND>;
using hub_filament_sensor = hall_effect_sensor<30, config::HUB_FSENSOR_LOWER_BOUND, config::HUB_FSENSOR_UPPER_BOUND>;
using MotorHalSensor = hall_effect_sensor<20, config::MOTOR_SENSOR_LOWER_BOUND, config::MOTOR_SENSOR_UPPER_BOUND>;

}
