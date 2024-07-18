#include "eject_filament_command.h"

logic::eject_filament_command::eject_filament_command(modules::feeding_system (&_feeding_systems)[config::TOOL_COUNT], modules::hub_filament_sensor& _hub_fsensor, modules::registers& _registers)
    : concrete_command_base(_feeding_systems, _hub_fsensor, _registers)
{
}

bool logic::eject_filament_command::reset_helper(uint8_t param)
{
    if (!check_tool_index(param)) {
        return false;
    }

    _registers.write(modules::register_address::CURRENT_SLOT, param);

    _error = error_code::RUNNING;
    _state = progress_code::EJECTING_FILAMENT;

    return true;
}

bool logic::eject_filament_command::step()
{
    auto& _feeding_system = _feeding_systems[_registers.read(modules::register_address::CURRENT_SLOT)];

    if (_feeding_system.has_filament()) {
        _feeding_system.step(modules::motor_direction::BACKWARD);
        return false;
    }

    _feeding_system.set_filament_position(modules::filament_position::NOT_LOADED);
    finished_ok();

    return true;
}
