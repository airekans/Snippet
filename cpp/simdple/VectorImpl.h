#ifndef CPP_SIMDPLE_VECTORIMPL_H_
#define CPP_SIMDPLE_VECTORIMPL_H_

#include "simdple/detail/Pack.h"
#include "simdple/VectorImplSSE.h"

namespace simdple {

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
    return lhs.BitwiseAnd(rhs);
}

template<typename T, unsigned int N>
inline VectorImpl<T, N> operator^(const VectorImpl<T, N> lhs, const VectorImpl<T, N> rhs)
{
    return lhs.BitwiseXor(rhs);
}

}  // namespace simdple


#endif /* CPP_SIMDPLE_VECTORIMPL_H_ */
