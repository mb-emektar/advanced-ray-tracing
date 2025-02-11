#ifndef BRDF_H_
#define BRDF_H_

#include "Vector3.h"
#include "light.h"
#include "ray.h"
#include "shape.h"

class BRDF
{
  public:
    virtual Vector3 calculate_BRDF_component(Ray &ray, const Material &material,
                                             shared_ptr<Light> current_light) const = 0;
};

class PhongBRDF : public BRDF
{
  public:
    PhongBRDF(float a_exponent);
    Vector3 calculate_BRDF_component(Ray &ray, const Material &material, shared_ptr<Light> current_light) const;

  private:
    float exponent;
};

class ModifiedPhongBRDF : public BRDF
{
  public:
    ModifiedPhongBRDF(float a_exponent, bool a_normalized);
    Vector3 calculate_BRDF_component(Ray &ray, const Material &material, shared_ptr<Light> current_light) const;

  private:
    bool normalized;
    float exponent;
};

class BlinnPhongBRDF : public BRDF
{
  public:
    BlinnPhongBRDF(float a_exponent);
    Vector3 calculate_BRDF_component(Ray &ray, const Material &material, shared_ptr<Light> current_light) const;

  private:
    float exponent;
};

class ModifiedBlinnPhongBRDF : public BRDF
{
  public:
    ModifiedBlinnPhongBRDF(float a_exponent, bool a_normalized);
    Vector3 calculate_BRDF_component(Ray &ray, const Material &material, shared_ptr<Light> current_light) const;

  private:
    bool normalized;
    float exponent;
};

class TorranceSparrowBRDF : public BRDF
{
  public:
    TorranceSparrowBRDF(float a_exponent, bool a_kdfresnel);
    Vector3 calculate_BRDF_component(Ray &ray, const Material &material, shared_ptr<Light> current_light) const;

  private:
    bool specular_component_fresnel;
    float exponent;

    float blinnDistribution(float cos_a) const;
    float geometryTerm(const Vector3 &incoming_direction, const Vector3 &outgoing_direction,
                       const Vector3 &halfway_vector, const Vector3 &normal) const;
    float fresnelReflectance(const Material &material, float cos_b) const;
};

#endif


