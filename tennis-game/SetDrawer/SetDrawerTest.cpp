#include "../googletest/googletest/include/gtest/gtest.h"
#include "./SetDrawer.h"

class SetDrawerTest : public ::testing::Test {
 protected:
    SetDrawerTest() {}            // You can do set-up work for each test here.
    ~SetDrawerTest() override {}  // You can do clean-up work that doesn't throw exceptions here.
    
    void SetUp() override {
        // Code here will be called immediately after the constructor (right before each test).
        setDrawer = new SetDrawer( /* provide the necessary arguments to create a SetDrawer object */ );
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right before the destructor).
        delete setDrawer;
    }

    SetDrawer* setDrawer;
};

TEST_F(SetDrawerTest, TestCloaker) {
    std::string stringToCloak = " 4 3 2";
    int sectionToCloak = 2;
    std::string expectedOutput = " 4   2";  // Replace this with the expected output of the cloaker method

    ASSERT_EQ(setDrawer->cloaker(stringToCloak, sectionToCloak), expectedOutput);
}

// int main(int argc, char** argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
