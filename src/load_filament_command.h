#pragma once

#include "concrete_command_base.h"
#include "config.h"
#include "feeding_system.h"
#include "registers.h"
#include "sensor.h"

namespace logic {

/**
 * @enum load_filament_stage
 * @brief Stages of the filament loading process.
 */
enum class load_filament_stage {
    LOAD_TO_FEEDING_SYSTEM,
    LOAD_TO_HUB,
    PARK_BEFORE_HUB
};

/**
 * @class load_filament_command
 * @brief Command to load filament into a 3D printer.
 *
 * This class handles the process of loading filament into the feeding system of a 3D printer.
 */
class load_filament_command : public concrete_command_base {
public:
    /**
     * @brief Constructor for the load_filament_command class.
     *
     * @param _feeding_systems Array of feeding systems, one for each tool.
     * @param _hub_fsensor Reference to the hub filament sensor.
     * @param _registers Reference to the registers module.
     */
    load_filament_command(modules::feeding_system (&_feeding_systems)[config::TOOL_COUNT], modules::hub_filament_sensor& _hub_fsensor, modules::registers& _registers);

    /**
     * @brief Resets the command with a given parameter.
     *
     * @param param Index of the filament slot to load.
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
     * @brief Handles the stage of loading filament to the pulley.
     *
     * @param feedingSystem Reference to the feeding system.
     * @return True if the stage is handled successfully, false otherwise.
     */
    bool handle_load_to_pulley(modules::feeding_system& feedingSystem);

    /**
     * @brief Handles the stage of loading filament to the hub.
     *
     * @param feedingSystem Reference to the feeding system.
     * @return True if the stage is handled successfully, false otherwise.
     */
    bool handle_load_to_hub(modules::feeding_system& feedingSystem);

    /**
     * @brief Handles the stage of parking filament before the hub.
     *
     * @param feedingSystem Reference to the feeding system.
     * @return True if the stage is handled successfully, false otherwise.
     */
    bool handle_before_hub_park(modules::feeding_system& feedingSystem);

    load_filament_stage _current_stage;
    uint16_t _remaining_additional_steps;
};

} // namespace logic
