#ifndef TEXTURECHECKERBOARD_H_
#define TEXTURECHECKERBOARD_H_

#include "texture.h"

class CheckerboardTexture : public Texture
{
  public:
    float scale, offset;
    Vector3 white, black;

    CheckerboardTexture();

    Color calculate_texture_color(Vector3 &position, const Vector2 &uv_mapping) override;
    void calculate_properties(const string &a_decal_mode) override;
    Vector3 calculate_normal_map_vector(Matrix3 &tbn_matrix, const Vector2 &uv_mapping) override;
    Vector3 calculate_bump_vector(const Matrix3 &tbn_matrix, const Vector2 &uv_mapping, Vector3 &position,
                                  Vector3 &normal) const override;
};

#endif


