#pragma once

/**
 * @class fifo
 * @brief A templated class representing a fixed-size FIFO (First In, First Out) buffer.
 *
 * This class provides methods to push items into the buffer, pop items from the buffer,
 * and check the buffer's state (whether it is full or empty).
 *
 * @tparam SIZE The capacity of the FIFO buffer.
 */
template <uint16_t SIZE>
class fifo {
public:
    /**
     * @brief Constructor for the fifo class.
     *
     * Initializes the FIFO buffer with the given capacity.
     */
    fifo()
        : _capacity(SIZE)
        , _current_size(0)
        , _head(0)
        , _tail(0)
    {
    }

    /**
     * @brief Pushes an item into the FIFO buffer.
     *
     * @param item The item to be pushed into the buffer.
     * @return True if the item was successfully pushed, false if the buffer is full.
     */
    bool push(const uint8_t& item)
    {
        if (is_full()) {
            return false;
        }
        _buffer[_tail] = item;
        _tail = (_tail + 1) % _capacity;
        ++_current_size;
        return true;
    }

    /**
     * @brief Pops an item from the FIFO buffer.
     *
     * @param item The item to be popped from the buffer.
     * @return True if the item was successfully popped, false if the buffer is empty.
     */
    bool pop(uint8_t& item)
    {
        if (is_empty()) {
            return false;
        }
        item = _buffer[_head];
        _head = (_head + 1) % _capacity;
        --_current_size;
        return true;
    }

    /**
     * @brief Checks if the FIFO buffer is empty.
     *
     * @return True if the buffer is empty, false otherwise.
     */
    bool is_empty() const
    {
        return _current_size == 0;
    }

    /**
     * @brief Checks if the FIFO buffer is full.
     *
     * @return True if the buffer is full, false otherwise.
     */
    bool is_full() const
    {
        return _current_size == _capacity;
    }

    /**
     * @brief Gets the current size of the FIFO buffer.
     *
     * @return The number of items currently in the buffer.
     */
    uint16_t size() const
    {
        return _current_size;
    }

private:
    uint8_t _buffer[SIZE];
    uint16_t _head, _tail;
    const uint16_t _capacity;
    uint16_t _current_size;
};
