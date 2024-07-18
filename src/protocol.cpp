/// @file _protocol.cpp
#include "protocol.h"

namespace modules {

uint8_t request_automaton_state::char_to_hex(uint8_t c)
{
    return ('0' <= c && c <= '9') ? c - '0' : c - 'a' + 10;
}

uint8_t response_strategy_base::hex_to_char(uint8_t n)
{
    return n < 10 ? n + '0' : n - 10 + 'a';
}

bool request_code_state::is_valid_code(uint8_t c) const
{
    return c == 'Q'
        || c == 'T'
        || c == 'L'
        || c == 'M'
        || c == 'U'
        || c == 'X'
        || c == 'P'
        || c == 'S'
        || c == 'E'
        || c == 'W'
        || c == 'F'
        || c == 'f'
        || c == 'H'
        || c == 'R';
}

bool request_code_state::code_with_more_arguments(uint8_t c) const
{
    return c == 'W';
}

void request_code_state::set_response_strategy(uint8_t c) const
{
    if (c == 'Q') {
        _protocol._encode_strategy = &_protocol._res_query;
    } else if (c == 'R' || c == 'S') {
        _protocol._encode_strategy = &_protocol._res_read;
    } else {
        _protocol._encode_strategy = &_protocol._res_generic;
    }
}

request_state request_code_state::parse_byte(uint8_t c)
{
    set_response_strategy(c);

    if (is_valid_code(c)) {
        _protocol._request = request(static_cast<request_code>(c));

        if (code_with_more_arguments(c)) {
            _protocol.change_request_state(_protocol._rq_address);
        } else {
            _protocol.change_request_state(_protocol._rq_value);
        }

        return request_state::NEED_MORE_DATA;
    }

    _protocol._request.code = request_code::UNKNOWN;
    _protocol.change_request_state(_protocol._rq_error);
    return request_state::ERROR;
}

request_state request_value_state::parse_byte(uint8_t c)
{
    if (std::isxdigit(c)) {
        _protocol._request.first_argument <<= 4U;
        _protocol._request.first_argument |= char_to_hex(c);
        return request_state::NEED_MORE_DATA;
    } else if (c == '*') {
        _protocol.change_request_state(_protocol._rq_crc);
        return request_state::NEED_MORE_DATA;
    }

    _protocol._request.code = request_code::UNKNOWN;
    _protocol.change_request_state(_protocol._rq_error);
    return request_state::ERROR;
}

request_state request_address_state::parse_byte(uint8_t c)
{
    if (std::isxdigit(c)) {
        _protocol._request.first_argument <<= 4U;
        _protocol._request.first_argument |= char_to_hex(c);
        return request_state::NEED_MORE_DATA;
    } else if (c == ' ') { // end of address, value coming
        _protocol.change_request_state(_protocol._rq_write_value);
        return request_state::NEED_MORE_DATA;
    }

    _protocol._request.code = request_code::UNKNOWN;
    _protocol.change_request_state(_protocol._rq_error);
    return request_state::ERROR;
}

request_state request_write_value_state::parse_byte(uint8_t c)
{
    if (std::isxdigit(c)) {
        _protocol._request.second_argument <<= 4U;
        _protocol._request.second_argument |= char_to_hex(c);
        return request_state::NEED_MORE_DATA;
    } else if (c == '*') {
        _protocol.change_request_state(_protocol._rq_crc);
        return request_state::NEED_MORE_DATA;
    }

    _protocol._request.code = request_code::UNKNOWN;
    _protocol.change_request_state(_protocol._rq_error);
    return request_state::ERROR;
}

request_state request_crc_state::parse_byte(uint8_t c)
{
    if (std::isxdigit(c)) {
        _protocol._request.crc8 <<= 4U;
        _protocol._request.crc8 |= char_to_hex(c);
        return request_state::NEED_MORE_DATA;
    } else if (c == '\n') {
        // check get_crc at this spot
        if (_protocol._request.crc8 != _protocol._request.compute_crc8()) {
            // get_crc mismatch
            _protocol._request.code = request_code::UNKNOWN;
            _protocol.change_request_state(_protocol._rq_error);
            return request_state::ERROR;
        }

        _protocol.change_request_state(_protocol._rq_code);
        return request_state::MESSAGE_COMPLETED;
    }

    // TODO error handler function
    // return error_handler();
    _protocol._request.code = request_code::UNKNOWN;
    _protocol.change_request_state(_protocol._rq_error);
    return request_state::ERROR;
}

request_state request_error_state::parse_byte(uint8_t c)
{
    if (c == '\n') {
        _protocol.change_request_state(_protocol._rq_code);
        return request_state::MESSAGE_COMPLETED;
    }

    _protocol._request.code = request_code::UNKNOWN;
    _protocol.change_request_state(_protocol._rq_error);
    return request_state::ERROR;
}

uint8_t request::get_crc() const
{
    return crc8;
}

uint8_t request::compute_crc8() const
{
    uint8_t crc = 0;
    crc = modules::update_crc<uint8_t>(0, static_cast<uint8_t>(code));
    crc = modules::update_crc<uint8_t>(crc, first_argument);
    crc = modules::update_crc<uint16_t>(crc, second_argument);
    return crc;
}

uint8_t response::get_crc() const
{
    return _request.crc8;
}

uint8_t response::compute_crc8() const
{
    uint8_t crc = _request.compute_crc8();
    crc = modules::update_crc<uint8_t>(crc, static_cast<uint8_t>(code));
    crc = modules::update_crc<uint16_t>(crc, value);
    return crc;
}

uint8_t protocol::encode_response(const response_parameters& params)
{
    return _encode_strategy->encode_response(params);
}

request_state protocol::parse_byte(uint8_t c)
{
    return _rq_state->parse_byte(c);
}

void protocol::change_request_state(request_automaton_state& state)
{
    _rq_state = &state;
}

void protocol::reset_request_decoder()
{
    _rq_state = &_rq_code;
}

const request protocol::get_request() const
{
    return _request;
}

uint8_t response_generic_strategy::encode_response(const response_parameters& params)
{
    response rsp(request(params.message.code, params.message.first_argument), params.response_result, 0);
    uint8_t index = encode_request_code(rsp._request, params.response_buffer);
    params.response_buffer[index] = static_cast<uint8_t>(params.response_result);
    index += 1;
    index += append_crc(rsp.get_crc(), params.response_buffer + index);
    params.response_buffer[index] = '\n';
    index += 1;
    return index;
}

uint8_t response_query_strategy::encode_response(const response_parameters& params)
{
    response rsp(params.message, params.current_state.code, params.current_state.value);
    uint8_t index = encode_request_code(params.message, params.response_buffer);

    params.response_buffer[index++] = static_cast<uint8_t>(rsp.code);

    index += uint_to_hex<uint16_t>(rsp.value, params.response_buffer + index);
    index += append_crc(rsp.get_crc(), params.response_buffer + index);

    params.response_buffer[index++] = '\n';
    return index;
}

uint8_t response_read_strategy::encode_response(const response_parameters& params)
{
    response rsp(
        params.message,
        params.accepted ? response_code::ACCEPTED : response_code::REJECTED,
        params.accepted ? params.second_arg : 0);

    uint8_t index = encode_request_code(params.message, params.response_buffer);
    params.response_buffer[index] = static_cast<uint8_t>(rsp.code);
    index += 1;

    index += params.accepted ? uint_to_hex<uint16_t>(params.second_arg, params.response_buffer + index) : 0;
    index += append_crc(rsp.get_crc(), params.response_buffer + index);
    params.response_buffer[index] = '\n';
    return index + 1;
}

uint8_t response_strategy_base::encode_request_code(const request& msg, uint8_t* dst)
{
    dst[0] = static_cast<uint8_t>(msg.code);

    uint8_t index = 1 + uint_to_hex<uint8_t>(msg.first_argument, dst + 1);

    dst[index] = ' ';
    return index + 1;
}

uint8_t response_strategy_base::append_crc(uint8_t crc, uint8_t* dst)
{
    dst[0] = '*';
    return 1 + uint_to_hex<uint8_t>(crc, dst + 1);
}

} // namespace modules
