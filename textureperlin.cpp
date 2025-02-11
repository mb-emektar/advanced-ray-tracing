#include "textureperlin.h"

#define EPSILON 0.006

PerlinTexture::PerlinTexture(const string &a_noise_type)
{
    scaling_factor = 0;
    bump_factor = 0;
    perlin_noise = PerlinNoise();

    if (a_noise_type == "absval")
        noise_conv_type = absval;
    else if (a_noise_type == "linear")
        noise_conv_type = linear;

    texture_type = perlin;
}

float PerlinTexture::get_noise_value(const Vector3 &position) const
{
    Vector3 new_position = position * scaling_factor;
    float noise = perlin_noise.get_noise(new_position);

    if (noise_conv_type == absval)
        noise = fabs(noise);
    else if (noise_conv_type == linear)
    {
        noise += 1;
        noise /= 2;
    }

    return noise;
}

void PerlinTexture::calculate_properties(const string &a_decal_mode)
{
    find_decal_mode(a_decal_mode);
}

Color PerlinTexture::calculate_texture_color(Vector3 &position, const Vector2 &uv_mapping)
{
    float noise = get_noise_value(position);
    Color tex_color = Color(noise);
    return tex_color;
}
Vector3 PerlinTexture::calculate_normal_map_vector(Matrix3 &tbn_matrix, const Vector2 &uv_mapping)
{
    Vector3 temp = Vector3();
    return temp;
}

Vector3 PerlinTexture::calculate_bump_vector(const Matrix3 &tbn_matrix, const Vector2 &uv_mapping, Vector3 &position,
                                             Vector3 &normal) const
{
    float noise = get_noise_value(position);

    Vector3 gradient_vector = Vector3();
    gradient_vector.set_x(
        bump_factor *
        ((get_noise_value(Vector3(position.get_x() + EPSILON, position.get_y(), position.get_z())) - noise) / EPSILON));
    gradient_vector.set_y(
        bump_factor *
        ((get_noise_value(Vector3(position.get_x(), position.get_y() + EPSILON, position.get_z())) - noise) / EPSILON));
    gradient_vector.set_z(
        bump_factor *
        ((get_noise_value(Vector3(position.get_x(), position.get_y(), position.get_z() + EPSILON)) - noise) / EPSILON));

    Vector3 tangent = (gradient_vector.dot_product(normal)) * normal;
    Vector3 bump_normal = (normal - (gradient_vector - tangent));
    bump_normal.normalize();
    return bump_normal;
}


