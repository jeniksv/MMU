#pragma once
#include <stdint.h>

/**
 * @enum progress_code
 * @brief Enum class that encapsulates various progress and error codes.
 *
 * Progress codes are used throughout the printer firmware to indicate the current
 * state or outcome of printer operations. They include both operational statuses
 * and error states. Each code is mapped to a specific printer operation or error
 * scenario for easy tracking and debugging.
 */
enum class progress_code {
    OK = 0,

    ENGAGING_IDLER,
    DISENGAGING_IDLER,
    UNLOADING_TO_FINDA,
    UNLOADING_TO_FEEDING_SYSTEM,
    FEEDING_TO_FINDA,
    FEEDING_TO_BONDTECH,
    FEEDING_TO_NOZZLE,
    AVOIDING_GRIND,
    FINISHING_MOVES,

    ERRDISENGAGING_IDLER,
    ERRENGAGING_IDLER,
    ERR_WAITING_FOR_USER,
    ERR_INTERNAL,
    ERR_HELPING_FILAMENT,
    ERR_TMC_FAILED,

    UNLOADING_FILAMENT,
    LOADING_FILAMENT,
    SELECTING_FILAMENT_SLOT,
    PREPARING_BLADE,
    PUSHING_FILAMENT,
    PERFORMING_CUT,
    RETURNING_SELECTOR,
    PARKING_SELECTOR,
    EJECTING_FILAMENT,
    RETRACTING_FROM_FINDA,

    HOMING,
    MOVING_SELECTOR,

    FEEDING_TO_F_SENSOR,

    HW_TEST_BEGIN,
    HW_TEST_IDLER,
    HW_TEST_SELECTOR,
    HW_TEST_FEEDING_SYSTEM,
    HW_TEST_CLEANUP,
    HW_TEST_EXEC,
    HW_TEST_DISPLAY,
    ERR_HW_TEST_FAILED,

    EMPTY = 0xFF
};
