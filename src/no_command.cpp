#include "no_command.h"

logic::no_command::no_command()
    : logic::command_base()
{
}

bool logic::no_command::reset_helper(uint8_t)
{
    return true;
}

bool logic::no_command::step()
{
    return true;
}
