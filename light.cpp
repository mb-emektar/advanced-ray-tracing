#include "light.h"
#include <cfloat>

Light::Light()
{
    this->get_position() = Vector3();
};

Vector3 &Light::get_position()
{
    return position;
}

PointLight::PointLight()
{
    intensity = Vector3();
    position = Vector3();
}

float PointLight::calculate_distance(const Intersection &intersection)
{
    float distance = (this->get_position() - intersection.intersection_point).length();
    return distance;
}

Vector3 PointLight::calculate_irradiance(const Intersection &intersection)
{
    float distance = calculate_distance(intersection);
    Vector3 irradiance = intensity / (pow(distance, 2));

    return irradiance;
}

Vector3 PointLight::calculate_incoming_vector(const Intersection &intersection)
{
    Vector3 incoming_vector = (this->get_position() - intersection.intersection_point);
    incoming_vector.normalize();
    return incoming_vector;
}

thread_local Vector3 AreaLight::incoming_vector;
thread_local Vector3 AreaLight::point_light;

AreaLight::AreaLight()
{
    u_vector = Vector3();
    v_vector = Vector3();
    normal = Vector3();
    radiance = Vector3();
    extent = 0;
}
void AreaLight::calculate_properties()
{
    // Construct orthonormal basis uvn.
    Vector3 temp_normal = normal;
    int smallest_index = temp_normal.get_smallest_index();
    if (smallest_index == 0)
        temp_normal.set_x(1);
    else if (smallest_index == 1)
        temp_normal.set_y(1);
    else if (smallest_index == 2)
        temp_normal.set_z(1);

    u_vector = temp_normal.cross_product(normal);
    v_vector = u_vector.cross_product(normal);
    u_vector.normalize();
    v_vector.normalize();
}

float AreaLight::calculate_distance(const Intersection &intersection)
{
    float distance = (point_light - intersection.intersection_point).length();
    return distance;
}

Vector3 AreaLight::calculate_irradiance(const Intersection &intersection)
{
    float area = pow(extent, 2);
    float distance = calculate_distance(intersection);

    float cos_angle = abs(normal.dot_product(-1 * incoming_vector));
    float diff_vector = (area * cos_angle) / (pow(distance, 2));
    Vector3 irradiance = diff_vector * radiance;

    return irradiance;
}

Vector3 AreaLight::calculate_incoming_vector(const Intersection &intersection)
{
    float e1 = gNURandomDistribution(gRandomGenerator);
    float e2 = gNURandomDistribution(gRandomGenerator);
    point_light = this->get_position() + extent * (e1 * u_vector + e2 * v_vector);

    incoming_vector = (point_light - intersection.intersection_point);
    incoming_vector.normalize();
    return incoming_vector;
}

DirectionalLight::DirectionalLight()
{
    direction = Vector3();
    radiance = Vector3();
}

float DirectionalLight::calculate_distance(const Intersection &intersection)
{
    return FLT_MAX;
}

Vector3 DirectionalLight::calculate_irradiance(const Intersection &intersection)
{
    return radiance;
}

Vector3 DirectionalLight::calculate_incoming_vector(const Intersection &intersection)
{
    Vector3 incoming_vector = direction * (-1);
    incoming_vector.normalize();
    return incoming_vector;
}

SpotLight::SpotLight()
{
    direction = Vector3();
    intensity = Vector3();
    coverage_angle = 0;
    falloff_angle = 0;
}

float SpotLight::calculate_falloff(const Vector3 &a_point)
{
    Intersection intersection = Intersection();
    intersection.intersection_point = a_point;
    Vector3 incoming_vector = calculate_incoming_vector(intersection);
    Vector3 unit_direction = direction.unit_vector();
    float cos_angle = unit_direction.dot_product(-1 * incoming_vector);

    float falloff;
    if (acos(cos_angle) < (falloff_angle / 2))
        falloff = 1;
    else if (acos(cos_angle) > (coverage_angle / 2))
        falloff = 0;
    else
    {
        falloff = (cos_angle - cos(coverage_angle / 2)) / (cos(falloff_angle / 2) - cos(coverage_angle / 2));
        falloff = pow(falloff, 4);
    }
    return falloff;
}

float SpotLight::calculate_distance(const Intersection &intersection)
{
    float distance = (this->get_position() - intersection.intersection_point).length();
    return distance;
}

Vector3 SpotLight::calculate_irradiance(const Intersection &intersection)
{
    float distance = calculate_distance(intersection);
    Vector3 irradiance = calculate_falloff(intersection.intersection_point) * (intensity / (pow(distance, 2)));

    return irradiance;
}

Vector3 SpotLight::calculate_incoming_vector(const Intersection &intersection)
{
    Vector3 incoming_vector = (this->get_position() - intersection.intersection_point);
    incoming_vector.normalize();

    return incoming_vector;
}

SphericalDirLight::SphericalDirLight(const string

 &a_image_name)
{
    const string ip_type = "nearest";
    environment_tex = new ImageTexture(ip_type);
    environment_tex->bump_factor = 1;
    environment_tex->normalizer = 255;
    environment_tex->image_path = a_image_name;

    const string decal_mode = "replace_kd";

    environment_tex->calculate_properties(decal_mode);
    random_device rd;
    gRandomGenerator = mt19937(rd());

    gRandomGenerator.seed(chrono::system_clock::now().time_since_epoch().count());
}

float SphericalDirLight::calculate_distance(const Intersection &intersection)
{
    return FLT_MAX;
}

Vector3 SphericalDirLight::calculate_irradiance(const Intersection &intersection)
{
    Color tex_color = get_texture_color(incoming_vector);

    Vector3 temp;
    temp.set_x(tex_color.red);
    temp.set_y(tex_color.green);
    temp.set_z(tex_color.blue);

    Vector3 irradiance = temp * (2 * PI_NUMBER);
    return irradiance;
}

Vector3 SphericalDirLight::calculate_incoming_vector(const Intersection &intersection)
{
    float rnd1, rnd2, rnd3;
    while (true)
    {
        rnd1 = gNURandomDistribution(gRandomGenerator);
        rnd2 = gNURandomDistribution(gRandomGenerator);
        rnd3 = gNURandomDistribution(gRandomGenerator);

        incoming_vector.set_x(rnd1);
        incoming_vector.set_y(rnd2);
        incoming_vector.set_z(rnd3);

        incoming_vector.set_x(2 * incoming_vector.get_x() - 1);
        incoming_vector.set_y(2 * incoming_vector.get_y() - 1);
        incoming_vector.set_z(2 * incoming_vector.get_z() - 1);

        if (incoming_vector.length() <= 1 && (incoming_vector.dot_product(intersection.normal) > 0))
            break;
    }

    incoming_vector.normalize();
    return incoming_vector;
}

Color SphericalDirLight::get_texture_color(Vector3 &a_direction)
{
    float u_vector, v_vector;
    u_vector = (PI_NUMBER - atan2(a_direction.get_z(), a_direction.get_x())) / (2 * PI_NUMBER);
    v_vector = acos(a_direction.get_y()) / PI_NUMBER;
    Vector2 temp1 = Vector2(u_vector, v_vector);
    Vector3 temp2 = Vector3();
    Color color = environment_tex->calculate_texture_color(temp2, temp1);
    return color;
}


