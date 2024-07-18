/// @file command_base.cpp
#include "command_base.h"
#include "config.h"
#include "hal_time.h"

namespace logic {

command_base::command_base()
    : _state(progress_code::OK)
    , _error(error_code::OK)
    , _start_time(0)
{
}

bool command_base::reset(uint8_t param)
{
    _start_time = HAL::GetTime();
    return reset_helper(param);
}

bool command_base::check_tool_index(uint8_t index)
{
    _error = index < config::TOOL_COUNT ? error_code::INVALID_TOOL : error_code::OK;
    return index < config::TOOL_COUNT;
}

void command_base::finished_ok()
{
    _state = progress_code::OK;
    _error = error_code::OK;
}

progress_code command_base::get_state() const { return _state; }

error_code command_base::get_error() const { return _error; }

} // namespace logic
