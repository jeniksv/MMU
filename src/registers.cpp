#include "registers.h"

namespace modules {

template <uint8_t Size>
register_array<Size>::register_array()
{
    write_version_numbers();
}

template <uint8_t Size>
uint16_t register_array<Size>::read(register_address address)
{
    return _registers[static_cast<uint8_t>(address)];
}

template <uint8_t Size>
void register_array<Size>::write(register_address address, uint16_t value)
{
    _registers[static_cast<uint8_t>(address)] = value;
}

template <uint8_t Size>
void register_array<Size>::write_version_numbers()
{
    _registers[static_cast<uint8_t>(register_address::PROJECT_MAJOR)] = config::PROJECT_MAJOR;
    _registers[static_cast<uint8_t>(register_address::PROJECT_MINOR)] = config::PROJECT_MINOR;
    _registers[static_cast<uint8_t>(register_address::PROJECT_REVISION)] = config::PROJECT_REVISION;
    _registers[static_cast<uint8_t>(register_address::PROJECT_BUILD_NUMBER)] = config::PROJECT_BUILD_NUMBER;
}

template class register_array<config::REGISTERS_SIZE>;

} // namespace modules
