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

    inline static Impl Load(T elem)
    {
        Impl res;
        res.Set(elem);
        return res;
    }

    inline static Impl Load(const T* addr)
    {
        Impl res;
        res.Set(addr);
        return res;
    }

    inline Impl Add(const Impl other) const
    {
        Impl res;
        res.gv = gv;
        res.AddFrom(other);
        return res;
    }

    inline Impl Sub(const Impl other) const
    {
        Impl res;
        res.gv = gv;
        res.SubFrom(other);
        return res;
    }
};

template<typename T, typename VT, typename GVT, unsigned int N>
struct IntegerPack : public Pack<T, VT, GVT, N>
{
    typedef typename Pack<T, VT, GVT, N>::Impl Impl;

    inline Impl BitwiseAndNot(const Impl other) const
    {
        Impl res;
        res.gv = this->gv;
        res.BitwiseAndNotFrom(other);
        return res;
    }

    inline Impl BitwiseXor(const Impl other) const
    {
        Impl res;
        res.gv = this->gv;
        res.BitwiseXorFrom(other);
        return res;
    }
};

}  // namespace detail
}  // namespace simdple

#endif /* CPP_SIMDPLE_DETAIL_PACK_H_ */
