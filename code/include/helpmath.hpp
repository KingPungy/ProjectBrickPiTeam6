#ifndef HELPMATH_HPP
#define HELPMATH_HPP

// map function for every data type that supports basic math
template<class T>
T map(T v, T min0, T max0, T min1, T max1) {
    return min1 + (max1 - min1) * ((v - min0) / (max0 - min0));
}

template<class T>
T lerp(T a, T b, T f) {
    return (a * (1.0 - f)) + (b * f);
}

#endif