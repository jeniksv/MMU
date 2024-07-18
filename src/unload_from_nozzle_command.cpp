#include "unload_from_nozzle_command.h"

logic::unload_from_nozzle_command::unload_from_nozzle_command(modules::feeding_system (&_feeding_systems)[config::TOOL_COUNT], modules::hub_filament_sensor& _hub_fsensor, modules::registers& _registers)
    : concrete_command_base(_feeding_systems, _hub_fsensor, _registers)
    , _remaining_additional_steps(0)
{
}

bool logic::unload_from_nozzle_command::reset_helper(uint8_t param)
{
    auto& _feeding_system = _feeding_systems[_registers.read(modules::register_address::CURRENT_SLOT)];
    _registers.write(modules::register_address::FEEDING_SYSTEM_FSENSOR_STATE, static_cast<uint16_t>(_feeding_system.has_filament()));

    _remaining_additional_steps = config::HUB_HELP_ADDITIONAL_STEPS;
    _error = error_code::RUNNING;
    _state = progress_code::UNLOADING_FILAMENT;

    return true;
}

bool logic::unload_from_nozzle_command::step()
{
    auto& _feeding_system = _feeding_systems[_registers.read(modules::register_address::CURRENT_SLOT)];

    if (_hub_fsensor.is_triggered()) {
        _feeding_system.step(modules::motor_direction::BACKWARD, config::MOTOR_PWM_FAST);
        return false;
    }

    if (_remaining_additional_steps > 0) {
        _remaining_additional_steps--;
        _feeding_system.step(modules::motor_direction::BACKWARD);
        return false;
    }

    _feeding_system.set_filament_position(modules::filament_position::BEFORE_FILAMENT_HUB);
    finished_ok();

    return true;
}
