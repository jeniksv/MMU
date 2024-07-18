#pragma once

#include "concrete_command_base.h"
#include "config.h"
#include "feeding_system.h"
#include "registers.h"
#include "sensor.h"

namespace logic {

/**
 * @class load_to_nozzle_command
 * @brief Command to load filament to the 3D printer nozzle.
 *
 * This class handles the process of loading filament to the nozzle of a 3D printer.
 */
class load_to_nozzle_command : public concrete_command_base {
public:
    /**
     * @brief Constructor for the load_to_nozzle_command class.
     *
     * @param _feeding_systems Array of feeding systems, one for each tool.
     * @param _hub_fsensor Reference to the hub filament sensor.
     * @param _registers Reference to the registers module.
     */
    load_to_nozzle_command(modules::feeding_system (&_feeding_systems)[config::TOOL_COUNT], modules::hub_filament_sensor& _hub_fsensor, modules::registers& _registers);

    /**
     * @brief Resets the command with the given parameter.
     *
     * @param param Index of the slot for the tool change.
     * @return True if the reset is successful, false otherwise.
     */
    bool reset_helper(uint8_t param) override;

    /**
     * @brief Executes a step of the filament loading process.
     *
     * @return True if the step is successful, false otherwise.
     */
    bool step() override;

private:
    /**
     * @brief Handles the additional steps required for loading filament.
     *
     * @param feedingSystem Reference to the feeding system.
     * @return True if all additional steps were made, false otherwise.
     */
    bool handle_additional_steps(modules::feeding_system& feedingSystem);

    uint16_t _remaining_additional_steps;
};

} // namespace logic
