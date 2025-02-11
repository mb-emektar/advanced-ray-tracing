#ifndef VERTEX_H_
#define VERTEX_H_

#include "vector3.h"

class Vertex
{
  public:
    // data
    Vector3 position;
    Vector3 normal;
    int num_triangles;

    // constructor&destructor
    Vertex()
    {
        position.set_x(0);
        position.set_y(0);
        position.set_z(0);

        normal.set_x(0);
        normal.set_y(0);
        normal.set_z(0);

        num_triangles = 0;
    }

    // functions
    void calculate_shading_properties(Vector3 &a_normal)
    {
        num_triangles++;

        normal.set_x(normal.get_x() + a_normal.get_x());
        normal.set_y(normal.get_y() + a_normal.get_y());
        normal.set_z(normal.get_z() + a_normal.get_z());
    }
};

#endif


