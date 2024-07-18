#pragma once

#include "config.h"
#include "dc_motor.h"
#include "hal_time.h"
#include "sensor.h"

namespace modules {

/**
 * @enum filament_position
 * @brief Represents the various positions of the filament within the feeding system.
 */
enum class filament_position {
    NOT_LOADED, ///< Filament is not loaded.
    IN_FEEDING_SYSTEM, ///< Filament is in the pulley.
    BEFORE_FILAMENT_HUB, ///< Filament is before the filament hub.
    IN_FILAMENT_HUB, ///< Filament is in the filament hub.
    IN_BONDTECH, ///< Filament is in the Bondtech extruder.
    IN_NOZZLE ///< Filament is in the nozzle.
};

/**
 * @class feeding_system
 * @brief Manages the feeding of filament in a 3D printer.
 *
 * This class controls the DC motor, filament sensor, and motor sensor to manage the
 * feeding of filament through the system.
 */
class feeding_system {
public:
    /**
     * @brief Constructor for the feeding_system class.
     *
     * @param motor Reference to the DC motor.
     * @param _filament_sensor Reference to the filament sensor.
     * @param _motor_sensor Reference to the motor sensor.
     */
    feeding_system(modules::dc_motor& motor, modules::feeding_system_filament_sensor& _filament_sensor, modules::MotorHalSensor& _motor_sensor);

    /**
     * @brief Checks if filament is present in the feeding system.
     *
     * @return True if filament is present, false otherwise.
     */
    bool has_filament();

    /**
     * @brief Moves the filament in the specified direction with a given PWM percentage.
     *
     * @param direction The direction to move the filament.
     * @param pwmPercent The PWM percentage to control the motor speed.
     */
    void step(modules::motor_direction direction, float pwmPercent = config::MOTOR_PWM_MEDIUM);

    /**
     * @brief Stops the feeding system immediately.
     *
     * This method is used in emergency situations to stop the motor.
     */
    void panic();

    /**
     * @brief Sets the current position of the filament.
     *
     * @param position The position of the filament.
     */
    void set_filament_position(filament_position position);

    /**
     * @brief Gets the current position of the filament.
     *
     * @return The current position of the filament.
     */
    filament_position get_filament_position() const;

private:
    modules::dc_motor& _motor;
    modules::feeding_system_filament_sensor& _filament_sensor;
    modules::MotorHalSensor& _motor_sensor;
    filament_position _current_filament_position;
};

} // namespace modules
