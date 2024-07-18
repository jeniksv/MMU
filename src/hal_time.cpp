#include "hal_time.h"

#ifdef UNITTEST

#include <chrono>

unsigned long HAL::GetTime()
{
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return static_cast<unsigned long>(duration.count());
}

#else

unsigned long HAL::current_millis = 0;

unsigned long HAL::GetTime()
{
    return current_millis;
}

#endif
