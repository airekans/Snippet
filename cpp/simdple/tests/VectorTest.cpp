#include "simdple/Vector.h"
#include <gtest/gtest.h>

using namespace simdple;

TEST(VectorTest, TestCtor)
{
    Vector<char, 16> v(1);
    ASSERT_EQ(1, v.v[0]);
}

TEST(VectorTest, TestAdd)
{
    Vector<char, 16> v(1);
    Vector<char, 16> v1(2);
    Vector<char, 16> v2(3);
    Vector<char, 16> v3(v + v2 + v1);
    v3 = v + v3;
    ASSERT_EQ(7, v3.v[0]);
}

