#include "eject_filament_command.h"
#include "load_filament_command.h"
#include "load_to_nozzle_command.h"
#include "registers.h"
#include "test_base.h"
#include "tool_change_command.h"
#include "unload_from_nozzle_command.h"

class LogicTest : public TestBase {
protected:
    modules::registers _registers;
    logic::load_filament_command _load_filament_command { _feeding_systems, hubFSensor, _registers };
    logic::eject_filament_command _eject_filament_command { _feeding_systems, hubFSensor, _registers };
    logic::unload_from_nozzle_command _unload_from_nozzle_command { _feeding_systems, hubFSensor, _registers };
    logic::load_to_nozzle_command _load_to_nozzle_command { _feeding_systems, hubFSensor, _registers };
    logic::tool_change_command _tool_change_command { _unload_from_nozzle_command, _load_to_nozzle_command };
};

TEST_F(LogicTest, LoadFilament)
{
    // trying to load filament with first pulley system
    uint16_t _slot_index = 0;
    _load_filament_command.reset(_slot_index);
    ASSERT_EQ(_load_filament_command.get_state(), progress_code::LOADING_FILAMENT);
    ASSERT_EQ(_slot_index, _registers.read(modules::register_address::CURRENT_SLOT));

    aFeedingSystemFSensor.set_return_value(false);
    for (std::size_t i = 0; i < 10; ++i) {
        _load_filament_command.step();
        ASSERT_FALSE(afeeding_system.has_filament());
        ASSERT_EQ(afeeding_system.get_filament_position(), modules::filament_position::NOT_LOADED);
        ASSERT_EQ(_slot_index, _registers.read(modules::register_address::CURRENT_SLOT));
    }

    // TODO
    // pulleyFSensors[_slot_index].set_return_value(true);
    aFeedingSystemFSensor.set_return_value(true);

    _load_filament_command.step();
    ASSERT_TRUE(_feeding_systems[_slot_index].has_filament());
    ASSERT_EQ(_feeding_systems[_slot_index].get_filament_position(), modules::filament_position::IN_FEEDING_SYSTEM);
    ASSERT_EQ(_slot_index, _registers.read(modules::register_address::CURRENT_SLOT));

    hubFSensor.set_return_value(true);
    _load_filament_command.step();
    ASSERT_EQ(_feeding_systems[_slot_index].get_filament_position(), modules::filament_position::IN_FILAMENT_HUB);
    ASSERT_EQ(_slot_index, _registers.read(modules::register_address::CURRENT_SLOT));

    hubFSensor.set_return_value(false);
    for (std::size_t i = 0; i <= config::HUB_HELP_ADDITIONAL_STEPS; ++i) {
        _load_filament_command.step();
    }
    ASSERT_EQ(_feeding_systems[_slot_index].get_filament_position(), modules::filament_position::BEFORE_FILAMENT_HUB);
    ASSERT_EQ(_load_filament_command.get_state(), progress_code::OK);
    ASSERT_EQ(_slot_index, _registers.read(modules::register_address::CURRENT_SLOT));
}

TEST_F(LogicTest, EjectFilament)
{
    // trying to eject filament with first pulley system
    uint16_t _slot_index = 0;
    _eject_filament_command.reset(_slot_index);
    ASSERT_EQ(_eject_filament_command.get_state(), progress_code::EJECTING_FILAMENT);
    ASSERT_EQ(_slot_index, _registers.read(modules::register_address::CURRENT_SLOT));

    aFeedingSystemFSensor.set_return_value(true);
    for (std::size_t i = 0; i < 10; ++i) {
        _eject_filament_command.step();
        ASSERT_TRUE(_feeding_systems[_slot_index].has_filament());
        ASSERT_EQ(_slot_index, _registers.read(modules::register_address::CURRENT_SLOT));
    }

    aFeedingSystemFSensor.set_return_value(false);
    _eject_filament_command.step();
    ASSERT_FALSE(_feeding_systems[_slot_index].has_filament());
    ASSERT_EQ(_feeding_systems[_slot_index].get_filament_position(), modules::filament_position::NOT_LOADED);
    ASSERT_EQ(_eject_filament_command.get_state(), progress_code::OK);
    ASSERT_EQ(_slot_index, _registers.read(modules::register_address::CURRENT_SLOT));
}

TEST_F(LogicTest, UnloadFromNozzle)
{
    _unload_from_nozzle_command.reset(0);
    ASSERT_EQ(_unload_from_nozzle_command.get_state(), progress_code::UNLOADING_FILAMENT);

    hubFSensor.set_return_value(true);
    for (std::size_t i = 0; i < 10; ++i) {
        _unload_from_nozzle_command.step();
        ASSERT_EQ(aMotor.get_direction(), modules::motor_direction::BACKWARD);
    }

    hubFSensor.set_return_value(false);
    for (std::size_t i = 0; i <= config::HUB_HELP_ADDITIONAL_STEPS; ++i) {
        _unload_from_nozzle_command.step();
    }

    ASSERT_EQ(_feeding_systems[0].get_filament_position(), modules::filament_position::BEFORE_FILAMENT_HUB);
    ASSERT_EQ(_unload_from_nozzle_command.get_state(), progress_code::OK);
}

TEST_F(LogicTest, LoadToNozzle)
{
}
