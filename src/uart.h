#pragma once

#include "config.h"
#include "fifo.h"
#include "hardware_component_base.h"
#include <stdint.h>

namespace HAL {

/**
 * @class UART
 * @brief A class representing a UART hardware component.
 *
 * This class provides methods for UART _communication, including reading and writing data,
 * as well as handling interrupts.
 */
class UART : public hardware_component_base {
public:
    /**
     * @brief Constructor for the UART class.
     *
     * Initializes the UART component.
     */
    UART();

    /**
     * @brief reads a byte from the UART receive buffer.
     *
     * @return The byte read from the receive buffer.
     */
    uint8_t read_byte();

    /**
     * @brief Checks if data is available in the UART receive buffer.
     *
     * @return True if data is available, false otherwise.
     */
    bool is_available();

    /**
     * @brief Writes data to the UART transmit buffer.
     *
     * @param data Pointer to the data to be written.
     * @param l Length of the data to be written.
     */
    void write(const uint8_t* data, uint8_t l);

    /**
     * @brief Handles UART receive interrupts.
     *
     * @param data The byte received from the UART.
     */
    void on_rx_interrupt(uint8_t data);

    /**
     * @brief Handles UART transmit interrupts.
     *
     * @return The next byte to be transmitted.
     */
    uint8_t on_tx_interrupt();

private:
    fifo<config::UART_BUFFER_SIZE> _rx_fifo;
    fifo<config::UART_BUFFER_SIZE> _tx_fifo;
};

} // namespace HAL
