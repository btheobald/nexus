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

TEST_F(Vec2fFixture, SetTest) {
    a.set(10.0f, 5.1f);
    ASSERT_NEAR(a.get(0), 10.0f, 0.001) << a.get(0);
    ASSERT_NEAR(a.get(1), 5.1f, 0.001) << a.get(1);
}