#include "load_to_nozzle_command.h"

logic::load_to_nozzle_command::load_to_nozzle_command(modules::feeding_system (&_feeding_systems)[config::TOOL_COUNT], modules::hub_filament_sensor& _hub_fsensor, modules::registers& _registers)
    : concrete_command_base(_feeding_systems, _hub_fsensor, _registers)
    , _remaining_additional_steps(0)
{
}

bool logic::load_to_nozzle_command::reset_helper(uint8_t param)
{
    if (!check_tool_index(param)) {
        return false;
    }

    _registers.write(modules::register_address::CURRENT_SLOT, param);
    _remaining_additional_steps = config::EXTRUDER_HELP_ADDITIONAL_STEPS;

    _error = error_code::RUNNING;
    _state = progress_code::FEEDING_TO_NOZZLE;

    return true;
}

bool logic::load_to_nozzle_command::step()
{
    auto& _feeding_system = _feeding_systems[_registers.read(modules::register_address::CURRENT_SLOT)];

    if (!_registers.read(modules::register_address::EXTRUDER_FSENSOR_STATE)) {
        _feeding_system.step(modules::motor_direction::FORWARD);
        return false;
    }

    _feeding_system.set_filament_position(modules::filament_position::IN_BONDTECH);
    finished_ok();

    return handle_additional_steps(_feeding_system);
}

bool logic::load_to_nozzle_command::handle_additional_steps(modules::feeding_system& _feeding_system)
{
    if (_remaining_additional_steps > 0) {
        _remaining_additional_steps--;
        _feeding_system.step(modules::motor_direction::FORWARD);
        return false;
    }

    _feeding_system.set_filament_position(modules::filament_position::IN_NOZZLE);
    return true;
}
