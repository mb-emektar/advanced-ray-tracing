#include "matrix4.h"
#include <algorithm> // For swap
#include <cmath>     // For abs

Matrix4::Matrix4()
{
    fill(values[0], values[0] + 16, 0);
}

Matrix4::Matrix4(float val)
{
    fill(values[0], values[0] + 16, 0);

    for (int i = 0; i < 4; i++)
    {
        values[i][i] = val;
    }
}

Matrix4 Matrix4::operator*(const Matrix4 &a_matrix)
{
    Matrix4 out_matrix = Matrix4();

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                out_matrix[i][j] += values[i][k] * a_matrix[k][j];
            }
        }
    }

    return out_matrix;
}

Matrix4 Matrix4::operator*=(const Matrix4 &rhs)
{
    float out_matrix[4][4];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            out_matrix[i][j] = 0;
            for (int k = 0; k < 4; k++)
            {
                out_matrix[i][j] += values[i][k] * rhs[k][j];
            }
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            values[i][j] = out_matrix[i][j];
        }
    }

    return *this;
}

float *Matrix4::operator[](int i)
{
    return values[i];
}

const float *Matrix4::operator[](int i) const
{
    return values[i];
}

Matrix4 calculate_transpose(const Matrix4 &a_matrix)
{
    Matrix4 out_matrix = Matrix4();

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            out_matrix[i][j] = a_matrix[j][i];
        }
    }

    return out_matrix;
}

Matrix4 inverse(const Matrix4 &a_matrix)
{
    Matrix4 inv = Matrix4();
    float augmented[4][8];

    // create an augmented matrix [A | I]
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            augmented[i][j] = a_matrix[i][j];
            augmented[i][j + 4] = (i == j) ? 1 : 0;
        }
    }

    // perform Gauss-Jordan elimination
    for (int i = 0; i < 4; ++i)
    {
        // find the pivot element
        int pivot = i;
        for (int j = i + 1; j < 4; ++j)
        {
            if (abs(augmented[j][i]) > abs(augmented[pivot][i]))
            {
                pivot = j;
            }
        }

        // swap rows to move the pivot element to the current row
        if (i != pivot)
        {
            swap_ranges(augmented[i], augmented[i] + 8, augmented[pivot]);
        }

        // check for singular matrix
        if (augmented[i][i] == float(0))
        {
            return inv;
        }

        // scale the pivot row
        float pivotVal = augmented[i][i];
        for (int j = 0; j < 8; ++j)
        {
            augmented[i][j] /= pivotVal;
        }

        // eliminate the current column in other rows
        for (int j = 0; j < 4; ++j)
        {
            if (i != j)
            {
                float factor = augmented[j][i];
                for (int k = 0; k < 8; ++k)
                {
                    augmented[j][k] -= factor * augmented[i][k];
                }
            }
        }
    }

    // extract the inverse matrix from the augmented matrix
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            inv[i][j] = augmented[i][j + 4];
        }
    }

    return inv;
}

Vector3 point_multiplication(const Matrix4 &a_matrix, Vector3 &a_point)
{
    const float p[4] = {a_point.get_x(), a_point.get_y(), a_point.get_z(), 1};
    float res[4] = {0};

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            res[i] += a_matrix[i][j] * p[j];
        }
    }

    Vector3 out_vector = Vector3();
    out_vector.set_x(res[0]);
    out_vector.set_y(res[1]);
    out_vector.set_z(res[2]);
    return out_vector;
}

Vector3 vector_multiplication(const Matrix4 &a_matrix, Vector3 &vector)
{
    Vector3 out_vector = Vector3();

    float res[3] = {out_vector.get_x(), out_vector.get_y(), out_vector.get_z()};
    float vec[3] = {vector.get_x(), vector.get_y(), vector.get_z()};

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            res[i] += a_matrix[i][j] * vec[j];
        }
    }
    out_vector.set_x(res[0]);
    out_vector.set_y(res[1]);
    out_vector.set_z(res[2]);
    return out_vector;
}

ostream &operator<<(ostream &os, Matrix4 &v)
{
    os << "Matrix4 = " << "  \n";
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            os << v[i][j] << "  ";
        }
        os << "  \n";
    }
    return os;
}

void Matrix4::print_info()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << values[i][j] << "  ";
        }
        cout << endl;
    }
}

