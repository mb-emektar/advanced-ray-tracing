#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "matrix4.h"
#include "ray.h"
#include "vector3.h"
#include <cfloat>

class BoundingBox
{
  public:
    Vector3 centroid;
    Vector3 diagonal;
    Vector3 min_point;
    Vector3 max_point;

    // constructor&destructor
    BoundingBox();
    BoundingBox(Vector3 a_min_point, Vector3 a_max_point);
    ~BoundingBox(){};

    // functions
    void expand(BoundingBox &a_bounding_box);
    float intersect(Ray &a_ray);
    void box_transformation(const Matrix4 &transformationMat);
    void get_box_points(vector<Vector3> &vertices);
};

#endif


