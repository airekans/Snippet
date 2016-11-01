#ifndef CPP_SIMDPLE_VECTORIMPL_H_
#define CPP_SIMDPLE_VECTORIMPL_H_

#include <x86intrin.h>

namespace simdple {
namespace detail {

template<bool> struct StaticAssert;
template<> struct StaticAssert<true> { typedef int Type; };

template<unsigned int N>
struct AssertIsPowerOf2
{
    typedef typename StaticAssert<(N > 0) && ((N & (N - 1)) == 0)>::Type Type;
};


template<typename T, typename VT, typename GVT, unsigned int N>
struct Pack
{
    typedef T ElemType;
    typedef VT VType;
    typedef GVT GeneralVT;
    enum { ElemNum = N };
    typedef typename StaticAssert<(sizeof(VT) / sizeof(T)) == N>::Type _ASSERT;
    typedef typename AssertIsPowerOf2<N>::Type _IS_POWER_OF_2;

    union {
        VT v;
        GeneralVT gv;
        T data[N];
    };

    inline T& operator[](size_t i)
    {
        return data[i];
    }

    inline T operator[](size_t i) const
    {
        return data[i];
    }
};

}  // namespace detail

template<typename T, unsigned int N>
struct VectorImpl;

template<> struct VectorImpl<char, 16> : public detail::Pack<char, __v16qi, __m128i, 16>
{
    inline static VectorImpl Load(char elem)
    {
        VectorImpl res;
        res.Set(elem);
        return res;
    }

    inline void Set(char elem)
    {
        gv = _mm_set1_epi8(elem);
    }

    inline void AddFrom(const VectorImpl other)
    {
        gv = _mm_add_epi8(gv, other.gv);
    }

    inline VectorImpl Add(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = gv;
        res.AddFrom(other);
        return res;
    }

    inline void SubFrom(const VectorImpl other)
    {
        gv = _mm_sub_epi8(gv, other.gv);
    }

    inline VectorImpl Sub(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = gv;
        res.SubFrom(other);
        return res;
    }

    inline void AndFrom(const VectorImpl other)
    {
        gv = _mm_and_si128(gv, other.gv);
    }

    inline VectorImpl And(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = gv;
        res.AndFrom(other);
        return res;
    }

    // (!a) & b
    // This can be used in expression template to optimize
    inline void AndNotFrom(const VectorImpl other)
    {
        gv = _mm_andnot_si128(gv, other.gv);
    }

    inline VectorImpl AndNot(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = gv;
        res.AndNotFrom(other);
        return res;
    }

    inline VectorImpl IsEqual(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = _mm_cmpeq_epi8(gv, other.gv);
        return res;
    }
};

template<> struct VectorImpl<short, 8> : public detail::Pack<short, __v8hi, __m128i, 8>
{
    inline static VectorImpl Load(short elem)
    {
        VectorImpl res;
        res.Set(elem);
        return res;
    }

    inline void Set(short elem)
    {
        gv = _mm_set1_epi16(elem);
    }

    inline void AddFrom(const VectorImpl other)
    {
        gv = _mm_add_epi16(gv, other.gv);
    }

    inline VectorImpl Add(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = gv;
        res.AddFrom(other);
        return res;
    }

    inline void SubFrom(const VectorImpl other)
    {
        gv = _mm_sub_epi16(gv, other.gv);
    }

    inline VectorImpl Sub(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = gv;
        res.SubFrom(other);
        return res;
    }
};

template<> struct VectorImpl<int, 4> : public detail::Pack<int, __v4si, __m128i, 4> {};
template<> struct VectorImpl<long long, 2> : public detail::Pack<long long, __v2di, __m128i, 2> {};
template<> struct VectorImpl<float, 4> : public detail::Pack<float, __v4sf, __m128, 4> {};
template<> struct VectorImpl<double, 2> : public detail::Pack<double, __v2df, __m128d, 2> {};

template<typename T, unsigned int N>
inline VectorImpl<T, N> operator+(const VectorImpl<T, N> lhs, const VectorImpl<T, N> rhs)
{
    return lhs.Add(rhs);
}

template<typename T, unsigned int N>
inline VectorImpl<T, N> operator-(const VectorImpl<T, N> lhs, const VectorImpl<T, N> rhs)
{
    return lhs.Sub(rhs);
}

template<typename T, unsigned int N>
inline VectorImpl<T, N> operator&(const VectorImpl<T, N> lhs, const VectorImpl<T, N> rhs)
{
    return lhs.And(rhs);
}

}  // namespace simdple


#endif /* CPP_SIMDPLE_VECTORIMPL_H_ */
