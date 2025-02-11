#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "color.h"
#include "matrix3.h"
#include "vector2.h"
#include "vector3.h"

enum decal_mode
{
    replace_all,
    replace_normal,
    replace_bg,
    replace_kd,
    replace_ks,
    replace_bump_normal,
    blend_kd
};
enum TextureType
{
    image = 0,
    perlin,
    checkerboard,
};

class Texture
{
  public:
    bool is_bump, is_normal;
    decal_mode decal_mode;
    TextureType texture_type;

    Texture();
    void find_decal_mode(const string &a_decal_mode);

    virtual Color calculate_texture_color(Vector3 &position, const Vector2 &uv_mapping) = 0;
    virtual void calculate_properties(const string &a_decal_mode) = 0;
    virtual Vector3 calculate_normal_map_vector(Matrix3 &tbn_matrix, const Vector2 &uv_mapping) = 0;
    virtual Vector3 calculate_bump_vector(const Matrix3 &tbn_matrix, const Vector2 &uv_mapping, Vector3 &position,
                                          Vector3 &normal) const = 0;
};

#endif


