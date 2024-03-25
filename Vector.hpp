#pragma once
#include <cmath>
#include <iostream>
#include <sstream>

/*!
 * Class describing a 3 dimensional vector 
 */

// 3-D vector
class Vec
{
    double _x;
    double _y;
    double _z;

public:
    Vec() : _x{0.0}, _y{0.0}, _z{0.0} {}
    Vec(double x, double y, double z) 
    : _x{x}, _y{y}, _z{z} {}

    double x() const { return _x; }
    double y() const { return _y; }
    double z() const { return _z; }

    double norm() const { return sqrt(_x*_x + _y*_y + _z*_z); }
    double norm2() const { return _x*_x + _y*_y + _z*_z; }
    double norm3() const { double r = sqrt(_x*_x + _y*_y + _z*_z); return r*r*r; }

    Vec operator-() const { 
        return *this * -1 ;
    }

    // operations between two vectors
    Vec operator+(const Vec& v) const {
        return Vec(_x + v._x, _y + v._y, _z + v._z);
    }

    Vec operator-(const Vec& v) const {
        return Vec(_x - v._x, _y - v._y, _z - v._z);
    }

    // operations between a vector and a scalar
    Vec operator*(double s) const {
        return Vec(_x * s, _y * s, _z * s);
    }

    friend Vec operator*(double s, const Vec& a) {
        return a * s;
    }

    Vec operator/(double s) const {
        return Vec(_x / s, _y / s, _z / s);
    }

    /*
     *  Operations that mutate the object
     */

    Vec& operator+=(Vec v) {
        _x += v._x;
        _y += v._y;
        _z += v._z;
        return *this;
    }

    Vec& operator-=(Vec v) {
        _x -= v._x;
        _y -= v._y;
        _z -= v._z;
        return *this;
    }

    Vec& operator*=(double s) {
        _x *= s;
        _y *= s;
        _z *= s;
        return *this;
    }

    Vec& operator/=(double s) {
        _x /= s;
        _y /= s;
        _z /= s;
        return *this;
    }

    /*
     *  I/O
     */

    std::string to_row() const {
        std::stringstream out;
        out << _x << ", " << _y << ", " << _z;
        return out.str();
    }

    friend std::ostream& operator<<(std::ostream& out, const Vec& v){
        return out << '(' <<  v.to_row() << ')';
    }

    /*
     * Exact comparison (for testing purposes)
     */
    bool operator==(const Vec& v) const {
        return _x == v._x and _y == v._y and _z == v._z;
    }

    bool operator!=(const Vec& v) const {
        return not (*this == v);
    }
};