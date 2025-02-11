#ifndef PERLINNOISE_H_
#define PERLINNOISE_H_

#include "texture.h"
#include "vector3.h"
#include <chrono>
#include <random>
#include <string>
#include <vector>

class PerlinNoise
{
  public:
    // constructor
    PerlinNoise();
    // functions
    float get_noise(Vector3 &position) const;

  private:
    // data
    vector<Vector3> gradient_vector;
    vector<int> pixels;

    // functions
    float phi(int i) const;
    float weight(float x) const;
    float calculate_noise(Vector3 e, Vector3 v) const;
};

#endif

