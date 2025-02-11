#include "textureimage.h"

ImageTexture::ImageTexture(const string &a_interpolation)
{
    image_path = "";
    texture_img = nullptr;
    normalizer = 0;
    bump_factor = 0;
    img_width = 0;
    img_height = 0;

    if (a_interpolation == "bilinear")
        ip_type = bilinear;
    else if (a_interpolation == "nearest")
        ip_type = nearest;
    else if (a_interpolation == "trilinear")
        ip_type = trilinear;

    texture_type = image;
}

Color ImageTexture::fetch(int index1, int index2) const
{
    Color color = Color();

    index1 %= img_width;
    index2 %= img_height;
    if (index1 < 0)
        index1 += img_width;
    if (index2 < 0)
        index2 += img_height;
    index1 = min(index1, img_width - 1);
    index2 = min(index2, img_height - 1);

    color = texture_img->get_texture_img_color(index1, index2);
    return color;
}

void ImageTexture::calculate_properties(const string &a_decal_mode)
{
    find_decal_mode(a_decal_mode);
    texture_img = new Image(image_path);
    img_width = texture_img->get_width();
    img_height = texture_img->get_height();
}

Color ImageTexture::calculate_texture_color(Vector3 &position, const Vector2 &uv_mapping)
{
    Color color = Color(0);

    if (ip_type == trilinear)
    {
    }
    else if (ip_type == bilinear)
    {
        int p, q;
        float dx, dy;
        p = floor(uv_mapping.x * img_width);
        q = floor(uv_mapping.y * img_height);
        dx = uv_mapping.x * img_width - p;
        dy = uv_mapping.y * img_height - q;

        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                color += fetch(p + i, q + j) * ((1 - i) - (dx) * (-(i * 2 - 1))) * ((1 - j) - dy * (-(j * 2 - 1)));
    }
    else if (ip_type == nearest)
        color = fetch(uv_mapping.x * img_width, uv_mapping.y * img_height);

    if (decal_mode != replace_bg)
        color /= normalizer;

    return color;
}

Vector3 ImageTexture::getTextureNormal(const Vector2 &uv)
{
    Vector3 temp = Vector3();
    Color temp_color = calculate_texture_color(temp, uv);
    Vector3 normal;
    normal.set_x(temp_color.red);
    normal.set_y(temp_color.green);
    normal.set_z(temp_color.blue);

    normal -= Vector3(0.5, 0.5, 0.5);

    normal.normalize();
    return normal;
}

Vector3 ImageTexture::calculate_normal_map_vector(Matrix3 &tbn_matrix, const Vector2 &uv_mapping)
{
    Vector3 tex_normal = getTextureNormal(uv_mapping);
    Vector3 new_normal = (vector_multiplication(tbn_matrix, tex_normal));
    new_normal.normalize();

    return new_normal;
}

Vector3 ImageTexture::calculate_bump_vector(const Matrix3 &tbn_matrix, const Vector2 &uv_mapping, Vector3 &position,
                                            Vector3 &normal) const
{
    Vector3 pu = tbn_matrix.get_col(0);
    Vector3 pv = tbn_matrix.get_col(1);

    int i = floor(uv_mapping.x * img_width);
    int j = floor(uv_mapping.y * img_height);

    float du = (fetch(i + 1, j).calculate_average() - fetch(i, j).calculate_average()) * bump_factor;
    float dv = (fetch(i, j + 1).calculate_average() - fetch(i, j).calculate_average()) * bump_factor;

    Vector3 qu = pu + du * normal;
    Vector3 qv = pv + dv * normal;

    Vector3 bump_normal = qv.cross_product(qu);
    bump_normal.normalize();

    if (bump_normal.dot_product(normal) < 0)
        bump_normal *= -1;

    return bump_normal;
}


