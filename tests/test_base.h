#pragma once

#include "dc_motor.h"
#include "feeding_system.h"
#include "pins.h"
#include "sensor.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class TestBase : public testing::Test {
protected:
    HAL::pwm_pin aPwmPin {};
    HAL::gpio_pin aIN1Pin {};
    HAL::gpio_pin aIN2Pin {};

    HAL::pwm_pin bPwmPin {};
    HAL::gpio_pin bIN1Pin {};
    HAL::gpio_pin bIN2Pin {};

    HAL::pwm_pin cPwmPin {};
    HAL::gpio_pin cIN1Pin {};
    HAL::gpio_pin cIN2Pin {};

    HAL::pwm_pin dPwmPin {};
    HAL::gpio_pin dIN1Pin {};
    HAL::gpio_pin dIN2Pin {};

    HAL::gpio_pin stby1Pin {};
    HAL::gpio_pin stby2Pin {};

    HAL::adc_pin aMotorAdcPin {};
    HAL::adc_pin bMotorAdcPin {};
    HAL::adc_pin cMotorAdcPin {};
    HAL::adc_pin dMotorAdcPin {};

    HAL::adc_pin aFSensorPin {};
    HAL::adc_pin bFSensorPin {};
    HAL::adc_pin cFSensorPin {};
    HAL::adc_pin dFSensorPin {};

    HAL::adc_pin hubFSensorPin {};

    modules::dc_motor aMotor {
        aPwmPin,
        stby1Pin,
        aIN1Pin,
        aIN2Pin
    };

    modules::dc_motor bMotor {
        bPwmPin,
        stby1Pin,
        bIN1Pin,
        bIN2Pin
    };

    modules::dc_motor cMotor {
        cPwmPin,
        stby2Pin,
        cIN1Pin,
        cIN2Pin
    };

    modules::dc_motor dMotor {
        dPwmPin,
        stby2Pin,
        dIN1Pin,
        dIN2Pin
    };

    modules::dc_motor motors[config::TOOL_COUNT] = {
        aMotor,
        bMotor,
        cMotor,
        dMotor
    };

    modules::MotorHalSensor aMotorSensor { aMotorAdcPin };
    modules::MotorHalSensor bMotorSensor { bMotorAdcPin };
    modules::MotorHalSensor cMotorSensor { aMotorAdcPin };
    modules::MotorHalSensor dMotorSensor { bMotorAdcPin };

    modules::feeding_system_filament_sensor aFeedingSystemFSensor { bMotorAdcPin };
    modules::feeding_system_filament_sensor bFeedingSystemFSensor { bMotorAdcPin };
    modules::feeding_system_filament_sensor cFeedingSystemFSensor { bMotorAdcPin };
    modules::feeding_system_filament_sensor dFeedingSystemFSensor { bMotorAdcPin };

    modules::feeding_system_filament_sensor pulleyFSensors[config::TOOL_COUNT] = {
        aFeedingSystemFSensor,
        bFeedingSystemFSensor,
        cFeedingSystemFSensor,
        dFeedingSystemFSensor
    };

    modules::hub_filament_sensor hubFSensor { bMotorAdcPin };

    modules::feeding_system afeeding_system { aMotor, aFeedingSystemFSensor, aMotorSensor };
    modules::feeding_system bfeeding_system { bMotor, bFeedingSystemFSensor, bMotorSensor };
    modules::feeding_system cfeeding_system { cMotor, cFeedingSystemFSensor, cMotorSensor };
    modules::feeding_system dfeeding_system { dMotor, dFeedingSystemFSensor, dMotorSensor };

    modules::feeding_system _feeding_systems[config::TOOL_COUNT] = {
        afeeding_system,
        bfeeding_system,
        cfeeding_system,
        dfeeding_system
    };
};
