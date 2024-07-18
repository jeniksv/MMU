/// @file _protocol.h
#pragma once

#include "crc.h"
#include <cctype>
#include <cstdint>
#include <stdint.h>

namespace modules {

class protocol;

/**
 * @enum request_code
 * @brief Enumerates the types of requests that can be sent to the system.
 *
 * Each request code corresponds to a specific type of operation or query within the system,
 * allowing the recipient to determine the expected structure and necessary response.
 */
enum class request_code {
    UNKNOWN = 'X',
    QUERY = 'Q',
    TOOL = 'T',
    LOAD = 'L',
    UNLOAD = 'U',
    FEEDING_SYSTEM_FILAMENT_SENSOR = 'P',
    VERSION = 'S',
    EJECT = 'E',
    WRITE = 'W',
    EXTRUDER_FILAMENT_SENSOR = 'f',
    READ = 'R'
};

/**
 * @enum response_code
 * @brief Enumerates possible response statuses to requests.
 *
 * These codes are used to communicate the outcome or status of a processed request.
 */
enum class response_code {
    UNKNOWN = 'X',
    PROCESSING = 'P',
    ERROR = 'E',
    FINISHED = 'F',
    ACCEPTED = 'A',
    REJECTED = 'R',
};

/**
 * @enum request_state
 * @brief Indicates the processing state of a request.
 */
enum class request_state {
    MESSAGE_COMPLETED,
    NEED_MORE_DATA,
    ERROR,
};

/**
 * @class request
 * @brief Encapsulates a request in the communication protocol, including arguments and CRC calculation.
 *
 * This class holds details for a single request, including the type of request, arguments,
 * and an automatically calculated CRC for error detection.
 */
class request {
public:
    request(request_code code, uint8_t first_argument)
        : code(code)
        , first_argument(first_argument)
        , second_argument(0)
        , crc8(compute_crc8())
    {
    }

    request(request_code code)
        : code(code)
        , first_argument(0)
        , second_argument(0)
        , crc8()
    {
    }

    uint8_t compute_crc8() const;

    uint8_t get_crc() const;

    request_code code;
    uint8_t first_argument;
    uint16_t second_argument;

    uint8_t crc8;
};

/**
 * @class response
 * @brief Encapsulates a response to a request within the protocol.
 *
 * This class holds the details of a response related to a request, including the response code and any values associated with it.
 * It also handles the computation of a CRC8 checksum for the response to ensure data integrity.
 */
class response {
public:
    response(request _request, response_code code, uint16_t value)
        : _request(_request)
        , code(code)
        , value(value)
    {
        this->_request.crc8 = compute_crc8();
    }

    uint8_t compute_crc8() const;

    uint8_t get_crc() const;

    request _request;
    response_code code;
    uint16_t value;
};

/**
 * @class current_command_state
 * @brief Represents the current state of a command being processed.
 */
class current_command_state {
public:
    response_code code;
    uint16_t value;

    current_command_state(response_code code, uint16_t value)
        : code(code)
        , value(value)
    {
    }
};

/**
 * @class request_automaton_state
 * @brief Base class for all request processing states.
 *
 * This class defines a common interface for all state classes that handle different parts of the request parsing process.
 * Each state is responsible for parsing a portion of the request data and transitioning to the next appropriate state.
 */
class request_automaton_state {
public:
    /**
     * Construct a state object with a reference to the protocol.
     * @param p Reference to the protocol handling the request.
     */
    request_automaton_state(protocol& p)
        : _protocol(p)
    {
    }

    /**
     * Virtual destructor to ensure proper cleanup of derived classes.
     */
    virtual ~request_automaton_state() = default;

    /**
     * Parse an incoming byte and determine the next state of the request processing.
     * @param c The byte to parse.
     * @return The next state of the request processing.
     */
    virtual request_state parse_byte(uint8_t c) = 0;

protected:
    /**
     * Convert a character to its hexadecimal value.
     * @param c The character to convert.
     * @return The hexadecimal equivalent of the character.
     */
    uint8_t char_to_hex(uint8_t c);

    protocol& _protocol;
};

/**
 * @class request_code_state
 * @brief Handles the state of parsing the request code byte.
 */
class request_code_state : public request_automaton_state {
public:
    request_code_state(protocol& p)
        : request_automaton_state(p)
    {
    }

    virtual request_state parse_byte(uint8_t c) override;

private:
    void set_response_strategy(uint8_t c) const;

    bool is_valid_code(uint8_t c) const;

