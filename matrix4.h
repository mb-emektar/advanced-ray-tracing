#ifndef MATRIX4_H
#define MATRIX4_H

#include <iostream>

#include "vector3.h"

class Matrix4
{
  public:
    // constructor&destructor
    Matrix4();
    explicit Matrix4(float val);

    // functions
    void print_info();

    // operator overloading
    float *operator[](int i);
    const float *operator[](int i) const;
    Matrix4 operator*(const Matrix4 &rhs);
    Matrix4 operator*=(const Matrix4 &rhs);
    friend ostream &operator<<(ostream &os, Matrix4 &v);

  private:
    // data
    float values[4][4];
};

Matrix4 inverse(const Matrix4 &a_matrix);
Matrix4 calculate_transpose(const Matrix4 &a_matrix);
Vector3 point_multiplication(const Matrix4 &a_matrix, Vector3 &a_point);
Vector3 vector_multiplication(const Matrix4 &a_matrix, Vector3 &vector);

#endif


