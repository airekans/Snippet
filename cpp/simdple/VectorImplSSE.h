#ifndef CPP_SIMDPLE_VECTORIMPLSSE_H_
#define CPP_SIMDPLE_VECTORIMPLSSE_H_

#include <string.h>
#include <x86intrin.h>

#include "simdple/detail/Pack.h"

namespace simdple {

template<> struct VectorImpl<char, 16> :
    public detail::IntegerPack<char, __v16qi, __m128i, 16>
{
    // addr must be 128-bit aligned
    inline void Set(const char* addr)
    {
        gv = _mm_load_si128(reinterpret_cast<const GeneralVT*>(addr));
    }

    inline void Set(char elem)
    {
        gv = _mm_set1_epi8(elem);
    }

    inline void AddFrom(const VectorImpl other)
    {
        gv = _mm_add_epi8(gv, other.gv);
    }

    inline void SubFrom(const VectorImpl other)
    {
        gv = _mm_sub_epi8(gv, other.gv);
    }

    inline void BitwiseAndFrom(const VectorImpl other)
    {
        gv = _mm_and_si128(gv, other.gv);
    }

    inline VectorImpl BitwiseAnd(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = gv;
        res.BitwiseAndFrom(other);
        return res;
    }

    // (!a) & b
    // This can be used in expression template to optimize
    inline void BitwiseAndNotFrom(const VectorImpl other)
    {
        gv = _mm_andnot_si128(gv, other.gv);
    }

    inline void BitwiseXorFrom(const VectorImpl other)
    {
        gv = _mm_xor_si128(gv, other.gv);
    }

    inline void BitwiseNotSelf()
    {
        VectorImpl tmp;
        ::memset(&tmp, 0xFF, sizeof(VectorImpl));
        BitwiseXorFrom(tmp);
    }

    inline VectorImpl BitwiseNot() const
    {
        VectorImpl res;
        res.gv = gv;
        res.BitwiseNotSelf();
        return res;
    }

    inline VectorImpl IsEqual(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = _mm_cmpeq_epi8(gv, other.gv);
        return res;
    }

    inline VectorImpl IsGreater(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = _mm_cmpgt_epi8(gv, other.gv);
        return res;
    }
};

template<> struct VectorImpl<short, 8> :
    public detail::IntegerPack<short, __v8hi, __m128i, 8>
{
    // addr must be 128-bit aligned
    inline void Set(const short* addr)
    {
        gv = _mm_load_si128(reinterpret_cast<const GeneralVT*>(addr));
    }

    inline void Set(short elem)
    {
        gv = _mm_set1_epi16(elem);
    }

    inline void AddFrom(const VectorImpl other)
    {
        gv = _mm_add_epi16(gv, other.gv);
    }

    inline void SubFrom(const VectorImpl other)
    {
        gv = _mm_sub_epi16(gv, other.gv);
    }

    inline void BitwiseAndFrom(const VectorImpl other)
    {
        gv = _mm_and_si128(gv, other.gv);
    }

    inline VectorImpl BitwiseAnd(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = gv;
        res.BitwiseAndFrom(other);
        return res;
    }

    // (!a) & b
    // This can be used in expression template to optimize
    inline void BitwiseAndNotFrom(const VectorImpl other)
    {
        gv = _mm_andnot_si128(gv, other.gv);
    }

    inline VectorImpl BitwiseAndNot(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = gv;
        res.BitwiseAndNotFrom(other);
        return res;
    }

    inline void BitwiseXorFrom(const VectorImpl other)
    {
        gv = _mm_xor_si128(gv, other.gv);
    }

    inline VectorImpl BitwiseXor(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = gv;
        res.BitwiseXorFrom(other);
        return res;
    }

    inline VectorImpl IsEqual(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = _mm_cmpeq_epi16(gv, other.gv);
        return res;
    }

    inline VectorImpl IsGreater(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = _mm_cmpgt_epi16(gv, other.gv);
        return res;
    }
};

template<> struct VectorImpl<int, 4> :
    public detail::IntegerPack<int, __v4si, __m128i, 4>
{
    // addr must be 128-bit aligned
    inline void Set(const int* addr)
    {
        gv = _mm_load_si128(reinterpret_cast<const GeneralVT*>(addr));
    }

    inline void Set(int elem)
    {
        gv = _mm_set1_epi32(elem);
    }

    inline void AddFrom(const VectorImpl other)
    {
        gv = _mm_add_epi32(gv, other.gv);
    }

    inline void SubFrom(const VectorImpl other)
    {
        gv = _mm_sub_epi32(gv, other.gv);
    }

    // (!a) & b
    // This can be used in expression template to optimize
    inline void BitwiseAndNotFrom(const VectorImpl other)
    {
        gv = _mm_andnot_si128(gv, other.gv);
    }

    inline VectorImpl BitwiseAndNot(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = gv;
        res.BitwiseAndNotFrom(other);
        return res;
    }
};

template<> struct VectorImpl<long long, 2> :
    public detail::IntegerPack<long long, __v2di, __m128i, 2>
{
    // addr must be 128-bit aligned
    inline void Set(const long long* addr)
    {
        gv = _mm_load_si128(reinterpret_cast<const GeneralVT*>(addr));
    }

    inline void Set(long long elem)
    {
        gv = _mm_set1_epi64x(elem);
    }

    inline void AddFrom(const VectorImpl other)
    {
        gv = _mm_add_epi64(gv, other.gv);
    }

    inline void SubFrom(const VectorImpl other)
    {
        gv = _mm_sub_epi64(gv, other.gv);
    }

    // (!a) & b
    // This can be used in expression template to optimize
    inline void BitwiseAndNotFrom(const VectorImpl other)
    {
        gv = _mm_andnot_si128(gv, other.gv);
    }

    inline VectorImpl BitwiseAndNot(const VectorImpl other) const
    {
        VectorImpl res;
        res.gv = gv;
        res.BitwiseAndNotFrom(other);
        return res;
    }
};

template<> struct VectorImpl<float, 4> : public detail::Pack<float, __v4sf, __m128, 4>
{
    // addr must be 128-bit aligned
    inline void Set(const float* addr)
    {
        gv = _mm_load_ps(addr);
    }

    inline void Set(float elem)
    {
        gv = _mm_set1_ps(elem);
    }

    inline void AddFrom(const VectorImpl other)
    {
        gv = _mm_add_ps(gv, other.gv);
    }

    inline void SubFrom(const VectorImpl other)
    {
        gv = _mm_sub_ps(gv, other.gv);
    }
};

template<> struct VectorImpl<double, 2> : public detail::Pack<double, __v2df, __m128d, 2>
{
    // addr must be 128-bit aligned
    inline void Set(const double* addr)
    {
        gv = _mm_load_pd(addr);
    }

    inline void Set(double elem)
    {
        gv = _mm_set1_pd(elem);
    }

    inline void AddFrom(const VectorImpl other)
    {
        gv = _mm_add_pd(gv, other.gv);
    }

    inline void SubFrom(const VectorImpl other)
    {
        gv = _mm_sub_pd(gv, other.gv);
    }
};

}  // namespace simdple

#endif /* CPP_SIMDPLE_VECTORIMPLSSE_H_ */
