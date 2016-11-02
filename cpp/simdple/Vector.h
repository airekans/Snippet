#ifndef CPP_SIMDPLE_VECTOR_H_
#define CPP_SIMDPLE_VECTOR_H_

#include "simdple/VectorImpl.h"

namespace simdple {
namespace detail {

template<typename Vec, typename ImplT>
struct VectorExpression
{
    typedef ImplT Impl;
    Impl Eval() const { return static_cast<const Vec*>(this)->Eval(); }
};

}  // namespace simdple


template<typename T, unsigned int N>
struct Vector : public detail::VectorExpression<Vector<T, N>, VectorImpl<T, N> >
{
    typedef VectorImpl<T, N> Impl;

    explicit Vector(T elem) : v(Impl::Load(elem)) {}

    template<typename Vec>
    explicit Vector(const detail::VectorExpression<Vec, Impl>& e) : v(e.Eval()) {}

    template<typename Vec>
    Vector& operator=(const detail::VectorExpression<Vec, Impl>& e)
    {
        v = e.Eval();
        return *this;
    }

    Impl Eval() const { return v; }

    Impl v;
};

namespace detail {

template<typename LVec, typename RVec>
struct AddVector : public detail::VectorExpression<AddVector<LVec, RVec>, typename LVec::Impl>
{
    typedef typename LVec::Impl Impl;
    explicit AddVector(const LVec& lhs, const RVec& rhs) : _lhs(lhs), _rhs(rhs) {}
    Impl Eval() const { return _lhs.Eval() + _rhs.Eval(); }

    const LVec& _lhs;
    const RVec& _rhs;
};

}  // namespace detail

template<typename LVec, typename RVec, typename Impl>
inline detail::AddVector<detail::VectorExpression<LVec, Impl>,
    detail::VectorExpression<RVec, Impl> > operator+(
        const detail::VectorExpression<LVec, Impl>& lhs,
        const detail::VectorExpression<RVec, Impl>& rhs)
{
    return detail::AddVector<detail::VectorExpression<LVec, Impl>,
            detail::VectorExpression<RVec, Impl> >(lhs, rhs);
}

}  // namespace simdple


#endif /* CPP_SIMDPLE_VECTOR_H_ */
