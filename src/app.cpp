#include "app.h"
#include "protocol.h"
#include <cstdint>

app::app(modules::feeding_system (&ps)[config::TOOL_COUNT], modules::hub_filament_sensor& h, HAL::UART& c, modules::registers& r)
    : _feeding_systems(ps)
    , _hub_fsensor(h)
    , _communication(c)
    , _registers(r)
    , _no_command()
    , _load_filament_command(_feeding_systems, _hub_fsensor, _registers)
    , _eject_filament_command(_feeding_systems, _hub_fsensor, _registers)
    , _unload_from_nozzle_command(_feeding_systems, _hub_fsensor, _registers)
    , _load_to_nozzle_command(_feeding_systems, _hub_fsensor, _registers)
    , _tool_change_command(_unload_from_nozzle_command, _load_to_nozzle_command)
    , _current_command_request(modules::request_code::UNKNOWN)
    , _current_command(&_no_command)
    , _protocol()
{
}

void app::step()
{
    if (has_pending_request()) {
        auto temp = _protocol.get_request();
        process_request(temp);
    }

    if (_current_command->step()) {
        _current_command = &_no_command;
    }
}

bool app::has_pending_request()
{
    while (_communication.is_available()) {
        auto temp = _communication.read_byte();

        if (_protocol.parse_byte(temp) == modules::request_state::MESSAGE_COMPLETED) {
            return true;
        }
    }

    return false;
}

void app::process_request(const modules::request& _request)
{
    // modules::response_parameters response_params;

    switch (_request.code) {
    case modules::request_code::VERSION:
    case modules::request_code::READ:
        report_read_register(_request);
        break;

    case modules::request_code::WRITE:
        report_write_register(_request);
        break;

    case modules::request_code::FEEDING_SYSTEM_FILAMENT_SENSOR:
        report_fsensor(_request);
        break;

    case modules::request_code::QUERY:
        report_running_command();
        break;

    case modules::request_code::LOAD:
    case modules::request_code::UNLOAD:
    case modules::request_code::EJECT:
    case modules::request_code::TOOL:
        plan_command(_request);
        break;

    case modules::request_code::EXTRUDER_FILAMENT_SENSOR:
        _registers.write(modules::register_address::EXTRUDER_FSENSOR_STATE, _request.first_argument);
        report_command_change(_request, modules::response_code::ACCEPTED);
        break;

    default:
        break;
    }
}

void app::report_read_register(const modules::request& rq)
{
    uint8_t rsp[config::MAX_RESPONSE_SIZE];
    uint16_t second_argument = _registers.read(static_cast<modules::register_address>(rq.first_argument));

    // modules::response_parameters temp(rq, rsp, true, second_argument);
    auto temp = modules::response_parameters::create(rq, rsp).with_response_code(modules::response_code::ACCEPTED).is_accepted(true).with_second_arg(second_argument).build();
    uint8_t len = _protocol.encode_response(temp);
    _communication.write(rsp, len);
}

void app::report_write_register(const modules::request& rq)
{
    uint8_t rsp[config::MAX_RESPONSE_SIZE];
    _registers.write(static_cast<modules::register_address>(rq.first_argument), rq.second_argument);

    auto temp = modules::response_parameters::create(rq, rsp).with_response_code(modules::response_code::ACCEPTED).build();
    uint8_t len = _protocol.encode_response(temp);
    _communication.write(rsp, len);
}

void app::report_fsensor(const modules::request& rq)
{
    uint8_t rsp[config::MAX_RESPONSE_SIZE];
    auto temp = modules::response_parameters::create(rq, rsp).build();
    uint8_t len = _protocol.encode_response(temp);
    _communication.write(rsp, len);
}

void app::report_command_change(const modules::request& rq, modules::response_code stat)
{
    uint8_t rsp[config::MAX_RESPONSE_SIZE];
    auto temp = modules::response_parameters::create(rq, rsp).with_response_code(stat).build();
    uint8_t len = _protocol.encode_response(temp);
    _communication.write(rsp, len);
}

void app::report_running_command()
{
    uint8_t rsp[config::MAX_RESPONSE_SIZE];
    auto temp = modules::response_parameters::create(_current_command_request, rsp).with_command_state(current_command_state()).build();
    uint8_t len = _protocol.encode_response(temp);
    _communication.write(rsp, len);
}

void app::plan_command(const modules::request& rq)
{
    if (_current_command->get_state() != progress_code::OK) {
        report_command_change(rq, modules::response_code::REJECTED);
        return;
    }

    if (rq.code == modules::request_code::LOAD) {
        _current_command = &_load_filament_command;
    } else if (rq.code == modules::request_code::EJECT) {
        _current_command = &_eject_filament_command;
    } else if (rq.code == modules::request_code::TOOL) {
        _current_command = &_tool_change_command;
    } else if (rq.code == modules::request_code::UNLOAD) {
        _current_command = &_unload_from_nozzle_command;
    } else {
        _current_command = &_no_command;
    }

    _current_command_request = rq;
    bool accepted = _current_command->reset(rq.first_argument);
    report_command_change(rq, accepted ? modules::response_code::ACCEPTED : modules::response_code::REJECTED);
}

modules::current_command_state app::current_command_state() const
{
    switch (_current_command->get_error()) {
    case error_code::RUNNING:
        return modules::current_command_state(modules::response_code::PROCESSING, (uint16_t)_current_command->get_state());
    case error_code::OK:
        return modules::current_command_state(modules::response_code::FINISHED, 0);
    default:
        return modules::current_command_state(modules::response_code::ERROR, (uint16_t)_current_command->get_error());
    }
}
