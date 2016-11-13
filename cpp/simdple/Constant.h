#ifndef CPP_SIMDPLE_CONSTANT_H_
#define CPP_SIMDPLE_CONSTANT_H_

namespace simdple {

static const unsigned int MAX_BIT_NUM =
#if defined(__AVX__)
        256;
#else
        128;
#endif

static const unsigned int MAX_BYTE_NUM = MAX_BIT_NUM / 8;

}  // namespace simdple


#endif /* CPP_SIMDPLE_CONSTANT_H_ */
