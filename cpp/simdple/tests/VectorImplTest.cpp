#include "VectorImpl.h"
#include <gtest/gtest.h>
#include <cstring>

using namespace simdple;

template<typename T>
class VectorImplTest : public ::testing::Test {};

typedef ::testing::Types<VectorImpl<char, 16>, VectorImpl<short, 8> > MyTypes;
TYPED_TEST_CASE(VectorImplTest, MyTypes);

TYPED_TEST(VectorImplTest, TestAdd)
{
    TypeParam v = TypeParam::Load(1);
    TypeParam v_2 = v;
    TypeParam v_3 = v.Add(v_2);
    TypeParam v_4 = v_3.Add(v);
    TypeParam v_5 = v_4.Add(v_3);
    ASSERT_EQ(1, v_5[0]);

    v.AddFrom(v_2);
    v_3.AddFrom(v);
    ASSERT_EQ(1, v_3[0]);

    v = v_2 + v_5;
    v_4 = v + v_3;
    v_3 = v_5 + v_2 + v + v_4;
    ASSERT_EQ(1, v_3[0]);
}

TYPED_TEST(VectorImplTest, TestSub)
{
    TypeParam v = TypeParam::Load(1);
    TypeParam v_2 = v;
    TypeParam v_3 = v.Sub(v_2);
    TypeParam v_4 = v_3.Sub(v);
    TypeParam v_5 = v_4.Sub(v_2);
    ASSERT_EQ(1, v_5[0]);
}

TYPED_TEST(VectorImplTest, TestAndNot)
{
    TypeParam v = TypeParam::Load(10);
    TypeParam v_2 = TypeParam::Load(23);
    TypeParam v_3 = v.BitwiseAndNot(v_2);
    v_3 = v_3.BitwiseAndNot(v + v_2);
    TypeParam v_4 = v + v_3;
    v_3 = v.BitwiseAndNot(v_4 + v_2);
    ASSERT_EQ(2, v_3[0]);
}

