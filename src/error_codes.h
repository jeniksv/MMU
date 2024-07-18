#pragma once
#include <stdint.h>

/**
 * @enum error_code
 * @brief Enum class for defining error and operational status codes.
 *
 * The error codes are designed to be interpreted as negative values (high bit set) to
 * distinguish them from normal operational status codes, which are positive. This design
 * helps in quickly identifying error states in condition checks and log outputs.
 */
enum class error_code {
    RUNNING = 0x0000,
    OK = 0x0001,

    FSENSOR_DIDNT_SWITCH_ON = 0x8001,
    FSENSOR_DIDNT_SWITCH_OFF = 0x8002,

    EXTRUDER_FSENSOR_DIDNT_SWITCH_ON = 0x8003,
    EXTRUDER_FSENSOR_DIDNT_SWITCH_OFF = 0x8004,

    FILAMENT_ALREADY_LOADED = 0x8005,

    INVALID_TOOL = 0x8006,

    MOVE_FAILED = 0x800b,

    FILAMENT_EJECTED = 0x800c,

    LOAD_TO_EXTRUDER_FAILED = 0x802a,
    VERSION_MISMATCH = 0x802c,
    PROTOCOL_ERROR = 0x802d,
    MMU_NOT_RESPONDING = 0x802e,
};
