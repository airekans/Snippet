#ifndef CPP_SIMDPLE_VECTOR_H_
#define CPP_SIMDPLE_VECTOR_H_

#include "simdple/Constant.h"
#include "simdple/VectorImpl.h"

namespace simdple {
namespace detail {

template<typename Vec, typename ImplT>
struct VectorExpression
{
    typedef ImplT Impl;
    typedef typename Impl::ElemType ElemType;

    inline Impl Eval() const { return static_cast<const Vec*>(this)->Eval(); }
    inline ElemType operator[](size_t i) const { return Eval()[i]; }
};

}  // namespace simdple


template<typename T, unsigned int N = MAX_BYTE_NUM / sizeof(T)>
struct Vector : public detail::VectorExpression<Vector<T, N>, VectorImpl<T, N> >
{
    typedef VectorImpl<T, N> Impl;

    inline explicit Vector(T elem) : v(Impl::Load(elem)) {}

    template<typename Vec>
    inline explicit Vector(const detail::VectorExpression<Vec, Impl>& e)
    : v(e.Eval())
    {}

    template<typename Vec>
    inline Vector& operator=(const detail::VectorExpression<Vec, Impl>& e)
    {
        v = e.Eval();
        return *this;
    }

    inline Impl Eval() const { return v; }

    Impl v;
};

namespace detail {

template<typename LVec, typename RVec>
struct AddVector : public detail::VectorExpression<AddVector<LVec, RVec>, typename LVec::Impl>
{
    typedef typename LVec::Impl Impl;
    AddVector(const LVec& lhs, const RVec& rhs) : _lhs(lhs), _rhs(rhs) {}
    inline Impl Eval() const { return _lhs.Eval() + _rhs.Eval(); }

    const LVec& _lhs;
    const RVec& _rhs;
};

template<typename LVec, typename RVec>
struct SubVector : public detail::VectorExpression<SubVector<LVec, RVec>, typename LVec::Impl>
{
    typedef typename LVec::Impl Impl;
    SubVector(const LVec& lhs, const RVec& rhs) : _lhs(lhs), _rhs(rhs) {}
    inline Impl Eval() const { return _lhs.Eval() - _rhs.Eval(); }

    const LVec& _lhs;
    const RVec& _rhs;
};

template<typename Vec>
struct BitwiseNotVector : public detail::VectorExpression<BitwiseNotVector<Vec>, typename Vec::Impl>
{
    typedef typename Vec::Impl Impl;
    explicit BitwiseNotVector(const Vec& tmp) : v(tmp) {}
    inline Impl Eval() const { return v.Eval().BitwiseNot(); }

    const Vec& v;
};

template<typename LVec, typename RVec>
struct BitwiseAndVector :
        public detail::VectorExpression<BitwiseAndVector<LVec, RVec>, typename LVec::Impl>
{
    typedef typename LVec::Impl Impl;
    BitwiseAndVector(const LVec& lhs, const RVec& rhs) : _lhs(lhs), _rhs(rhs) {}
    inline Impl Eval() const { return _lhs.Eval().BitwiseAnd(_rhs.Eval()); }

    const LVec& _lhs;
    const RVec& _rhs;
};

template<typename LVec, typename RVec>
struct BitwiseAndVector<BitwiseNotVector<LVec>, BitwiseNotVector<RVec> > :
        public detail::VectorExpression<
            BitwiseAndVector<BitwiseNotVector<LVec>, BitwiseNotVector<RVec> >,
            typename LVec::Impl>
{
    typedef typename LVec::Impl Impl;
    typedef BitwiseNotVector<LVec> LNotVec;
    typedef BitwiseNotVector<RVec> RNotVec;
    BitwiseAndVector(const LNotVec& lhs, const RNotVec& rhs) : _lhs(lhs), _rhs(rhs) {}
    inline Impl Eval() const { return _lhs.v.Eval().BitwiseAndNot(_rhs.Eval()); }

    const LNotVec& _lhs;
    const RNotVec& _rhs;
};

template<typename LVec, typename RVec>
struct BitwiseAndVector<BitwiseNotVector<LVec>, RVec> :
        public detail::VectorExpression<
            BitwiseAndVector<BitwiseNotVector<LVec>, RVec>,
            typename LVec::Impl>
{
    typedef typename LVec::Impl Impl;
    typedef BitwiseNotVector<LVec> LNotVec;
    BitwiseAndVector(const LNotVec& lhs, const RVec& rhs) : _lhs(lhs), _rhs(rhs) {}
    inline Impl Eval() const { return _lhs.v.Eval().BitwiseAndNot(_rhs.Eval()); }

    const LNotVec& _lhs;
    const RVec& _rhs;
};

template<typename LVec, typename RVec>
struct BitwiseAndVector<LVec, BitwiseNotVector<RVec> > :
        public detail::VectorExpression<
            BitwiseAndVector<LVec, BitwiseNotVector<RVec> >,
            typename LVec::Impl>
{
    typedef typename LVec::Impl Impl;
    typedef BitwiseNotVector<RVec> RNotVec;
    BitwiseAndVector(const LVec& lhs, const RNotVec& rhs) : _lhs(lhs), _rhs(rhs) {}
    inline Impl Eval() const { return _rhs.v.Eval().BitwiseAndNot(_lhs.Eval()); }

    const LVec& _lhs;
    const RNotVec& _rhs;
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

template<typename LVec, typename RVec, typename Impl>
inline detail::SubVector<detail::VectorExpression<LVec, Impl>,
    detail::VectorExpression<RVec, Impl> > operator-(
        const detail::VectorExpression<LVec, Impl>& lhs,
        const detail::VectorExpression<RVec, Impl>& rhs)
{
    return detail::SubVector<detail::VectorExpression<LVec, Impl>,
            detail::VectorExpression<RVec, Impl> >(lhs, rhs);
}

namespace op {

template<typename Vec, typename Impl>
inline detail::BitwiseNotVector<detail::VectorExpression<Vec, Impl> >
    BitwiseNot(const detail::VectorExpression<Vec, Impl>& v)
{
    return detail::BitwiseNotVector<detail::VectorExpression<Vec, Impl> >(v);
}

}  // namespace op

template<typename LVec, typename RVec>
inline detail::BitwiseAndVector<LVec, RVec> operator&(const LVec& lhs, const RVec& rhs)
{
    return detail::BitwiseAndVector<LVec, RVec>(lhs, rhs);
}

}  // namespace simdple


#endif /* CPP_SIMDPLE_VECTOR_H_ */
