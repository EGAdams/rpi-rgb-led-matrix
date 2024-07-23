#include <gtest/gtest.h>
#include "../include/my_class.h"

TEST(MyClassTest, Add) {
    MyClass myClass;
    EXPECT_EQ(myClass.add(2, 3), 5);
    EXPECT_EQ(myClass.add(-1, 1), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
