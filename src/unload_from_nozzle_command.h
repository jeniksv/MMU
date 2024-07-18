#pragma once

#include "concrete_command_base.h"
#include "config.h"
#include "feeding_system.h"
#include "registers.h"
#include "sensor.h"

namespace logic {

/**
 * @class unload_from_nozzle_command
 * @brief Command to unload filament from a 3D printer nozzle.
 *
 * This class handles the process of unloading filament from the nozzle of a 3D printer.
 */
class unload_from_nozzle_command : public concrete_command_base {
public:
    /**
     * @brief Constructor for the unload_from_nozzle_command class.
     *
     * @param _feeding_systems Array of feeding systems, one for each tool.
     * @param _hub_fsensor Reference to the hub filament sensor.
     * @param _registers Reference to the registers module.
     */
    unload_from_nozzle_command(modules::feeding_system (&_feeding_systems)[config::TOOL_COUNT], modules::hub_filament_sensor& _hub_fsensor, modules::registers& _registers);

    /**
     * @brief Resets the command with a given parameter.
     *
     * @param param This parameter is not used in this function.
     * @return True if the reset is successful, false otherwise.
     */
    bool reset_helper(uint8_t param) override;

    /**
     * @brief Executes a step of the filament unloading process.
     *
     * @return True if the step is successful, false otherwise.
     */
    bool step() override;

private:
    uint16_t _remaining_additional_steps;
};

} // namespace logic
