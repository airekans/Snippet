#include "VectorImpl.h"
#include <gtest/gtest.h>
#include <cstring>
#include <malloc.h>

using namespace simdple;

template<typename T>
class GeneralVectorImplTest : public ::testing::Test {};

typedef ::testing::Types<VectorImpl<char, 16>, VectorImpl<short, 8>,
        VectorImpl<int, 4>, VectorImpl<long long, 2>,
        VectorImpl<float, 4>, VectorImpl<double, 2>
#ifdef __AVX2__
        , VectorImpl<char, 32>
#endif
> GeneralTypes;
TYPED_TEST_CASE(GeneralVectorImplTest, GeneralTypes);

TYPED_TEST(GeneralVectorImplTest, TestLoad)
{
    typedef typename TypeParam::ElemType ElemType;
    ElemType* addr = reinterpret_cast<ElemType*>(
            memalign(sizeof(TypeParam), sizeof(TypeParam)));
    for (int i = 0; i < sizeof(TypeParam) / sizeof(*addr); ++i) {
        addr[i] = i + 1;
    }

    TypeParam v = TypeParam::Load(addr);
    for (size_t i = 0; i < TypeParam::ElemNum; ++i) {
        ASSERT_EQ(i + 1, v[i]);
    }

    v = TypeParam::Load(3);
    for (size_t i = 0; i < TypeParam::ElemNum; ++i) {
        ASSERT_EQ(3, v[i]);
    }
}

TYPED_TEST(GeneralVectorImplTest, TestAdd)
{
    TypeParam v = TypeParam::Load(1);
    TypeParam v_2 = v;
    TypeParam v_3 = v.Add(v_2);
    TypeParam v_4 = v_3.Add(v);
    TypeParam v_5 = v_4.Add(v_3);
    ASSERT_EQ(5, v_5[0]);

    v.AddFrom(v_2);
    v_3.AddFrom(v);
    ASSERT_EQ(4, v_3[0]);

    v = v_2 + v_5;
    v_4 = v + v_3;
    v_3 = v_5 + v_2 + v + v_4;
    ASSERT_EQ(22, v_3[0]);
}

TYPED_TEST(GeneralVectorImplTest, TestSub)
{
    TypeParam v = TypeParam::Load(10);
    TypeParam v_2 = TypeParam::Load(5);
    TypeParam v_3 = v.Sub(v_2);
    TypeParam v_4 = v_3.Sub(v);
    TypeParam v_5 = v_4.Sub(v_2);
    ASSERT_EQ(-10, v_5[0]);
}

template<typename T>
class VectorImplBitwiseTest : public ::testing::Test {};

typedef ::testing::Types<VectorImpl<char, 16>, VectorImpl<short, 8>,
        VectorImpl<int, 4>, VectorImpl<long long, 2> > BitwiseTypes;
TYPED_TEST_CASE(VectorImplBitwiseTest, BitwiseTypes);

TYPED_TEST(VectorImplBitwiseTest, TestAndNot)
{
    TypeParam v = TypeParam::Load(10);
    TypeParam v_2 = TypeParam::Load(23);
    TypeParam v_3 = v.BitwiseAndNot(v_2);
    ASSERT_EQ(21, v_3[0]);
    v_3 = v_3.BitwiseAndNot(v + v_2);
    TypeParam v_4 = v + v_3;
    v_3 = v.BitwiseAndNot(v_4 + v_2);
    ASSERT_EQ(65, v_3[0]);
}

