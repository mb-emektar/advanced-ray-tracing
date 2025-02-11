#ifndef VECTOR3_H_
#define VECTOR3_H_

#include <math.h>

#include <iostream>

using namespace std;

class Vector3
{
  public:
    // constructor
    Vector3();
    Vector3(float a_x, float a_y, float a_z);
    Vector3(float a_k);

    // destructor
    ~Vector3()
    {
    }

    // getters
    float get_x();
    float get_y();
    float get_z();

    // setters
    void set_x(float a_x);
    void set_y(float a_y);
    void set_z(float a_z);

    // functions
    Vector3 unit_vector();
    void normalize();
    float length() const;
    float length_sqr() const;
    float dot_product(const Vector3 &v) const;
    Vector3 cross_product(const Vector3 &v) const;
    int get_smallest_index();
    void print_axis();

    // operator overloading

    // float operations
    Vector3 operator-(float r_fl) const;
    Vector3 operator+(float r_fl) const;
    Vector3 operator/(float r_fl) const;
    Vector3 operator*(float r_fl) const;

    // compound assignment operators
    Vector3 &operator-=(const Vector3 &v);
    Vector3 &operator+=(const Vector3 &v);
    Vector3 &operator/=(const Vector3 &v);
    Vector3 &operator*=(const Vector3 &v);

    // vector operations
    Vector3 operator-(const Vector3 &v) const;
    Vector3 operator+(const Vector3 &v) const;
    Vector3 operator/(const Vector3 &v) const;
    Vector3 operator*(const Vector3 &v) const;
    bool operator==(const Vector3 &v) const;
    bool operator!=(const Vector3 &v) const;

    // float-vector operations
    friend Vector3 operator*(float t, const Vector3 &v);
    friend Vector3 operator/(float t, const Vector3 &v);

    friend ostream &operator<<(ostream &os, Vector3 &v);
    float getAvg() const;

  private:
    float x, y, z;
};

typedef Vector3 Point;

#endif


