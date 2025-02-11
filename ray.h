#ifndef RAY_H
#define RAY_H

#include <vector>

#include "color.h"
#include "vector2.h"
#include "vector3.h"
#include <memory>

class Light;
class Shape;
class Texture;

struct Intersection
{
    // data
    bool does_hit;
    float distance;
    int material_id;
    Color color;
    Shape *p_shape;
    Vector3 normal;
    Vector3 intersection_point;
    shared_ptr<Texture> texture;
    Vector2 texture_vector;   

    bool is_light_shape;	
    Light *light_shape;
    Vector3 radiance;	

    // constructor&destructor
    Intersection();
    virtual ~Intersection();

    // functions
    bool intersected() const;

    // operator overloading
    Intersection &operator=(const Intersection &r_intersection);
};

class Ray
{
  public:
    // data
    bool shadow_ray;
    bool inside_obj;
    float time;
    Point origin;
    Vector3 direction;
    Intersection intersection;

    // constructor&destructor
    Ray();
    Ray(const Ray &a_r);
    Ray(const Point &a_origin, const Vector3 &a_direction);
    virtual ~Ray();

    // functions
    Point calculate(float a_t) const;
    void print_ray(); // for debbuging purposes

    // operator overloading
    Ray &operator=(const Ray &a_r);
};

class Shape;

#endif // RAY_H


