#include "perlinnoise.h"
#include <algorithm>
#include <random>

PerlinNoise::PerlinNoise()
{
    gradient_vector = {Vector3(1, 1, 0), Vector3(-1, 1, 0), Vector3(1, -1, 0), Vector3(-1, -1, 0),
                       Vector3(1, 0, 1), Vector3(-1, 0, 1), Vector3(1, 0, -1), Vector3(-1, 0, -1),
                       Vector3(0, 1, 1), Vector3(0, -1, 1), Vector3(0, 1, -1), Vector3(0, -1, -1),
                       Vector3(1, 1, 0), Vector3(-1, 1, 0), Vector3(0, -1, 1), Vector3(0, -1, -1)};

    for (int i = 0; i < 16; ++i)
        pixels.push_back(i);

    mt19937 gRandomGenerator;
    random_device rd;
    gRandomGenerator = mt19937(rd());
    uniform_real_distribution<float> gNURandomDistribution{-0.5, 0.5};
    shuffle(pixels.begin(), pixels.end(), gRandomGenerator);
}

float PerlinNoise::get_noise(Vector3 &position) const
{
    float noise = 0;
    int x, y, z;

    x = floor(position.get_x());
    y = floor(position.get_y());
    z = floor(position.get_z());

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
            {
                Vector3 lattice_point = Vector3(x + i, y + j, z + k);

                int a = lattice_point.get_x();
                int b = lattice_point.get_y();
                int c = lattice_point.get_z();

                Vector3 edge_vector = gradient_vector[phi(a + phi(b + phi(c)))];
                noise += calculate_noise(edge_vector, position - Vector3(a, b, c));
            }

    return noise;
}

float PerlinNoise::phi(int index) const
{
    while (index < 0)
        index += 16;
    index %= 16;

    return pixels[index];
}

float PerlinNoise::weight(float x) const
{
    float abs_x = fabs(x);
    return 1 - (10 * pow(abs_x, 3)) + (-6 * pow(abs_x, 5)) + (15 * pow(abs_x, 4));
}

float PerlinNoise::calculate_noise(Vector3 e, Vector3 v) const
{
    float output;
    output = e.dot_product(v) * weight(v.get_x()) * weight(v.get_y()) * weight(v.get_z());
    return output;
}

