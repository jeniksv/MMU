#pragma once

#include "concrete_command_base.h"
#include "config.h"
#include "feeding_system.h"
#include "registers.h"
#include "sensor.h"

namespace logic {

/**
 * @class eject_filament_command
 * @brief Command to eject filament from a 3D printer.
 *
 * This class handles the process of ejecting filament from the feeding system of a 3D printer.
 */
class eject_filament_command : public concrete_command_base {
public:
    /**
     * @brief Constructor for the eject_filament_command class.
     *
     * Initializes the eject_filament_command with references to the feeding systems, hub filament sensor, and registers.
     *
     * @param _feeding_systems Array of feeding systems, one for each tool.
     * @param _hub_fsensor Reference to the hub filament sensor.
     * @param _registers Reference to the registers module.
     */
    eject_filament_command(modules::feeding_system (&_feeding_systems)[config::TOOL_COUNT], modules::hub_filament_sensor& _hub_fsensor, modules::registers& _registers);

    /**
     * @brief Resets the command with the given parameter.
     *
     * This method prepares the command to eject the filament from the specified slot.
     *
     * @param param Index of the filament slot to eject.
     * @return True if the reset is successful, false otherwise.
     */
    bool reset_helper(uint8_t param) override;

    /**
     * @brief Executes a step of the filament ejection process.
     *
     * This method performs a single step in the process of ejecting filament.
     *
     * @return True if the step is successful, false otherwise.
     */
    bool step() override;
};

} // namespace logic
