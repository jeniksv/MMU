#include "feeding_system.h"

modules::feeding_system::feeding_system(modules::dc_motor& _motor, modules::feeding_system_filament_sensor& _filament_sensor, modules::MotorHalSensor& _motor_sensor)
    : _motor(_motor)
    , _filament_sensor(_filament_sensor)
    , _motor_sensor(_motor_sensor)
    , _current_filament_position(filament_position::NOT_LOADED)
{
}

bool modules::feeding_system::has_filament()
{
    return _filament_sensor.is_triggered();
}

void modules::feeding_system::step(modules::motor_direction direction, float pwmPercent)
{
    long stepstart = HAL::GetTime();

    _motor.start(direction, pwmPercent);

    while (_motor_sensor.is_triggered()) {
        if (HAL::GetTime() - stepstart > config::FEEDING_SYSTEM_STEP_TIMEOUT) {
            break;
        }
    }
    while (!_motor_sensor.is_triggered()) {
        if (HAL::GetTime() - stepstart > config::FEEDING_SYSTEM_STEP_TIMEOUT) {
            break;
        }
    }

    _motor.stop();
}

void modules::feeding_system::panic()
{
    _motor.panic();
}

void modules::feeding_system::set_filament_position(filament_position position)
{
    _current_filament_position = position;
}

modules::filament_position modules::feeding_system::get_filament_position() const
{
    return _current_filament_position;
}
