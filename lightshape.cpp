#include "lightshape.h"
#include "scene.h"
#include "shape.h"

thread_local float LightMesh::prob_weight;
thread_local Vector3 LightMesh::light_point;
thread_local Vector3 LightMesh::incoming_direction;
thread_local float LightSphere::prob_weight;
thread_local Vector3 LightSphere::light_point;
thread_local Vector3 LightSphere::incoming_direction;

LightMesh::LightMesh(Scene *a_scene) : Mesh(a_scene)
{
    bvh = nullptr;
    area_sum = 0;
}

Intersection LightMesh::intersect(Ray &ray)
{
    Intersection temp = Mesh::intersect(ray);
    ray.intersection.is_light_shape = true;
    ray.intersection.radiance = radiance;
    ray.intersection.light_shape = this;

    return ray.intersection;
}

Vector3 LightMesh::calculate_incoming_vector(const Intersection &intersection)
{
    float rand = gNURandomDistribution(gRandomGenerator);
    shared_ptr<Triangle> selected_triangle = NULL;

    // Select a triangle based on the cumulative distribution function (CDF)
    for (int i = 0; i < cdf_values.size(); i++)
    {
        if (rand <= cdf_values[i])
        {
            selected_triangle = dynamic_pointer_cast<Triangle>(triangles[i]);
            break;
        }
    }
    Vector3 vertex_a, vertex_b, vertex_c;
    vertex_a = p_scene->vertex_data[selected_triangle->vertex_indices.v0].position;
    vertex_b = p_scene->vertex_data[selected_triangle->vertex_indices.v1].position;
    vertex_c = p_scene->vertex_data[selected_triangle->vertex_indices.v2].position;

    Vector3 transformed_a, transformed_b, transformed_c;
    transformed_a = point_multiplication(transformation_matrix, vertex_a);
    transformed_b = point_multiplication(transformation_matrix, vertex_b);
    transformed_c = point_multiplication(transformation_matrix, vertex_c);

    float rnd1 = gNURandomDistribution(gRandomGenerator);
    float rnd2 = gNURandomDistribution(gRandomGenerator);

    // interpolate to find a point on the triangle in transformed space
    light_point = sqrt(rnd1) * ((1 - rnd2) * transformed_b + rnd2 * transformed_c) + (1 - sqrt(rnd1)) * transformed_a;

    float cos_t =
        max(float(0.001), selected_triangle->normal.dot_product(intersection.intersection_point - light_point));
    // calculate the probability weight for sampling
    prob_weight = ((light_point - intersection.intersection_point).length_sqr()) / (area_sum * cos_t);

    incoming_direction = (light_point - intersection.intersection_point);
    incoming_direction.normalize();
    return incoming_direction;
}

float LightMesh::calculate_distance(const Intersection &intersection)
{
    return (light_point - p_scene->shadow_ray_epsilon * incoming_direction - intersection.intersection_point).length();
}

Vector3 LightMesh::calculate_irradiance(const Intersection &intersection)
{
    return radiance / prob_weight;
}

void LightMesh::calculate_CDF()
{
    for (int i = 0; i < triangles.size(); i++)
    {
        shared_ptr<Triangle> triangle = dynamic_pointer_cast<Triangle>(triangles[i]);
        float area = triangle->calculate_area(transformation_matrix);
        area_sum += area;
        cdf_values.push_back(area_sum);
    }

    for (int i = 0; i < cdf_values.size(); i++)
        cdf_values[i] /= area_sum;
}

LightSphere::LightSphere(Scene *a_scene) : Sphere(a_scene)
{
}
Intersection LightSphere::intersect(Ray &ray)
{
    Intersection temp = Sphere::intersect(ray);

    temp.is_light_shape = true;
    temp.radiance = radiance;
    temp.light_shape = this;
    ray.intersection = temp;
    return temp;
}
Vector3 LightSphere::calculate_incoming_vector(const Intersection &intersection)
{
    Vector3 intersection_point = intersection.intersection_point;
    Vector3 local_point = point_multiplication(transform_matrix_inverse, intersection_point);
    Vector3 center = p_scene->vertex_data[center_vertex_id].position;

    // calculate the direction from the local point to the sphere center and its distance
    Vector3 direction = center - local_point;
    float distance = direction.length();

    // calculate maximum angle for sampling directions within the sphere's radius
    float cos_t_max = sqrt(1 - pow(radius / distance, 2));

    //  probability weight for sampling
    prob_weight = 1 / (2 * PI_NUMBER * (1 - cos_t_max));

    direction.normalize();
    Vector3 orthogonal_direction = direction;
    int smallest_index = orthogonal_direction.get_smallest_index();
    if (smallest_index == 0)
        orthogonal_direction.set_x(1);
    else if (smallest_index == 1)
        orthogonal_direction.set_y(1);
    else if (smallest_index == 2)
        orthogonal_direction.set_z(1);

    Vector3 u = orthogonal_direction.cross_product(direction);
    u.normalize();
    Vector3 v = u.cross_product(direction);
    v.normalize();

    float rnd1 = gNURandomDistribution(gRandomGenerator);
    float rnd2 = gNURandomDistribution(gRandomGenerator);

    // calculate spherical coordinates for the sampled direction
    float phi_angle = 2 * PI_NUMBER * rnd1;
    float theta_angle = acos(1 - rnd2 + rnd2 * cos_t_max);
    Vector3 lLocal =
        direction * cos(theta_angle) + v * sin(theta_angle) * cos(phi_angle) + u * sin(theta_angle) * sin(phi_angle);
    Vector3 lWorld = vector_multiplication(transformation_matrix, lLocal);

    // create a ray from the intersection point in the direction of the sampled vector
    Point ray_origin = intersection.intersection_point + p_scene->shadow_ray_epsilon * lWorld.unit_vector();
    Vector3 ray_direction = lWorld;
    ray_direction.normalize();
    Ray light_ray = Ray(ray_origin, ray_direction);

    Intersection result_int = intersect(light_ray);
    if (result_int.does_hit == true)
    {
        light_point = result_int.intersection_point;
        incoming_direction = (result_int.intersection_point - intersection.intersection_point).unit_vector();
    }
    return incoming_direction;
}

float LightSphere::calculate_distance(const Intersection &intersection)
{
    return (light_point - intersection.intersection_point).length();
}

Vector3 LightSphere::calculate_irradiance(const Intersection &intersection)
{
    return radiance / prob_weight;
}


