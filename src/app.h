#pragma once

#include "command_base.h"
#include "config.h"
#include "eject_filament_command.h"
#include "feeding_system.h"
#include "load_filament_command.h"
#include "load_to_nozzle_command.h"
#include "no_command.h"
#include "protocol.h"
#include "registers.h"
#include "sensor.h"
#include "tool_change_command.h"
#include "uart.h"
#include "unload_from_nozzle_command.h"

/**
 * @class app
 * @brief Manages high-level application logic for printer operations.
 *
 * This class orchestrates the execution of printer commands, management of sensor readings, and communication
 * with external interfaces. It handles various tasks like command planning, processing, and reporting states
 * to ensure smooth operation and integration of the printer's functionalities.
 */
class app {
public:
    /**
     * Constructor for the app class.
     * @param _feeding_systems Reference to an array of feeding system modules.
     * @param _hub_fsensor Reference to the filament sensor hub module.
     * @param c Reference to a UART communication interface.
     * @param _registers Reference to the registers module handling low-level device state.
     */
    app(modules::feeding_system (&_feeding_systems)[config::TOOL_COUNT], modules::hub_filament_sensor&, HAL::UART& c, modules::registers&);

    /**
     * Executes one step of the application logic.
     */
    void step();

private:
    /**
     * Checks if there is a pending request to be processed.
     * @return true if there is a pending request, false otherwise.
     */
    bool has_pending_request();

    /**
     * Processes an incoming request.
     * @param rq The request to be processed.
     */
    void process_request(const modules::request& rq);

    /**
     * Reports the result of a read register operation.
     * @param rq The request that initiated the read operation.
     */
    void report_read_register(const modules::request& rq);

    /**
     * Reports the result of a write register operation.
     * @param rq The request that initiated the write operation.
     */
    void report_write_register(const modules::request& rq);

    /**
     * Reports the status of the filament sensor.
     * @param rq The request that inquired about the filament sensor.
     */
    void report_fsensor(const modules::request& rq);

    /**
     * Reports if new command was accepted.
     * @param rq The request that caused the command change.
     * @param response_code The response code indicating the result of the command change.
     */
    void report_command_change(const modules::request& rq, modules::response_code);

    /**
     * Reports the currently running command to the communication interface.
     */
    void report_running_command();

    /**
     * Plans and initializes the execution of a command based on the request.
     * @param rq The request containing the command details.
     */
    void plan_command(const modules::request& rq);

    modules::current_command_state current_command_state() const;

    modules::feeding_system (&_feeding_systems)[config::TOOL_COUNT];
    modules::hub_filament_sensor& _hub_fsensor;
    modules::registers& _registers;

    logic::no_command _no_command;
    logic::load_filament_command _load_filament_command;
    logic::eject_filament_command _eject_filament_command;
    logic::unload_from_nozzle_command _unload_from_nozzle_command;
    logic::load_to_nozzle_command _load_to_nozzle_command;
    logic::tool_change_command _tool_change_command;
    logic::command_base* _current_command;

    modules::request _current_command_request;
    HAL::UART& _communication;

    modules::protocol _protocol;
};
