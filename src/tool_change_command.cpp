#include "tool_change_command.h"

logic::tool_change_command::tool_change_command(logic::unload_from_nozzle_command& _unload_from_nozzle_command, logic::load_to_nozzle_command& _load_to_nozzle_command)
    : command_base()
    , _unload_from_nozzle_command(_unload_from_nozzle_command)
    , _load_to_nozzle_command(_load_to_nozzle_command)
    , _current_stage(logic::tool_change_stage::UNLOAD)
    , _slot_index(0)
{
}

bool logic::tool_change_command::reset_helper(uint8_t param)
{
    _slot_index = param;
    _current_stage = logic::tool_change_stage::UNLOAD;
    return _unload_from_nozzle_command.reset_helper(param);
}

bool logic::tool_change_command::step()
{
    if (_current_stage == logic::tool_change_stage::UNLOAD) {
        _state = _unload_from_nozzle_command.get_state();
        _error = _unload_from_nozzle_command.get_error();

        bool finished = _unload_from_nozzle_command.step();

        if (finished) {
            _current_stage = logic::tool_change_stage::LOAD;
            _load_to_nozzle_command.reset_helper(_slot_index);
        }

        return false;
    }

    _state = _load_to_nozzle_command.get_state();
    _error = _load_to_nozzle_command.get_error();

    return _load_to_nozzle_command.step();
}
