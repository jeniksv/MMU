#include "test_base.h"

#include "app.h"
#include "config.h"
#include "registers.h"
#include "time.h"
#include "uart.h"

#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>

class CommunicationTest : public TestBase {
protected:
    HAL::UART c {};
    modules::registers r {};
    app a { _feeding_systems, hubFSensor, c, r };

    void TestHelper(const std::string& inputFilePath, const std::string& expectedFilePath)
    {
        std::ifstream inputFile(inputFilePath);
        std::ifstream expectedFile(expectedFilePath);

        if (!inputFile.is_open()) {
            EXPECT_TRUE(false) << "Unable to open file with _requests.";
        }

        if (!expectedFile.is_open()) {
            EXPECT_TRUE(false) << "Unable to open file with expected reponses.";
        }

        std::string _request;
        std::string response;

        while (getline(inputFile, _request) && getline(expectedFile, response)) {
            writeTouart(_request + "\n");

            a.step();

            std::string lastMessage = readFromuart();
            // std::cout << lastMessage;

            ASSERT_EQ(lastMessage, response + "\n");
        }
    }

    void writeTouart(const std::string& message)
    {
        for (const auto& byte : message) {
            c.on_rx_interrupt(byte);
        }
    }

    std::string readFromuart()
    {
        std::string message;

        while (true) {
            auto byte = c.on_tx_interrupt();
            message += byte;

            if (byte == '\n') {
                return message;
            }
        }

        return "";
    }
};

class InitHandshakeTest : public CommunicationTest { };

class EjectFilamentTest : public CommunicationTest { };

class LoadFilamentTest : public CommunicationTest { };

class UnloadFromNozzleTest : public CommunicationTest { };

TEST_F(CommunicationTest, InvalidMessages)
{
    writeTouart("S0+c6\nS1*ae\nS10*10\nS*7b\n");

    for (size_t i = 0; i < 4; ++i) {
        a.step();
        ASSERT_EQ(c.on_tx_interrupt(), 0);
    }
}

TEST_F(InitHandshakeTest, InitHandshake)
{
    TestHelper("tests/test_data/init", "tests/test_data/init_result");
}

TEST_F(LoadFilamentTest, LoadFilament)
{
    aFeedingSystemFSensor.set_return_value(true);
    hubFSensor.set_return_value(false);
    TestHelper("tests/test_data/load_first", "tests/test_data/load_first_result");

    hubFSensor.set_return_value(true);
    a.step();
    hubFSensor.set_return_value(false);

    for (uint16_t i = 0; i < config::HUB_HELP_ADDITIONAL_STEPS; ++i) {
        a.step();
    }

    TestHelper("tests/test_data/load_second", "tests/test_data/load_second_result");
}

TEST_F(UnloadFromNozzleTest, UnloadFromNozzle)
{
    hubFSensor.set_return_value(true);
    TestHelper("tests/test_data/unload_first", "tests/test_data/unload_first_result");

    hubFSensor.set_return_value(false);
    for (uint16_t i = 0; i < config::HUB_HELP_ADDITIONAL_STEPS; ++i) {
        a.step();
    }

    TestHelper("tests/test_data/unload_second", "tests/test_data/unload_second_result");
}

TEST_F(EjectFilamentTest, EjectFilament)
{
    r.write(modules::register_address::FEEDING_SYSTEM_FSENSOR_STATE, 0);
    aFeedingSystemFSensor.set_return_value(true);
    TestHelper("tests/test_data/eject_first", "tests/test_data/eject_first_result");

    aFeedingSystemFSensor.set_return_value(false);
    TestHelper("tests/test_data/eject_second", "tests/test_data/eject_second_result");
}
