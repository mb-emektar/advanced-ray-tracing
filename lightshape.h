#ifndef LIGHTSHAPE_H_
#define LIGHTSHAPE_H_

#include "Light.h"
#include "Shape.h"

class LightMesh : public Light, public Mesh
{
  public:
    Vector3 radiance;

    LightMesh(Scene *a_scene);
    Intersection intersect(Ray &ray) override;
    float calculate_distance(const Intersection &intersection) override;
    Vector3 calculate_irradiance(const Intersection &intersection) override;
    Vector3 calculate_incoming_vector(const Intersection &intersection) override;
    void calculate_CDF();

  private:
    static thread_local float prob_weight;
    static thread_local Vector3 light_point;
    static thread_local Vector3 incoming_direction;

    float area_sum;
    vector<float> cdf_values;
    mt19937 gRandomGenerator;
    uniform_real_distribution<float> gNURandomDistribution{0, 1};
};

class LightSphere : public Light, public Sphere
{
  public:
    Vector3 radiance;

    LightSphere(Scene *scene_);
    Intersection intersect(Ray &ray) override;
    float calculate_distance(const Intersection &intersection) override;
    Vector3 calculate_irradiance(const Intersection &intersection) override;
    Vector3 calculate_incoming_vector(const Intersection &intersection) override;

  private:
    static thread_local float prob_weight;
    static thread_local Vector3 light_point;
    static thread_local Vector3 incoming_direction;

    mt19937 gRandomGenerator;
    uniform_real_distribution<float> gNURandomDistribution{0, 1};
};

#endif


