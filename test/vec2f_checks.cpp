// Tests for Vec2f class functionality

#include <Vec2f.h>
#include "gtest/gtest.h"

class Vec2fFixture : public ::testing::Test {
public:
    Vec2fFixture() {
        // Init
        a = Vec2f(2.0f, -2.0f);
        b = Vec2f(0.5f,  0.25f);
        f = 5.0f;
    }

    void SetUp() {
        // Pre Test
    }

    void TearDown() {
        // Post Test
        // Exceptions allowed
    }

    ~Vec2fFixture() {

    }

    Vec2f a;
    Vec2f b;
    float f;
};

TEST_F(Vec2fFixture, GetTest) {
    EXPECT_EQ(a.get(0), 2.0f);
    EXPECT_EQ(a.get(1), -2.0f);
    EXPECT_EQ(b.get(0), 0.5f);
    EXPECT_EQ(b.get(1), 0.25f);
}

TEST_F(Vec2fFixture, SetCopyTest) {
    Vec2f tmp = Vec2f(&a);
    tmp.set(10.0f, 5.1f);
    // Tests must allow for floating point error.
    ASSERT_NEAR(tmp.get(0), 10.0f, 0.0001) << tmp.get(0);
    ASSERT_NEAR(tmp.get(1), 5.1f, 0.0001) << tmp.get(1);
    std::cout << "A: " << a.get(0) << ", " << a.get(1) << std::endl;
    std::cout << "TMP: " << tmp.get(0) << ", " << tmp.get(1) << std::endl;
}

TEST_F(Vec2fFixture, ScaleTest) {
    Vec2f tmp = b * f;
    // Tests must allow for floating point error.
    std::cout << "B: " << b.get(0) << ", " << b.get(1) << std::endl;
    std::cout << "B*5: " << tmp.get(0) << ", " << tmp.get(1) << std::endl;
    ASSERT_NEAR(tmp.get(0), 2.5f, 0.0001) << tmp.get(0);
    ASSERT_NEAR(tmp.get(1), 1.25f, 0.0001) << tmp.get(1);
}

TEST_F(Vec2fFixture, SumTest) {
    Vec2f tmp = a + b;
    // Tests must allow for floating point error.
    std::cout << "A+B: " << tmp.get(0) << ", " << tmp.get(1) << std::endl;
    ASSERT_NEAR(tmp.get(0), 2.5f, 0.0001) << tmp.get(0);
    ASSERT_NEAR(tmp.get(1), -1.75f, 0.0001) << tmp.get(1);
}

TEST_F(Vec2fFixture, SumToTest) {
    a += b;
    // Tests must allow for floating point error.
    std::cout << "A+=B: " << a.get(0) << ", " << a.get(1) << std::endl;
    ASSERT_NEAR(a.get(0), 2.5f, 0.0001) << a.get(0);
    ASSERT_NEAR(a.get(1), -1.75f, 0.0001) << a.get(1);
}