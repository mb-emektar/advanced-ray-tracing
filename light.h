#ifndef AREALIGHT_H_
#define AREALIGHT_H_

#include "ray.h"
#include "textureimage.h"
#include "vector2.h"
#include "vector3.h"
#include <chrono>
#include <random>

#define PI_NUMBER 3.141592
class Light
{
  public:
    Light();
    Vector3 &get_position();

    virtual float calculate_distance(const Intersection &intersection) = 0;
    virtual Vector3 calculate_irradiance(const Intersection &intersection) = 0;
    virtual Vector3 calculate_incoming_vector(const Intersection &intersection) = 0;

  private:
    Vector3 position;
};

class PointLight : public Light
{
  public:
    Vector3 intensity;
    Vector3 position;

    // constructor&destructor
    PointLight();
    ~PointLight(){};

    float calculate_distance(const Intersection &intersection) override;
    Vector3 calculate_irradiance(const Intersection &intersection) override;
    Vector3 calculate_incoming_vector(const Intersection &intersection) override;
};
class AreaLight : public Light
{
  public:
    mt19937 gRandomGenerator;
    uniform_real_distribution<float> gNURandomDistribution{-0.5, 0.5};

    // data
    Vector3 u_vector, v_vector;
    Vector3 position;
    Vector3 normal;
    Vector3 radiance;
    Vector3 point_on_light;

    float extent;

    // constructor&destructor
    AreaLight();
    ~AreaLight(){};

    void calculate_properties();
    float calculate_distance(const Intersection &intersection) override;
    Vector3 calculate_irradiance(const Intersection &intersection) override;
    Vector3 calculate_incoming_vector(const Intersection &intersection) override;

  private:
    static thread_local Vector3 incoming_vector;
    static thread_local Vector3 point_light;
};

class DirectionalLight : public Light
{
  public:
    Vector3 direction, radiance;

    // constructor&destructor
    DirectionalLight();
    ~DirectionalLight(){};

    float calculate_distance(const Intersection &intersection) override;
    Vector3 calculate_irradiance(const Intersection &intersection) override;
    Vector3 calculate_incoming_vector(const Intersection &intersection) override;
};

class SpotLight : public Light
{
  public:
    Vector3 direction, intensity;
    float coverage_angle;
    float falloff_angle;

    // constructor&destructor
    SpotLight();
    ~SpotLight(){};

    float calculate_distance(const Intersection &intersection) override;
    Vector3 calculate_irradiance(const Intersection &intersection) override;
    Vector3 calculate_incoming_vector(const Intersection &intersection) override;

  private:
    float calculate_falloff(const Vector3 &a_point);
};

class SphericalDirLight : public Light
{
  public:
    mt19937 gRandomGenerator;
    uniform_real_distribution<float> gNURandomDistribution{0, 1};
    ImageTexture *environment_tex;

    SphericalDirLight(const string &a_image_name);
    ~SphericalDirLight(){};
    Color get_texture_color(Vector3 &a_direction);

    float calculate_distance(const Intersection &intersection) override;
    Vector3 calculate_irradiance(const Intersection &intersection) override;
    Vector3 calculate_incoming_vector(const Intersection &intersection) override;

  private:
    Vector3 incoming_vector;
};







#endif


