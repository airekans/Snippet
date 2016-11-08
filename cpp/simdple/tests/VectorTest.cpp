#include "simdple/Vector.h"
#include <gtest/gtest.h>
#include <iostream>


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

TEST(VectorTest, testAndNot)
{
    Vector<char, 16> v(10);
    Vector<char, 16> v1(23);
    Vector<char, 16> v2(v + v1);  // 33
    Vector<char, 16> v3(op::BitwiseNot(v + v2) & v1);
    Vector<char, 16> v4(v2 & op::BitwiseNot(v3));
    Vector<char, 16> v5(op::BitwiseNot(v3) & op::BitwiseNot(v1));
    ASSERT_EQ(20, v3.v[0]);
    ASSERT_EQ(33, v4.v[0]);
    ASSERT_EQ(static_cast<char>(232), v5.v[0]);
}

