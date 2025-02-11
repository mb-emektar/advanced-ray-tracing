#include <cfloat> // defines FLT_MAX

#include "ray.h"

Intersection::Intersection()
{
    does_hit = false;
    distance = FLT_MAX;
    p_shape = nullptr;
    material_id = 0;
    texture = nullptr;
    texture_vector = Vector2();
    is_light_shape = false;
    light_shape = nullptr;
    radiance = Vector3();

    intersection_point.set_x(0);
    intersection_point.set_y(0);
    intersection_point.set_z(0);
}

Intersection::~Intersection()
{
}

bool Intersection::intersected() const
{
    return (p_shape != NULL);
}

Intersection &Intersection::operator=(const Intersection &r_intersection)
{
    does_hit = r_intersection.does_hit;
    p_shape = r_intersection.p_shape;
    color = r_intersection.color;
    normal = r_intersection.normal;
    distance = r_intersection.distance;
    material_id = r_intersection.material_id;
    intersection_point = r_intersection.intersection_point;
    texture = r_intersection.texture;    
	texture_vector = r_intersection.texture_vector;
    is_light_shape = r_intersection.is_light_shape;
    radiance = r_intersection.radiance;
    light_shape = r_intersection.light_shape;
    return *this;
}

Ray::Ray()
{
    intersection = Intersection();
    origin = Vector3();
    direction = Vector3();
    shadow_ray = false;
    inside_obj = false;
    time = -1;
}

Ray::Ray(const Ray &a_r)
{
    intersection = Intersection();
    origin = a_r.origin;
    direction = a_r.direction;
    shadow_ray = false;
    inside_obj = false;
    time = -1;
}

Ray::Ray(const Point &a_origin, const Vector3 &a_direction)
{
    intersection = Intersection();
    origin = a_origin;
    direction = a_direction;
    shadow_ray = false;
    inside_obj = false;
    time = -1;
}

Ray::~Ray()
{
}

Point Ray::calculate(float a_distance) const
{
    return origin + direction * a_distance;
}
void Ray::print_ray()
{
}

Ray &Ray::operator=(const Ray &r_ray)
{
    direction = r_ray.direction;
    inside_obj = r_ray.inside_obj;
    intersection = r_ray.intersection;
    origin = r_ray.origin;
    shadow_ray = r_ray.shadow_ray;
    time = r_ray.time;
    return *this;
}

