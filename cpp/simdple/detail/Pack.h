#ifndef CPP_SIMDPLE_DETAIL_PACK_H_
#define CPP_SIMDPLE_DETAIL_PACK_H_

#include <string.h>

#include "simdple/detail/Util.h"

namespace simdple {

template<typename T, unsigned int N> struct VectorImpl;

namespace detail {

template<typename T, typename VT, typename GVT, unsigned int N>
struct Pack
{
    typedef T ElemType;
    typedef VT VType;
    typedef GVT GeneralVT;
    enum { ElemNum = N };
    enum { SIZE = N * sizeof(T) };
    typedef VectorImpl<T, N> Impl;
    typedef typename StaticAssert<(sizeof(VT) / sizeof(T)) == N>::Type _ASSERT;
    typedef typename StaticAssert<sizeof(VT) == sizeof(GVT)>::Type _ASSERT2;
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

    inline static size_t Size() { return SIZE; }

//    inline static Impl Load(T elem)
//    {
//        Impl res;
//        res.Set(elem);
//        return res;
//    }
//
//    inline static Impl Load(const T* addr)
//    {
//        Impl res;
//        res.Set(addr);
//        return res;
//    }
};

}  // namespace detail
}  // namespace simdple

#endif /* CPP_SIMDPLE_DETAIL_PACK_H_ */
