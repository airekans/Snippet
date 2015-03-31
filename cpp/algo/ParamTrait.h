#ifndef ALGO_PARAMTRAIT_H_
#define ALGO_PARAMTRAIT_H_

namespace snippet {
namespace algo {

template<typename T>
struct ParamTrait
{
    typedef T& DeclType;
    typedef T DefType;
};

template<typename T>
struct ParamTrait<T*>
{
    typedef T* DeclType;
    typedef T* DefType;
};

template<typename T>
struct ParamTrait<T&>
{
    typedef T& DeclType;
    typedef T DefType;
};


#define ALGO_NATIVE_PARAMTRAIT(type) \
template<> \
struct ParamTrait<type> \
{ \
    typedef type DeclType; \
    typedef type DefType; \
}

ALGO_NATIVE_PARAMTRAIT(bool);
ALGO_NATIVE_PARAMTRAIT(char);
ALGO_NATIVE_PARAMTRAIT(unsigned char);
ALGO_NATIVE_PARAMTRAIT(short);
ALGO_NATIVE_PARAMTRAIT(unsigned short);
ALGO_NATIVE_PARAMTRAIT(int);
ALGO_NATIVE_PARAMTRAIT(unsigned int);
ALGO_NATIVE_PARAMTRAIT(long);
ALGO_NATIVE_PARAMTRAIT(unsigned long);
ALGO_NATIVE_PARAMTRAIT(long long);
ALGO_NATIVE_PARAMTRAIT(unsigned long long);
ALGO_NATIVE_PARAMTRAIT(float);
ALGO_NATIVE_PARAMTRAIT(double);


}  // namespace algo
}  // namespace snippet



#endif /* ALGO_PARAMTRAIT_H_ */