    bool code_with_more_arguments(uint8_t c) const;
};

/**
 * @class request_code_state
 * @brief Handles the state of parsing the request code byte.
 */
class request_address_state : public request_automaton_state {
public:
    request_address_state(protocol& p)
        : request_automaton_state(p)
    {
    }
    virtual request_state parse_byte(uint8_t c) override;
};

/**
 * @class request_code_state
 * @brief Handles the state of parsing the request code byte.
 */
class request_value_state : public request_automaton_state {
public:
    request_value_state(protocol& p)
        : request_automaton_state(p)
    {
    }
    virtual request_state parse_byte(uint8_t c) override;
};

/**
 * @class request_code_state
 * @brief Handles the state of parsing the request code byte.
 */
class request_write_value_state : public request_automaton_state {
public:
    request_write_value_state(protocol& p)
        : request_automaton_state(p)
    {
    }
    virtual request_state parse_byte(uint8_t c) override;
};

/**
 * @class request_code_state
 * @brief Handles the state of parsing the request code byte.
 */
class request_crc_state : public request_automaton_state {
public:
    request_crc_state(protocol& p)
        : request_automaton_state(p)
    {
    }
    virtual request_state parse_byte(uint8_t c) override;
};

/**
 * @class request_code_state
 * @brief Handles the state of parsing the request code byte.
 */
class request_error_state : public request_automaton_state {
public:
    request_error_state(protocol& p)
        : request_automaton_state(p)
    {
    }

    virtual request_state parse_byte(uint8_t c) override;
};

/**
 * @class response_parameters
 * @brief Encapsulates all necessary parameters needed for a protocol response.
 *
 * This class stores the details required to formulate a response in the communication protocol,
 * including the original request, response code, and any additional information like sensor values or command state.
 */
class response_parameters {
public:
    /**
     * @class builder
     * @brief Builder class for creating instances of response_parameters.
     *
     * This builder class provides a fluent interface for setting various parameters of a response_parameters instance
     * and finally building it. It encapsulates all the necessary steps in creating a fully initialized response_parameters object.
     */
    class builder {
    public:
        builder(const request& msg, uint8_t* txbuff)
            : message(msg)
            , response_buffer(txbuff)
        {
        }

        builder& with_response_code(response_code r)
        {
            response_result = r;
            return *this;
        }

        builder& with_fsensor_value(uint8_t fsensorValue)
        {
            fsensor_value = fsensorValue;
            return *this;
        }

        builder& is_accepted(bool accepted)
        {
            _accepted = accepted;
            return *this;
        }

        builder& with_second_arg(uint16_t second_argument)
        {
            second_arg = second_argument;
            return *this;
        }

        builder& with_command_state(current_command_state rcs)
        {
            current_state = rcs;
            return *this;
        }
        /**
         * Finalizes the construction of the response_parameters object and returns it.
         * @return A fully constructed response_parameters instance.
         */
        response_parameters build()
        {
            return response_parameters(message, response_buffer, response_result, fsensor_value, _accepted, second_arg, current_state);
        }

    private:
        const request& message;
        uint8_t* response_buffer;
        response_code response_result = response_code::UNKNOWN;
        uint8_t fsensor_value = 0;
        bool _accepted = false;
        uint16_t second_arg = 0;
        current_command_state current_state = current_command_state(response_code::UNKNOWN, 0);
    };

    /**
     * Static factory method to start the building process of a response_parameters object.
     * @param msg Reference to the original request object.
     * @param txbuff Pointer to the buffer where the response will be written.
     * @return A builder instance to configure the response_parameters object.
     */
    static builder create(const request& msg, uint8_t* txbuff)
    {
        return builder(msg, txbuff);
    }

    /**
     * Constructs a response_parameters instance with detailed specification.
     * @param msg Reference to the original request object.
     * @param txbuff Pointer to the buffer where the response will be stored.
     * @param ar The response code indicating the outcome of the request.
     * @param fsensorValue The filament sensor's value related to the response.
     * @param accepted Boolean indicating whether the request was accepted.
     * @param second_argument Additional numeric argument that may be required depending on the request.
     * @param rcs The current state of the command associated with the response.
     */
    response_parameters(const request& msg, uint8_t* txbuff, response_code ar, uint8_t fsensorValue, bool accepted, uint16_t second_argument, current_command_state rcs)
        : message(msg)
        , response_buffer(txbuff)
        , response_result(ar)
        , fsensor_value(fsensorValue)
        , accepted(accepted)
        , second_arg(second_argument)
        , current_state(rcs)
    {
    }

    const request& message;
    uint8_t* response_buffer;
    response_code response_result;
    uint8_t fsensor_value;
    bool accepted;
    uint16_t second_arg;
    current_command_state current_state;
};

/**
 * @class response_strategy_base
 * @brief Abstract base class for response encoding strategies.
 *
 * This class provides the interface and common utilities for encoding responses,
 * which can be specialized in derived classes according to specific protocol needs.
 */
class response_strategy_base {
public:
    response_strategy_base() = default;

