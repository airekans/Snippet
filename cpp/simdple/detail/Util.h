#ifndef CPP_SIMDPLE_DETAIL_UTIL_H_
#define CPP_SIMDPLE_DETAIL_UTIL_H_


namespace simdple {
namespace detail {

template<bool> struct StaticAssert;
template<> struct StaticAssert<true> { typedef int Type; };

template<unsigned int N>
struct AssertIsPowerOf2
{
    typedef typename StaticAssert<(N > 0) && ((N & (N - 1)) == 0)>::Type Type;
};

template<typename T> struct IsInteger { enum { Result = 0 }; };
template<> struct IsInteger<char> { enum { Result = 1 }; };
template<> struct IsInteger<unsigned char> { enum { Result = 1 }; };
template<> struct IsInteger<short> { enum { Result = 1 }; };
template<> struct IsInteger<unsigned short> { enum { Result = 1 }; };
template<> struct IsInteger<int> { enum { Result = 1 }; };
template<> struct IsInteger<unsigned int> { enum { Result = 1 }; };
template<> struct IsInteger<long int> { enum { Result = 1 }; };
template<> struct IsInteger<unsigned long int> { enum { Result = 1 }; };
template<> struct IsInteger<long long> { enum { Result = 1 }; };
template<> struct IsInteger<unsigned long long> { enum { Result = 1 }; };

}  // namespace detail
}  // namespace simdple


#endif /* CPP_SIMDPLE_DETAIL_UTIL_H_ */
