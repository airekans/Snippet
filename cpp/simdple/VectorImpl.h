#ifndef CPP_SIMDPLE_VECTORIMPL_H_
#define CPP_SIMDPLE_VECTORIMPL_H_

#include <x86intrin.h>

namespace simdple {
namespace detail {

template<bool> struct StaticAssert;
template<> struct StaticAssert<true> { typedef int Type; };

template<typename T, unsigned int N> struct GeneralPack;
template<> struct GeneralPack<char, 16> { typedef __m128i Type; };
template<> struct GeneralPack<short, 16> { typedef __m128i Type; };
template<> struct GeneralPack<int, 16> { typedef __m128i Type; };
template<> struct GeneralPack<long long, 16> { typedef __m128i Type; };
template<> struct GeneralPack<float, 16> { typedef __m128 Type; };
template<> struct GeneralPack<double, 16> { typedef __m128d Type; };

template<typename T, typename VT, unsigned int N>
struct Pack
{
    typedef T ElemType;
    typedef VT VType;
    typedef typename GeneralPack<T, sizeof(VT)>::Type GeneralVT;
    enum { ElemNum = N };
    typedef typename StaticAssert<(sizeof(VT) / sizeof(T)) == N>::Type _ASSERT;

    union {
        VT v;
        GeneralVT gv;
        T data[N];
    };
};

}  // namespace detail

template<typename T, unsigned int N>
struct VectorImpl;

template<> struct VectorImpl<char, 16> : public detail::Pack<char, __v16qi, 16>
{
    VectorImpl Add(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = _mm_add_epi8(gv, other.gv);
        return res;
    }

    VectorImpl Sub(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = _mm_sub_epi8(gv, other.gv);
        return res;
    }
};

template<> struct VectorImpl<short, 8> : public detail::Pack<short, __v8hi, 8>
{
    VectorImpl Add(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = _mm_add_epi16(gv, other.gv);
        return res;
    }

    VectorImpl Sub(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = _mm_sub_epi16(gv, other.gv);
        return res;
    }
};

template<> struct VectorImpl<int, 4> : public detail::Pack<int, __v4si, 4> {};
template<> struct VectorImpl<long long, 2> : public detail::Pack<long long, __v2di, 2> {};
template<> struct VectorImpl<float, 4> : public detail::Pack<float, __v4sf, 4> {};
template<> struct VectorImpl<double, 2> : public detail::Pack<double, __v2df, 2> {};

template<typename T, unsigned int N>
VectorImpl<T, N> operator+(const VectorImpl<T, N> lhs, const VectorImpl<T, N> rhs)
{
    return lhs.Add(rhs);
}

template<typename T, unsigned int N>
VectorImpl<T, N> operator-(const VectorImpl<T, N> lhs, const VectorImpl<T, N> rhs)
{
    return lhs.Sub(rhs);
}

}  // namespace simdple


#endif /* CPP_SIMDPLE_VECTORIMPL_H_ */
