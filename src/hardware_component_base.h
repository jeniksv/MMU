#pragma once

/**
 * @class hardware_component_base
 * @brief Base class for hardware components.
 *
 * This class serves as a base class for all hardware components. It provides
 * a default constructor and deletes the copy constructor and copy assignment operator
 * to prevent copying of hardware components.
 */
class hardware_component_base {
public:
    /**
     * @brief Default constructor for the hardware_component_base class.
     */
    hardware_component_base() = default;

    /**
     * @brief Deleted copy constructor.
     *
     * Prevents copying of hardware components.
     */
    hardware_component_base(const hardware_component_base&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     *
     * Prevents copying of hardware components.
     *
     * @return Reference to this object.
     */
    hardware_component_base& operator=(const hardware_component_base&) = delete;
};
