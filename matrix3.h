#ifndef MATRIX3_H_
#define MATRIX3_H_

#include "vector3.h"
#include <iostream>
#include <vector>

class Matrix3
{
  public:
    // constructor&destructor
    Matrix3();
    Matrix3(float val);
    Matrix3(vector<Vector3> cols);

    // functions
    Matrix3 operator*(const Matrix3 &rhs) const;
    Vector3 get_col(int i) const;
    void print_info() const;

    float *operator[](int i);
    const float *operator[](int i) const;

  private:
    // data
    float values[3][3];
};

Vector3 vector_multiplication(const Matrix3 &a_matrix, Vector3 &vector);
Matrix3 transpose(Matrix3 a_matrix);

#endif


