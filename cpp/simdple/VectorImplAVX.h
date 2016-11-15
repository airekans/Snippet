#ifndef CPP_SIMDPLE_VECTORIMPLAVX_H_
#define CPP_SIMDPLE_VECTORIMPLAVX_H_

#include "simdple/detail/Pack.h"

namespace simdple {

#ifdef __AVX__

template<> struct VectorImpl<char, 32> :
    public detail::IntegerPack<char, __v32qi, __m256i, 32>
{
    // addr must be 256-bit aligned
    inline void Set(const char* addr)
    {
        gv = _mm256_load_si256(reinterpret_cast<const GeneralVT*>(addr));
    }

    inline void Set(char elem)
    {
        gv = _mm256_set1_epi8(elem);
    }

#ifdef __AVX2__
    inline void AddFrom(const VectorImpl other)
    {
        gv = _mm256_add_epi8(gv, other.gv);
    }

    inline void SubFrom(const VectorImpl other)
    {
        gv = _mm256_sub_epi8(gv, other.gv);
    }

#endif // __AVX2__
};

template<> struct VectorImpl<short, 16> :
    public detail::IntegerPack<short, __v16hi, __m256i, 16>
{

};

template<> struct VectorImpl<int, 8> :
    public detail::IntegerPack<int, __v8si, __m256i, 8>
{

};

template<> struct VectorImpl<long long, 4> :
    public detail::IntegerPack<long long, __v4di, __m256i, 4>
{

};


#endif  // __AVX__

}  // namespace simdple


#endif /* CPP_SIMDPLE_VECTORIMPLAVX_H_ */
