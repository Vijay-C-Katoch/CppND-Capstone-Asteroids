#ifndef ND_UTILS_H_
#define ND_UTILS_H_

#include <cstdint>
#include <cstdlib>

// utility functions
template<typename T>
bool IsInBounds(const T& value, const T& low, const T& high) {
    return !(value < low) && !(high <= value);
}

inline int32_t divCeil(int32_t numerator, int32_t denominator)
{
    std::div_t res = std::div(numerator, denominator);

    int32_t s = 1;
    if (numerator < 0 && denominator < 0)
      s = 1;
    else if (res.quot < 0 || (res.quot == 0 && denominator < 0 && numerator > 0) || (res.quot == 0 && denominator > 0 && numerator < 0))
        s = -1;

    return res.quot + (s * (int)(!!res.rem));
}

#endif /* ND_UTILS_H_ */