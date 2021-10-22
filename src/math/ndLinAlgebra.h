#ifndef ND_LIN_ALGEBRA_H_
#define ND_LIN_ALGEBRA_H_

#include <cstddef>
#include <vector>
#include <cmath>
#include <initializer_list>
#include <algorithm>
#include <numeric>

#include "ndException.h"

namespace nd
{

    /* ****************************************************************
    *  Vectors 
    /* ****************************************************************/
    /// N dimension vector
    template <class T>
    class ndVector
    {
        ndVector() = default;

        ndVector(std::size_t numDims);

        ndVector(std::initializer_list<T> inList);

        ndVector(std::vector<T> inVector);

        ~ndVector() = default;

        std::size_t NumDims() const;

        void SetAtIndex(std::size_t idx, T val);

        T GetAtIndex(std::size_t idx);

        T Norm();

        void Normalize();

        ndVector<T>Normalized();

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

    template <class T>
    ndVector<T>::ndVector(std::size_t numDims) : _nDims{numDims}
    {
        _vector = std::vector<T>(numDims, static_cast<T>(0));
    }

    template <class T>
    ndVector<T>::ndVector(std::initializer_list<T> inList)
    {
        const T* it = inList.begin();
        const T* const end = inList.end();

        for (; it != end; ++it)
            _vector.push_back(*it);
    }

    template <class T>
    ndVector<T>::ndVector(std::vector<T> inVector)
    {
        _nDims = inVector.size();
        _vector = inVector;
    }

    template <class T>
    std::size_t ndVector<T>::NumDims() const
    {
        return _nDims;
    }

    template <class T>
    void ndVector<T>::SetAtIndex(std::size_t idx, T val)
    {
        _vector[idx] = val;
    }

    template <class T>
    T ndVector<T>::GetAtIndex(std::size_t idx)
    {
        return _vector[idx];
    }

    template <class T>
    T ndVector<T>::Norm()
    {
        T sqrdSum = std::accumulate(_vector.begin(), _vector.end(), decltype(_vector)::value_type(0), [](T a, T b) 
        {
            return a + b * b;
        });

    }

}

#endif /* ND_LIN_ALGEBRA_H_ */