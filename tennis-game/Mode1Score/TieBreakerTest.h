#ifndef TIEBREAKERTEST_H
#define TIEBREAKERTEST_H

#include <gtest/gtest.h>

class TieBreakerTest : public ::testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;
};

#endif // TIEBREAKERTEST_H
