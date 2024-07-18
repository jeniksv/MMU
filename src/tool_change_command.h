#pragma once

#include "command_base.h"
#include "config.h"
#include "feeding_system.h"
#include "load_to_nozzle_command.h"
#include "registers.h"
#include "sensor.h"
#include "unload_from_nozzle_command.h"

namespace logic {

/**
 * @enum tool_change_stage
 * @brief Stages of the tool change process.
 */
enum class tool_change_stage {
    UNLOAD,
    LOAD
};

/**
 * @class tool_change_command
 * @brief Command to handle tool changes in a 3D printer.
 *
 * This class manages the process of unloading the current tool and loading a new tool in a 3D printer.
 */
class tool_change_command : public command_base {
public:
    /**
     * @brief Constructor for the tool_change_command class.
     *
     * @param _unload_from_nozzle_command Reference to the command for unloading filament from the nozzle.
     * @param _load_to_nozzle_command Reference to the command for loading filament to the nozzle.
     */
    tool_change_command(logic::unload_from_nozzle_command& _unload_from_nozzle_command, logic::load_to_nozzle_command& _load_to_nozzle_command);

    /**
     * @brief Resets the command with the given parameter.
     *
     * @param param Index of the slot for the tool change.
     * @return True if the reset is successful, false otherwise.
     */
    bool reset_helper(uint8_t param) override;

    /**
     * @brief Executes a step of the tool change process.
     *
     * @return True if the step is successful, false otherwise.
     */
    bool step() override;

private:
    logic::unload_from_nozzle_command& _unload_from_nozzle_command;
    logic::load_to_nozzle_command& _load_to_nozzle_command;

    tool_change_stage _current_stage;
    uint8_t _slot_index;
};

} // namespace logic
