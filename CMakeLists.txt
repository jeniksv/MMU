project(mmu)

cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(
    mmu
    src/app.cpp
    src/command_base.cpp
    src/concrete_command_base.cpp
    #src/crc.cpp
    src/no_command.cpp
    src/load_filament_command.cpp
    src/unload_from_nozzle_command.cpp
    src/load_to_nozzle_command.cpp
    src/tool_change_command.cpp
    src/eject_filament_command.cpp
    src/protocol.cpp
    src/pins.cpp
    src/dc_motor.cpp
    src/uart.cpp
    src/hal_time.cpp
    src/feeding_system.cpp
    src/registers.cpp
)

target_compile_definitions(mmu PRIVATE UNITTEST)

subdirs(lib/googletest)

add_executable(
  tests_runner
  tests/communication_test.cpp
  tests/hardware_test.cpp
  tests/gpio_test.cpp
  tests/logic_test.cpp
) 

target_link_libraries(
  tests_runner
  GTest::gtest_main
  GTest::gmock_main
  mmu
)

target_compile_definitions(tests_runner PRIVATE UNITTEST)

include(GoogleTest)
gtest_discover_tests(tests_runner)
