#ifndef ND_LIN_ALGEBRA_H_
#define ND_LIN_ALGEBRA_H_

#include <cstddef>
#include <vector>
#include <cmath>
#include <initializer_list>

#include "ndException.h"

namespace nd
{

    /// N dimension vector
    template <class T>
    class ndVector
    {
        ndVector() = default;

        ndVector(std::size_t numDims);

        ndVector(std::initializer_list<T> inList);

        ndVector(std::vector<T> inVector);

        ~ndVector();

        std::size_t NumDims() const;

        void SetAtIndex(std::size_t idx, T val);

        T GetAtIndex(std::size_t idx);

        T Length();

        void Normalize();

        ndVector<T>GetNormalized();

        // overloads
        ndVector<T> operator+ (const ndVector<T>& rhs) const;
        ndVector<T> operator- (const ndVector<T>& rhs) const;
        ndVector<T> operator* (const ndVector<T>& rhs) const;

        // friend
        template <class T2> friend ndVector<T2> operator* (const T2& lhs, const ndVector<T2>& rhs );

        //logical grouping
        // dot product, projection of one vector over other
        static T dot(const ndVector<T>& v1, const ndVector<T>& v2);
        //cross product, identify plane of vectors
        static ndVector<T> cross(const ndVector<T>& v1, const ndVector<T>& v2);




    private:
        std::vector<T> _vector;
        std::size_t _nDims;

    };
}

#endif /* ND_LIN_ALGEBRA_H_ */