#include "matrix3.h"

Matrix3::Matrix3()
{
    fill(values[0], values[0] + 9, 0);
}

Matrix3::Matrix3(float val)
{
    fill(values[0], values[0] + 9, 0);

    for (int i = 0; i < 3; i++)
    {
        values[i][i] = val;
    }
}

Matrix3::Matrix3(vector<Vector3> cols)
{
    for (int i = 0; i < 3; i++)
    {
        values[0][i] = cols[i].get_x();
        values[1][i] = cols[i].get_y();
        values[2][i] = cols[i].get_z();
    }
}

Matrix3 Matrix3::operator*(const Matrix3 &rhs) const
{
    Matrix3 result = Matrix3();

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int m = 0; m < 3; m++)
            {
                result[i][j] += values[i][m] * rhs[m][j];
            }
        }
    }

    return result;
}

Vector3 Matrix3::get_col(int i) const
{
    Vector3 output;
    output.set_x(values[0][i]);
    output.set_y(values[1][i]);
    output.set_z(values[2][i]);
    return output;
}

void Matrix3::print_info() const
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << values[i][j] << "  ";
        }
        cout << endl;
    }
}

float *Matrix3::operator[](int i)
{
    return values[i];
}

const float *Matrix3::operator[](int i) const
{
    return values[i];
}

Vector3 vector_multiplication(const Matrix3 &a_matrix, Vector3 &vector)
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
Matrix3 transpose(Matrix3 a_matrix)
{
    Matrix3 result = Matrix3();

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (i == 0)
                result[i][j] = a_matrix.get_col(j).get_x();
            else if (i == 1)
                result[i][j] = a_matrix.get_col(j).get_y();
            else if (i == 2)
                result[i][j] = a_matrix.get_col(j).get_z();
        }
    }

    return result;
}

