#ifndef TRANSFORMATION_H_
#define TRANSFORMATION_H_

#include "matrix4.h"
#include "vector"

class Transformation
{
  public:
    Matrix4 &get_transformation_matrix();
    virtual void calculate_matrix() = 0;

  protected:
    Matrix4 transformation_matrix;
};

class Rotation : public Transformation
{
  public:
    float m_angle;
    Vector3 m_axis;

    Rotation();
    void calculate_matrix() override;
};

class Scaling : public Transformation
{
  public:
    Vector3 m_scaling_vector;

    Scaling();
    void calculate_matrix() override;
};

class Translation : public Transformation
{
  public:
    Vector3 m_translation_vector;

    Translation();
    void calculate_matrix() override;
};

class Composite : public Transformation
{
  public:
    Composite();
    void calculate(float *elements);
    void calculate_matrix() override;
};

#endif


