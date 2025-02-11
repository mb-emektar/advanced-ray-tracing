#ifndef TEXTUREPERLIN_H_
#define TEXTUREPERLIN_H_

#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <vector>

#include "perlinnoise.h"
#include "texture.h"
#include "vector3.h"

enum NoiseType
{
    linear,
    absval
};

class PerlinTexture : public Texture
{
  public:
    float scaling_factor, bump_factor;
    PerlinNoise perlin_noise;

    PerlinTexture(const string &a_noise_type);

    float get_noise_value(const Vector3 &position) const;

    Color calculate_texture_color(Vector3 &position, const Vector2 &uv_mapping) override;
    void calculate_properties(const string &a_decal_mode) override;
    Vector3 calculate_normal_map_vector(Matrix3 &tbn_matrix, const Vector2 &uv_mapping) override;
    Vector3 calculate_bump_vector(const Matrix3 &tbn_matrix, const Vector2 &uv_mapping, Vector3 &position,
                                  Vector3 &normal) const override;

  private:
    NoiseType noise_conv_type;
};

#endif

