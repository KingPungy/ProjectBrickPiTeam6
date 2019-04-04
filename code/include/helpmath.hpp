#ifndef HELPMATH_HPP
#define HELPMATH_HPP

// map function for every data type
template<class T>
T map(T v, T min0, T max0, T min1, T max1) {
    return min1 + (max1 - min1) * ((v - min0) / (max0 - min0));
}

#endif