#pragma once
#include <stdint.h>

namespace config {

#ifdef UNITTEST
constexpr uint8_t TOOL_COUNT = 4;
constexpr uint16_t FEEDING_SYSTEM_STEP_TIMEOUT = 0;
#else
constexpr uint8_t TOOL_COUNT = 2;
constexpr uint16_t FEEDING_SYSTEM_STEP_TIMEOUT = 100;
#endif

constexpr uint8_t REGISTERS_SIZE = 40;

constexpr uint8_t PROJECT_MAJOR = 3;
constexpr uint8_t PROJECT_MINOR = 0;
constexpr uint8_t PROJECT_REVISION = 3;
constexpr uint8_t PROJECT_BUILD_NUMBER = 0;

constexpr uint8_t MAX_RESPONSE_SIZE = 14;

constexpr uint16_t FILAMENT_SENSOR_LOWER_BOUND = 0;
constexpr uint16_t FILAMENT_SENSOR_UPPER_BOUND = 30;

constexpr uint16_t HUB_FSENSOR_LOWER_BOUND = 30;
constexpr uint16_t HUB_FSENSOR_UPPER_BOUND = 85;

constexpr uint16_t MOTOR_SENSOR_LOWER_BOUND = 55;
constexpr uint16_t MOTOR_SENSOR_UPPER_BOUND = 150;

constexpr float MOTOR_PWM_SLOW = 30.0;
constexpr float MOTOR_PWM_MEDIUM = 50.0;
constexpr float MOTOR_PWM_FAST = 70.0;

constexpr uint16_t EXTRUDER_HELP_ADDITIONAL_STEPS = 12;

constexpr uint16_t HUB_HELP_ADDITIONAL_STEPS = 250;

constexpr uint16_t UART_BUFFER_SIZE = 128;

};
