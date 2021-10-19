#ifndef ND_UTILS_H_
#define ND_UTILS_H_

#include <cstdint>
#include <cstdlib>

// utility functions
template<typename T>
bool IsInBounds(const T& value, const T& low, const T& high) {
    return !(value < low) && !(high < value);
}

inline int32_t divCeil(int32_t numerator, int32_t denominator)
{
    std::div_t res = std::div(numerator, denominator);
    return res.quot + !!res.rem;
}

#endif /* ND_UTILS_H_ */