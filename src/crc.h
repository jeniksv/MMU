#pragma once

#include <stdint.h>
#include <type_traits>

namespace modules {

/**
 * @brief Updates the CRC (Cyclic Redundancy Check) with the bytes of the given data.
 *
 * This template function computes the CRC byte by byte over the entirety of an integral data type.
 * It leverages partial template specialization for `uint8_t` for the actual CRC computation.
 *
 * @tparam T Integral data type of the input data.
 * @param crc The initial CRC value.
 * @param data The data over which to compute the CRC.
 * @return uint8_t The updated CRC value.
 *
 * @note The function static_asserts that T is an integral type.
 */
template <typename T>
uint8_t update_crc(uint8_t crc, T data)
{
    static_assert(std::is_integral<T>::value, "data must be an integral type");

    for (uint8_t i = 0; i < sizeof(T); ++i) {
        uint8_t byte = static_cast<uint8_t>(data & 0xFF);
        crc = update_crc<uint8_t>(crc, byte);
        data >>= 8;
    }

    return crc;
}

/**
 * @brief Specializes the CRC update function for `uint8_t` data type.
 *
 * This function performs a bitwise computation to update the CRC using a specific polynomial.
 * It is used by the template function `update_crc` when processing byte data.
 *
 * @param crc The initial CRC value.
 * @param b The byte to process.
 * @return uint8_t The updated CRC value.
 */
template <>
inline uint8_t update_crc<uint8_t>(uint8_t crc, uint8_t b)
{
    uint8_t polynomial = 0x07; // CRC-8 polynomial
    crc ^= b;

    for (uint8_t i = 0; i < 8; ++i) {
        if (crc & 0x80) {
            crc = (crc << 1) ^ polynomial;
        } else {
            crc <<= 1;
        }
    }

    return crc;
}

} // namespace modules
