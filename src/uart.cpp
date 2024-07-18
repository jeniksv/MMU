#include "uart.h"

namespace HAL {

UART::UART()
{
}

uint8_t UART::read_byte()
{
    uint8_t value;
    _rx_fifo.pop(value);
    return value;
}

bool UART::is_available()
{
    return !_rx_fifo.is_empty();
}

void UART::write(const uint8_t* data, uint8_t l)
{
    for (uint16_t i = 0; i < l; ++i) {
        if (_tx_fifo.is_full()) {
            break;
        }
        _tx_fifo.push(data[i]);
    }
}

void UART::on_rx_interrupt(uint8_t byteReceived)
{
    if (!_rx_fifo.is_full()) {
        _rx_fifo.push(byteReceived);
    }
}

uint8_t UART::on_tx_interrupt()
{
    if (!_tx_fifo.is_empty()) {
        uint8_t value;
        _tx_fifo.pop(value);
        return value;
    }
    return 0;
}

}