    /**
     * Pure virtual function to encode a response based on the given parameters.
     * @param params The response parameters containing data to be encoded.
     * @return Number of bytes written to the response buffer.
     */
    virtual uint8_t encode_response(const response_parameters& params) = 0;

protected:
    /**
     * Template function to convert an unsigned integer value to its hexadecimal string representation.
     * @param value The value to convert.
     * @param dst Pointer to the buffer where the hexadecimal string will be stored.
     * @return The number of characters written to the buffer.
     */
    template <typename T>
    uint8_t uint_to_hex(T value, uint8_t* dst)
    {
        if (value == 0) {
            *dst = '0';
            return 1;
        }

        uint8_t chars_out = 0;
        bool leading_nibble_found = false;

        for (int8_t i = sizeof(T) * 2 - 1; i >= 0; --i) {
            uint8_t nibble = (value >> (i * 4)) & 0xF;
            if (leading_nibble_found || nibble > 0) {
                dst[chars_out++] = hex_to_char(nibble);
                leading_nibble_found = true;
            }
        }

        if (chars_out == 0) {
            dst[chars_out++] = '0';
        }

        return chars_out;
    }

    /**
     * Encodes the request code from a request message into a destination buffer.
     * @param msg The request object containing the code to encode.
     * @param dst Pointer to the buffer where the encoded code will be written.
     * @return The number of bytes written to the buffer.
     */
    uint8_t encode_request_code(const request& msg, uint8_t* dst);

    /**
     * Appends a CRC value to a destination buffer.
     * @param crc The CRC value to append.
     * @param dst Pointer to the buffer where the CRC will be written.
     * @return The number of bytes written to the buffer.
     */
    uint8_t append_crc(uint8_t crc, uint8_t* dst);

    /**
     * Converts a nibble (4 bits) into its hexadecimal character representation.
     * @param n The nibble to convert.
     * @return The hexadecimal character.
     */
    uint8_t hex_to_char(uint8_t n);
};

/**
 * @class response_generic_strategy
 * @brief Strategy for encoding generic responses like Command ACCEPTED or REJECTED.
 *
 * Implements the generic response encoding logic that applies to basic acceptance or rejection of commands.
 */
class response_generic_strategy : public response_strategy_base {
public:
    response_generic_strategy() = default;

    virtual uint8_t encode_response(const response_parameters& params) override;
};

/**
 * @class response_query_strategy
 * @brief Strategy for encoding responses to QUERY operation status requests.
 *
 * Implements the encoding logic for responses specific to QUERY operations, providing status or other information.
 */
class response_query_strategy : public response_strategy_base {
public:
    response_query_strategy() = default;

    virtual uint8_t encode_response(const response_parameters& params) override;
};

/**
 * @class response_read_strategy
 * @brief Strategy for encoding responses to READ queries.
 *
 * Implements the encoding logic for responses to READ requests, typically involving returning data or state information.
 */
class response_read_strategy : public response_strategy_base {
public:
    response_read_strategy() = default;

    virtual uint8_t encode_response(const response_parameters& params) override;
};

/**
 * @class protocol
 * @brief Manages parsing requests and encoding responses in a communication protocol.
 *
 * The protocol class is the central manager for all operations related to protocol communication.
 * It handles byte-by-byte parsing of incoming data, maintains the state of the current request,
 * and utilizes various strategies to encode responses.
 */
class protocol {
public:
    protocol()
        : _rq_code(*this)
        , _rq_value(*this)
        , _rq_address(*this)
        , _rq_write_value(*this)
        , _rq_crc(*this)
        , _rq_error(*this)
        , _rq_state(&_rq_code)
        , _request(request_code::UNKNOWN)
        , _res_generic()
        , _res_query()
        , _res_read()
        , _encode_strategy()
    {
    }

    /**
     * Parses a single byte and updates the protocol state accordingly.
     * @param c The byte to parse.
     * @return The current state of the request parsing after processing the byte.
     */
    request_state parse_byte(uint8_t c);

    /**
     * Retrieves the most recently fully parsed request.
     * @return The last fully parsed request object.
     */
    const request get_request() const;

    /**
     * Changes the current state of request parsing.
     * @param state Reference to the new state to transition to.
     */
    void change_request_state(request_automaton_state& state);

    /**
     * Resets the internal state of the request decoder, typically used after encountering an error.
     */
    void reset_request_decoder();

    /**
     * Encodes a response based on the provided response parameters.
     * @param params The parameters containing the details needed for encoding the response.
     * @return The number of bytes written to the response buffer.
     */
    uint8_t encode_response(const response_parameters& params);

private:
    friend class request_code_state;
    friend class request_value_state;
    friend class request_address_state;
    friend class request_write_value_state;
    friend class request_crc_state;
    friend class request_error_state;

    request_code_state _rq_code;
    request_value_state _rq_value;
    request_address_state _rq_address;
    request_write_value_state _rq_write_value;
    request_crc_state _rq_crc;
    request_error_state _rq_error;
    request_automaton_state* _rq_state;

    request _request;

    response_generic_strategy _res_generic;
    response_query_strategy _res_query;
    response_read_strategy _res_read;
    response_strategy_base* _encode_strategy;
};

} // namespace modules
