#pragma once

#include "config.h"
#include <stdint.h>

namespace modules {

/**
 * @enum register_address
 * @brief Addresses of various registers.
 *
 * This enum defines the addresses of different registers used in the system.
 */
enum class register_address {
    PROJECT_MAJOR = 0x00,
    PROJECT_MINOR = 0x01,
    PROJECT_REVISION = 0x02,
    PROJECT_BUILD_NUMBER = 0x03,
    MMU_ERRORS = 0x04,
    CURRENT_PROGRESS_CODE = 0x05,
    CURRENT_ERROR_CODE = 0x06,
    FILAMENT_STATE = 0x07,
    FEEDING_SYSTEM_FSENSOR_STATE = 0x08,
    EXTRUDER_FSENSOR_STATE = 0x09,
    EXTRA_LOAD_DISTANCE = 0x0b,
    FSENSOR_UNLOAD_CHECK_DIST = 0x0c,
    CURRENT_SLOT = 0x1b,
    RESERVED_FOR_INTERNAL_USE = 0x21,
    BOWDEN_LENGTH = 0x22,
};

/**
 * @class register_array
 * @brief A class representing an array of registers.
 *
 * This class provides methods to read from and write to an array of registers.
 * The size of the array is determined by the template parameter.
 *
 * @tparam SIZE The size of the register array.
 */
template <uint8_t SIZE>
class register_array {
public:
    /**
     * @brief Constructor for the register_array class.
     *
     * Initializes the register array and sets version numbers.
     */
    register_array();

    /**
     * @brief Reads the value from a specified register address.
     *
     * @param address The address of the register to read.
     * @return The value stored in the specified register.
     */
    uint16_t read(register_address address);

    /**
     * @brief Writes a value to a specified register address.
     *
     * @param address The address of the register to write.
     * @param value The value to write to the register.
     */
    void write(register_address address, uint16_t value);

private:
    /**
     * @brief Writes the version numbers to the appropriate registers.
     *
     * This function initializes the version-related registers with their respective values.
     */
    void write_version_numbers();

    uint16_t _registers[SIZE];
};

/**
 * @typedef registers
 * @brief Alias for register_array with size specified by config::REGISTERS_SIZE.
 */
using registers = register_array<config::REGISTERS_SIZE>;

} // namespace modules
