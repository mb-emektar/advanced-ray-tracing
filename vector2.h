#ifndef VECTOR2_H_
#define VECTOR2_H_
#include <iostream>
#include <math.h>

using namespace std;
class Vector2
{
  public:
    // data
    float x, y;

    // constructor
    Vector2();
    Vector2(float a_x, float a_y);
    Vector2(float a_k);

    // functions
    float dot_product(const Vector2 &r_v2) const;
    float length() const;
    float length_sqr() const;
    Vector2 unit_vector();
    void normalize();
    void print_axis();

    // operator overloading

    // float operations
    Vector2 operator-(float r_fl) const;
    Vector2 operator+(float r_fl) const;
    Vector2 operator/(float r_fl) const;
    Vector2 operator*(float r_fl) const;

    // compound assignment operators
    Vector2 &operator-=(const Vector2 &r_v2);
    Vector2 &operator+=(const Vector2 &r_v2);
    Vector2 &operator/=(const Vector2 &r_v2);
    Vector2 &operator*=(const Vector2 &r_v2);

    // vector operations
    Vector2 operator-(const Vector2 &r_v2) const;
    Vector2 operator+(const Vector2 &r_v2) const;
    Vector2 operator/(const Vector2 &r_v2) const;
    Vector2 operator*(const Vector2 &r_v2) const;

    bool operator==(const Vector2 &r_v2) const;
    bool operator!=(const Vector2 &r_v2) const;
    float &operator[](int i);

    // float-vector operations
    friend Vector2 operator*(float t, const Vector2 &v);
    friend Vector2 operator/(float t, const Vector2 &v);

    friend ostream &operator<<(ostream &os, const Vector2 &v);
};

#endif

