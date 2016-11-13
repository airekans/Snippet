#ifndef CPP_SIMDPLE_VECTORIMPLAVX_H_
#define CPP_SIMDPLE_VECTORIMPLAVX_H_

#include "simdple/detail/Pack.h"

namespace simdple {

#ifdef __AVX__

template<> struct VectorImpl<char, 32> :
    public detail::IntegerPack<char, __v32qi, __m256i, 32>
{

};

template<> struct VectorImpl<short, 16> :
    public detail::IntegerPack<short, __v16hi, __m256i, 16>
{

};

#endif  // __AVX__

}  // namespace simdple


#endif /* CPP_SIMDPLE_VECTORIMPLAVX_H_ */
