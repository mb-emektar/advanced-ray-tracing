#include "transformation.h"

Matrix4 &Transformation::get_transformation_matrix()
{
    return transformation_matrix;
}

Rotation::Rotation()
{
    m_angle = 0;
    m_axis = Vector3();
}
void Rotation::calculate_matrix()
{
    Vector3 axis_unit_vector = m_axis;
    axis_unit_vector.normalize();
    float unit_v[3] = {axis_unit_vector.get_x(), axis_unit_vector.get_y(), axis_unit_vector.get_z()};

    float cosine_angle, sinus_angle, one_minus_cosine_angle;
    cosine_angle = cos(m_angle);
    sinus_angle = sin(m_angle);
    one_minus_cosine_angle = 1 - cosine_angle;

    transformation_matrix[3][3] = 1;
    transformation_matrix[2][2] = cosine_angle + pow(unit_v[2], 2) * one_minus_cosine_angle;
    transformation_matrix[2][1] = unit_v[1] * unit_v[2] * one_minus_cosine_angle + unit_v[0] * sinus_angle;
    transformation_matrix[2][0] = unit_v[0] * unit_v[2] * one_minus_cosine_angle - unit_v[1] * sinus_angle;
    transformation_matrix[1][2] = unit_v[1] * unit_v[2] * one_minus_cosine_angle - unit_v[0] * sinus_angle;
    transformation_matrix[1][1] = cosine_angle + pow(unit_v[1], 2) * one_minus_cosine_angle;
    transformation_matrix[1][0] = unit_v[0] * unit_v[1] * one_minus_cosine_angle + unit_v[2] * sinus_angle;
    transformation_matrix[0][2] = unit_v[0] * unit_v[2] * one_minus_cosine_angle + unit_v[1] * sinus_angle;
    transformation_matrix[0][1] = unit_v[0] * unit_v[1] * one_minus_cosine_angle - unit_v[2] * sinus_angle;
    transformation_matrix[0][0] = cosine_angle + pow(unit_v[0], 2) * one_minus_cosine_angle;
}

Scaling::Scaling()
{
    m_scaling_vector = Vector3();
}
void Scaling::calculate_matrix()
{
    transformation_matrix[3][3] = 1;
    transformation_matrix[2][2] = m_scaling_vector.get_z();
    transformation_matrix[1][1] = m_scaling_vector.get_y();
    transformation_matrix[0][0] = m_scaling_vector.get_x();
}

Translation::Translation()
{
    transformation_matrix = Matrix4();
}
void Translation::calculate_matrix()
{
    transformation_matrix = Matrix4(1);
    transformation_matrix[2][3] = m_translation_vector.get_z();
    transformation_matrix[1][3] = m_translation_vector.get_y();
    transformation_matrix[0][3] = m_translation_vector.get_x();
}
Composite::Composite()
{
    transformation_matrix = Matrix4();
}
void Composite::calculate_matrix()
{
}
void Composite::calculate(float *elements)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            transformation_matrix[i][j] = elements[(i * 4) + j];
}

