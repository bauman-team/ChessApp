#include "pch.h"

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    auto error = RUN_ALL_TESTS();
    return error;
}
