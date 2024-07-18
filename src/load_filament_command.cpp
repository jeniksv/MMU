#include "load_filament_command.h"

namespace logic {

load_filament_command::load_filament_command(modules::feeding_system (&_feeding_systems)[config::TOOL_COUNT], modules::hub_filament_sensor& _hub_fsensor, modules::registers& _registers)
    : concrete_command_base(_feeding_systems, _hub_fsensor, _registers)
    , _current_stage()
    , _remaining_additional_steps()
{
}

bool load_filament_command::reset_helper(uint8_t param)
{
    if (!check_tool_index(param)) {
        return false;
    }

    _registers.write(modules::register_address::CURRENT_SLOT, param);

    _remaining_additional_steps = config::HUB_HELP_ADDITIONAL_STEPS;
    _current_stage = load_filament_stage::LOAD_TO_FEEDING_SYSTEM;
    _error = error_code::RUNNING;
    _state = progress_code::LOADING_FILAMENT;

    return true;
}

bool load_filament_command::step()
{
    auto& _feeding_system = _feeding_systems[_registers.read(modules::register_address::CURRENT_SLOT)];

    if (_current_stage == load_filament_stage::LOAD_TO_FEEDING_SYSTEM) {
        return handle_load_to_pulley(_feeding_system);
    }

    if (_current_stage == load_filament_stage::LOAD_TO_HUB) {
        return handle_load_to_hub(_feeding_system);
    }

    return handle_before_hub_park(_feeding_system);
}

bool load_filament_command::handle_load_to_pulley(modules::feeding_system& _feeding_system)
{
    _feeding_system.step(modules::motor_direction::FORWARD);

    if (_feeding_system.has_filament()) {
        _current_stage = load_filament_stage::LOAD_TO_HUB;
        _feeding_system.set_filament_position(modules::filament_position::IN_FEEDING_SYSTEM);
    }

    return false;
}

bool load_filament_command::handle_load_to_hub(modules::feeding_system& _feeding_system)
{
    _feeding_system.step(modules::motor_direction::FORWARD);

    if (_hub_fsensor.is_triggered()) {
        _current_stage = load_filament_stage::PARK_BEFORE_HUB;
        _feeding_system.set_filament_position(modules::filament_position::IN_FILAMENT_HUB);
    }

    return false;
}

bool load_filament_command::handle_before_hub_park(modules::feeding_system& _feeding_system)
{
    if (_remaining_additional_steps > 0) {
        _remaining_additional_steps--;
        _feeding_system.step(modules::motor_direction::BACKWARD);
        return false;
    }

    finished_ok();
    _feeding_system.set_filament_position(modules::filament_position::BEFORE_FILAMENT_HUB);
    return true;
}

} // namespace logic
