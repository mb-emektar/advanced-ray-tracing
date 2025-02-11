#include "texturecheckerboard.h"

CheckerboardTexture::CheckerboardTexture()
{
    offset = 0;
    scale = 0;
    white = Vector3();
    black = Vector3();
    texture_type = checkerboard;
}

Color CheckerboardTexture::calculate_texture_color(Vector3 &position, const Vector2 &uv_mapping)
{
    Color color = Color();

    int x = ((int)(scale * (position.get_x() + offset)) % 2);
    int y = ((int)(scale * (position.get_y() + offset)) % 2);
    int z = ((int)(scale * (position.get_z() + offset)) % 2);

    if ((x + y + z) % 2) // is black
    {
        color.red = black.get_x();
        color.green = black.get_y();
        color.blue = black.get_z();
    }
    else
    {
        color.red = white.get_x();
        color.green = white.get_y();
        color.blue = white.get_z();
    }

    return color;
}

void CheckerboardTexture::calculate_properties(const string &a_decal_mode)
{
    find_decal_mode(a_decal_mode);
}

Vector3 CheckerboardTexture::calculate_normal_map_vector(Matrix3 &tbn_matrix, const Vector2 &uv_mapping)
{
    Vector3 output = tbn_matrix.get_col(2);
    return output;
}

Vector3 CheckerboardTexture::calculate_bump_vector(const Matrix3 &tbn_matrix, const Vector2 &uv_mapping,
                                                   Vector3 &position, Vector3 &normal) const
{
    return normal;
}

