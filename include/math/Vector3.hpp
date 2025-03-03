#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <sstream>
#include <string>

class Vector3 {
public:
  double x, y, z;

  Vector3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

  Vector3 operator+(const Vector3 &v) const {
    return Vector3(x + v.x, y + v.y, z + v.z);
  };

  Vector3 operator-(const Vector3 &v) const {
    return Vector3(x - v.x, y - v.y, z - v.z);
  };

  Vector3 operator*(double scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
  };

  Vector3 operator/(double scalar) const {
    return Vector3(x / scalar, y / scalar, z / scalar);
  };

  bool operator==(const Vector3 &v) const {
    return (x == v.x) && (y == v.y) && (z == v.z);
  };

  Vector3 &operator+=(const Vector3 &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  double magnitude() const { return std::sqrt(x * x + y * y + z * z); }

  Vector3 normalized() const {
    double length = magnitude();
    return (length == 0) ? Vector3(0, 0, 0) : *this / magnitude();
  }

  float dot(const Vector3 &other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  Vector3 cross(const Vector3 &other) const {
    return Vector3(y * other.z - z * other.y, z * other.x - x * other.z,
                   x * other.y - y * other.x);
  }

  std::string toString() const {
    std::ostringstream ss;
    ss << "(" << x << ", " << y << ", " << z << ")";
    return ss.str();
  }
};

inline Vector3 operator*(double scalar, const Vector3 &v) { return v * scalar; }

#endif // !VECTOR3_H
