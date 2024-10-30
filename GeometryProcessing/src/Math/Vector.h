#pragma once
#include "GeometryProcessing.h"
#include <Eigen/Core>
#include <Eigen/Eigen>

template<typename T, size_t N>
class GEO_API TVector : public Eigen::Vector<T, N>
{
public:
    using Eigen::Vector<T, N>::Vector; // Inherit constructor

    /*float operator|(const TVector<T, N>& other)
    {
        return this->dot(other);
    }*/

    /*
    float operator^(const TVector<T, N>& other)
    {
        if (N == 3) return this->cross(other);
        return this->cross3(other);
    }*/
};

using Vector4 = Eigen::Vector<float, 4>;
using Vector3 = Eigen::Vector<float, 3>;
using Vector2 = Eigen::Vector<float, 2>;

using Vector4i = Eigen::Vector<int, 4>;
using Vector3i = Eigen::Vector<int, 3>;
using Vector2i = Eigen::Vector<int, 2>;