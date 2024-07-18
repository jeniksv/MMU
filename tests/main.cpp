#define UNITTEST

#include "gtest/gtest.h"

uint16_t main(uint16_t argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
