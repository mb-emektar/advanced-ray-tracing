#ifndef TEXTUREIMAGE_H_
#define TEXTUREIMAGE_H_

#include "image.h"
#include "texture.h"

#include <string>

enum InterpolationType
{
    nearest,
    bilinear,
    trilinear
};

class ImageTexture : public Texture
{
  public:
    int img_width, img_height;

    float normalizer, bump_factor;
    Image *texture_img;
    string image_path;
    InterpolationType ip_type;

    ImageTexture(const string &a_interpolation);

    Color fetch(int i, int j) const;

    Color calculate_texture_color(Vector3 &position, const Vector2 &uv_mapping) override;
    void calculate_properties(const string &a_decal_mode) override;
    Vector3 calculate_normal_map_vector(Matrix3 &tbn_matrix, const Vector2 &uv_mapping) override;
    Vector3 calculate_bump_vector(const Matrix3 &tbn_matrix, const Vector2 &uv_mapping, Vector3 &position,
                                  Vector3 &normal) const override;

  private:
    Vector3 getTextureNormal(const Vector2 &uv);
};

#endif


