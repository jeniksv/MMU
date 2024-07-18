#include "eject_filament_command.h"

logic::concrete_command_base::concrete_command_base(modules::feeding_system (&_feeding_systems)[config::TOOL_COUNT], modules::hub_filament_sensor& _hub_fsensor, modules::registers& _registers)
    : command_base()
    , _feeding_systems(_feeding_systems)
    , _hub_fsensor(_hub_fsensor)
    , _registers(_registers)
{
}
