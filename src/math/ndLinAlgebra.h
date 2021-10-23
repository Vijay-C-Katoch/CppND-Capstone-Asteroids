#ifndef ND_LIN_ALGEBRA_H_
#define ND_LIN_ALGEBRA_H_

#include <cstddef>
#include <vector>
#include <cmath>
#include <initializer_list>
#include <algorithm>
#include <numeric>
#include <functional>

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

        // return norm of vector or length
        T Norm();

        // Normalize the vector in place
        void Normalize();

        // return copy copy of vector divided by norm
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

    // vector Exceptions type
    template <class T>
    using VectorException = ClassException<ndVector<T>, RuntimeException>;

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
        T sqrdSum = std::accumulate(_vector.begin(), _vector.end(), decltype(_vector)::value_type(0), [](const T& a, const T& b) 
        {
            return a + b * b;
        });

        return sqrdSum;
    }

    template <class T>
    void ndVector<T>::Normalize()
    {
        T vectorNorm = this->Norm();
        std::transform(_vector.begin(), _vector.end(), _vector.begin(), [&vectorNorm](const T& a)
        {
            return a / vectorNorm;
        });
    }

    template <class T>
    ndVector<T> ndVector<T>::Normalized()
    {
        ndVector<T> selfCopy(_vector);
        selfCopy.Normalize();
        return selfCopy;
    }

    // overloads
    template <class T>
    ndVector<T> ndVector<T>::operator+ (const ndVector<T>& rhs) const
    {
        if (_nDims != rhs._nDims)
            throw VectorException(__FILE__, __LINE__, __func__, "[Vector] : Dimensions mismatch");

        std::vector<T> resultVec(_nDims, T());
        std::transform(_vector.begin(), _vector.end(), rhs._vector.begin, resultVec.begin(), std::plus<T>());

        ndVector<T> result(resultVec);
        return result;
    }

    template <class T>
    ndVector<T> ndVector<T>::operator- (const ndVector<T>& rhs) const
    {
        if (_nDims != rhs._nDims)
            throw VectorException(__FILE__, __LINE__, __func__, "[Vector] : Dimensions mismatch");

        std::vector<T> resultVec(_nDims, T());
        std::transform(_vector.begin(), _vector.end(), rhs._vector.begin, resultVec.begin(), std::minus<T>());

        ndVector<T> result(resultVec);
        return result;
    }

    template <class T>
    ndVector<T> ndVector<T>::operator* (const ndVector<T>& rhs) const
    {
        if (_nDims != rhs._nDims)
            throw VectorException(__FILE__, __LINE__, __func__, "[Vector] : Dimensions mismatch");

        std::vector<T> resultVec(_nDims, T());
        std::transform(_vector.begin(), _vector.end(), rhs._vector.begin, resultVec.begin(), std::multiplies<T>());

        ndVector<T> result(resultVec);
        return result;
    }

    // friend (with all types)
    template <class T>
    ndVector<T> operator* (const T& lhs, const ndVector<T>& rhs)
    {
        // scalar multiplication
        std::vector<T> resultVec(rhs._nDims, T());
        std::transform(rhs._vector.begin(), rhs._vector.end(), resultVec.begin(), std::bind1st(std::multiplies<T>(), lhs));

        ndVector<T> result(resultVec);
        return result;
    }

}

#endif /* ND_LIN_ALGEBRA_H_ */