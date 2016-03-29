/*************************************************************************
MGLVector.h

Full-featured 3D vector class.

Author   :   Dave Astle

Written for More OpenGL Game Programming
*************************************************************************/

#ifndef MGLVECTOR_H_INCLUDED
#define MGLVECTOR_H_INCLUDED

#include <math.h>

namespace MGL
{
  #define PI		(3.14159265359f)
  #define DEG2RAD(a)	(PI/180*(a))
  #define RAD2DEG(a)	(180/PI*(a))

  class Vector3
  {
  public:
    union
    {
      struct
      {	
        float x;
        float y;
        float z;    // x,y,z coordinates
      };
      float v[3];
    };

    // constructors
    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    Vector3(const Vector3 &vec) : x(vec.x), y(vec.y), z(vec.z) {}

    // vector set
    void Set(float nx, float ny, float nz)
    {
      x = nx; y = ny; z = nz;
    }

    // vector index
    float &operator[](const long idx)
    {
      return *((&x)+idx);
    }

    // vector assignment
    const Vector3 &operator=(const Vector3 &vec)
    {
      x = vec.x;
      y = vec.y;
      z = vec.z;
      return *this;
    }

    // vector equality
    const bool operator==(const Vector3 &vec) const
    {
      return ((x == vec.x) && (y == vec.y) && (z == vec.z));
    }

    // vector inequality
    const bool operator!=(const Vector3 &vec) const
    {
      return !(*this == vec);
    }

    // vector add
    const Vector3 operator+(const Vector3 &vec) const
    {
      return Vector3(x + vec.x, y + vec.y, z + vec.z);
    }

    // vector add (opposite of negation)
    const Vector3 operator+() const
    {
      return Vector3(*this);
    }

    // vector increment
    const Vector3& operator+=(const Vector3& vec)
    {
      x += vec.x;
      y += vec.y;
      z += vec.z;
      return *this;
    }

    // vector subtraction
    const Vector3 operator-(const Vector3& vec) const
    {
      return Vector3(x - vec.x, y - vec.y, z - vec.z);
    }

    // vector negation
    const Vector3 operator-() const
    {
      return Vector3(-x, -y, -z);
    }

    // vector decrement
    const Vector3 &operator-=(const Vector3& vec)
    {
      x -= vec.x;
      y -= vec.y;
      z -= vec.z;
      return *this;
    }

    // scalar self-multiply
    const Vector3 &operator*=(const float s)
    {
      x *= s;
      y *= s;
      z *= s;
      return *this;
    }

    // scalar self-divide
    const Vector3 &operator/=(const float s)
    {
      const float recip = 1.0f/s; // for speed, one division
      x *= recip;
      y *= recip;
      z *= recip;
      return *this;
    }

    // post multiply by scalar
    const Vector3 operator*(const float s) const
    {
      return Vector3(x*s, y*s, z*s);
    }

    // pre multiply by scalar
    friend const Vector3 operator*(const float s, const Vector3& vec)
    {
      return vec*s;
    }

    // divide by scalar
    const Vector3 operator/(const float s) const
    {
      const float recip = 1/s;
      return Vector3(recip*x, recip*y, recip*z);
    }

    // vector multiply
    const Vector3 operator*(const Vector3& vec) const
    {
      return Vector3(x*vec.x, y*vec.y, z*vec.z);
    }

    // cross product
    const Vector3 CrossProduct(const Vector3 &vec) const
    {
      return Vector3(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
    }

    // dot product
    const float DotProduct(const Vector3 &vec) const
    {
      return x*vec.x + y*vec.x + z*vec.z;
    }

    // length of vector
    const float Length() const
    {
      return sqrtf(x*x + y*y + z*z);
    }

    // return the unit vector
    const Vector3 UnitVector() const
    {
      return (*this) / Length();
    }

    // normalize this vector
    void Normalize()
    {
      (*this) /= Length();
    }

    // return angle between two vectors
    const float inline Angle(const Vector3& vec) const
    {
      return acosf(DotProduct(vec));
    }

    // reflect this vector off surface with normal vector
    const Vector3 Reflection(const Vector3& normal) const
    {
      return (*this - normal * 2.0 * DotProduct(normal));
    }

    // rotate angle degrees about a normal
    const Vector3 Rotate(const float angle, const Vector3& normal) const
    {	
      const float cosine = cosf(angle);
      const float sine = sinf(angle);

      return Vector3(*this * cosine + ((normal * *this) * (1.0f - cosine)) *
        normal + (CrossProduct(normal)) * sine);
    }

  };
}

#endif