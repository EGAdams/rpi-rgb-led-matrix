#include <gtest/gtest.h>
#include "LogObject.h"

TEST(LogObjectTest, DefaultConstructor) {
    LogObject logObject;
    EXPECT_TRUE(logObject.getId().empty());
    EXPECT_EQ(logObject.getTimestamp(), 0);
    EXPECT_TRUE(logObject.getMessage().empty());
    EXPECT_TRUE(logObject.getMethod().empty());
}

TEST(LogObjectTest, SettersAndGetters) {
    LogObject logObject;
    const std::string id = "log123";
    const uint64_t timestamp = 1234567890;
    const std::string message = "This is a log message";
    const std::string method = "SomeMethod";

    logObject.setId(id);
    logObject.setTimestamp(timestamp);
    logObject.setMessage(message);
    logObject.setMethod(method);

    EXPECT_EQ(logObject.getId(), id);
    EXPECT_EQ(logObject.getTimestamp(), timestamp);
    EXPECT_EQ(logObject.getMessage(), message);
    EXPECT_EQ(logObject.getMethod(), method);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
