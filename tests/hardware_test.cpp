#include "test_base.h"

#include "dc_motor.h"
#include "feeding_system.h"
#include "pins.h"
#include "sensor.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class HardwareComponentsTest : public TestBase {
protected:
    modules::sliding_window<int, 5> _sliding_window;
};

TEST_F(HardwareComponentsTest, sliding_windowInitiallyZero)
{
    EXPECT_EQ(_sliding_window.get_average(), 0);
}

TEST_F(HardwareComponentsTest, sliding_windowHandlesUpdatesCorrectly)
{
    _sliding_window.update_with(1);
    EXPECT_EQ(_sliding_window.get_average(), 1);

    _sliding_window.update_with(2);
    _sliding_window.update_with(3);
    _sliding_window.update_with(4);
    _sliding_window.update_with(5);
    EXPECT_EQ(_sliding_window.get_average(), 3);
}

TEST_F(HardwareComponentsTest, sliding_windowHandlesWrapAround)
{
    uint16_t valuesToAdd[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    for (uint16_t val : valuesToAdd) {
        _sliding_window.update_with(val);
    }
    EXPECT_EQ(_sliding_window.get_average(), 8);
}

TEST_F(HardwareComponentsTest, sliding_windowPartialWindow)
{
    _sliding_window.update_with(10);
    _sliding_window.update_with(20);
    EXPECT_EQ(_sliding_window.get_average(), 15);
}

TEST_F(HardwareComponentsTest, dc_motor)
{
    aMotor.start(modules::motor_direction::FORWARD, 50);
    ASSERT_EQ(aPwmPin.get_current_duty_cycle(), 50);
    ASSERT_EQ(stby1Pin.get_state(), HAL::gpio_pin_state::HIGH);
    ASSERT_EQ(aMotor.get_direction(), modules::motor_direction::FORWARD);
}

TEST_F(HardwareComponentsTest, feeding_system)
{
    afeeding_system.set_filament_position(modules::filament_position::NOT_LOADED);
    ASSERT_EQ(afeeding_system.get_filament_position(), modules::filament_position::NOT_LOADED);
    ASSERT_FALSE(afeeding_system.has_filament());
}
