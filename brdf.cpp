#include "BRDF.h"

PhongBRDF::PhongBRDF(float a_exponent)
{
    exponent = a_exponent;
}

Vector3 PhongBRDF::calculate_BRDF_component(Ray &ray, const Material &material, shared_ptr<Light> current_light) const
{
    Vector3 incoming_direction, irradiance, outgoing_direction;
    Vector3 brdf_component = Vector3();
    Intersection intersection = ray.intersection;

    incoming_direction = current_light->calculate_incoming_vector(ray.intersection);
    irradiance = current_light->calculate_irradiance(ray.intersection);
    outgoing_direction = (ray.origin - ray.intersection.intersection_point);
    outgoing_direction.normalize();

    float cos_t = max(float(0), incoming_direction.dot_product(intersection.normal));

    if (cos_t > 0)
    {
        Vector3 diffuse_component = material.diffuse;
        Vector3 specular_component = material.specular;

        Vector3 reflected =
            (2 * (intersection.normal * (incoming_direction.dot_product(intersection.normal))) - incoming_direction);
        reflected.normalize();
        float cos_a_r = max(float(0), reflected.dot_product(outgoing_direction));

        brdf_component = diffuse_component + specular_component * (pow(cos_a_r, exponent) / cos_t);
    }

    return irradiance * brdf_component * cos_t;
}

ModifiedPhongBRDF::ModifiedPhongBRDF(float a_exponent, bool a_normalized)
{
    exponent = a_exponent;
    normalized = a_normalized;
}

Vector3 ModifiedPhongBRDF::calculate_BRDF_component(Ray &ray, const Material &material,
                                                    shared_ptr<Light> current_light) const
{
    Vector3 incoming_direction, irradiance, outgoing_direction;
    Vector3 brdf_component = Vector3();
    Intersection intersection = ray.intersection;

    incoming_direction = current_light->calculate_incoming_vector(ray.intersection);
    irradiance = current_light->calculate_irradiance(ray.intersection);
    outgoing_direction = (ray.origin - ray.intersection.intersection_point);
    outgoing_direction.normalize();
    float cos_t = max(float(0), incoming_direction.dot_product(intersection.normal));

    if (cos_t > 0)
    {
        Vector3 diffuse_component = material.diffuse;
        Vector3 specular_component = material.specular;

        Vector3 reflected =
            (2 * (intersection.normal * (incoming_direction.dot_product(intersection.normal))) - incoming_direction);
        reflected.normalize();
        float cos_a_r = max(float(0), reflected.dot_product(outgoing_direction));

        if (normalized == true)
        {
            brdf_component = (diffuse_component / PI_NUMBER) +
                             (specular_component * ((exponent + 2) / (2 * PI_NUMBER)) * pow(cos_a_r, exponent));
        }
        else
        {
            brdf_component = diffuse_component + specular_component * pow(cos_a_r, exponent);
        }
    }

    return irradiance * brdf_component * cos_t;
}

BlinnPhongBRDF::BlinnPhongBRDF(float a_exponent)
{
    exponent = a_exponent;
}

Vector3 BlinnPhongBRDF::calculate_BRDF_component(Ray &ray, const Material &material,
                                                 shared_ptr<Light> current_light) const
{
    Vector3 incoming_direction, irradiance, outgoing_direction;
    Vector3 brdf_component = Vector3();
    Intersection intersection = ray.intersection;

    incoming_direction = current_light->calculate_incoming_vector(ray.intersection);
    irradiance = current_light->calculate_irradiance(ray.intersection);
    outgoing_direction = (ray.origin - ray.intersection.intersection_point);
    outgoing_direction.normalize();
    float cos_t = max(float(0), incoming_direction.dot_product(intersection.normal));

    if (cos_t > 0)
    {
        Vector3 diffuse_component = material.diffuse;
        Vector3 specular_component = material.specular;

        float cos_a_h =
            max(float(0), ((incoming_direction + outgoing_direction).unit_vector()).dot_product(intersection.normal));

        brdf_component = diffuse_component + specular_component * (pow(cos_a_h, exponent) / cos_t);
    }

    return irradiance * brdf_component * cos_t;
}

ModifiedBlinnPhongBRDF::ModifiedBlinnPhongBRDF(float a_exponent, bool a_normalized)
{
    exponent = a_exponent;
    normalized = a_normalized;
}

Vector3 ModifiedBlinnPhongBRDF::calculate_BRDF_component(Ray &ray, const Material &material,
                                                         shared_ptr<Light> current_light) const
{
    Vector3 incoming_direction, irradiance, outgoing_direction;
    Vector3 brdf_component = Vector3();
    Intersection intersection = ray.intersection;

    incoming_direction = current_light->calculate_incoming_vector(ray.intersection);
    irradiance = current_light->calculate_irradiance(ray.intersection);
    outgoing_direction = (ray.origin - ray.intersection.intersection_point);
    outgoing_direction.normalize();

    float cos_t = max(float(0), incoming_direction.dot_product(intersection.normal));

    if (cos_t > 0)
    {
        Vector3 diffuse_component = material.diffuse;
        Vector3 specular_component = material.specular;

        float cos_a_h =
            max(float(0), ((incoming_direction + outgoing_direction).unit_vector()).dot_product(intersection.normal));

        if (normalized != true)
            brdf_component = diffuse_component + specular_component * pow(cos_a_h, exponent);
        else
            brdf_component = (diffuse_component / PI_NUMBER) +
                             (specular_component * ((exponent + 8) / (8 * PI_NUMBER)) * pow(cos_a_h, exponent));
    }

    return irradiance * brdf_component * cos_t;
}

TorranceSparrowBRDF::TorranceSparrowBRDF(float a_exponent, bool a_kdfresnel)
{
    exponent = a_exponent;
    specular_component_fresnel = a_kdfresnel;
}

Vector3 TorranceSparrowBRDF::calculate_BRDF_component(Ray &ray, const Material &material,
                                                      shared_ptr<Light> current_light) const
{
    Vector3 incoming_direction, irradiance, outgoing_direction;
    Vector3 brdf_component = Vector3();
    Intersection intersection = ray.intersection;

    incoming_direction = current_light->calculate_incoming_vector(ray.intersection);
    irradiance = current_light->calculate_irradiance(ray.intersection);
    outgoing_direction = (ray.origin - ray.intersection.intersection_point);
    outgoing_direction.normalize();

    float cos_t = max(float(0), incoming_direction.dot_product(intersection.normal));

    if (cos_t > 0)
    {
        Vector3 diffuse_component = material.diffuse;
        Vector3 specular_component = material.specular;

        Vector3 halfway_vector = (incoming_direction + outgoing_direction).unit_vector();
        float cos_a_h = max(float(0), halfway_vector.dot_product(intersection.normal));
        float cos_b = max(float(0), outgoing_direction.dot_product(halfway_vector));
        float cos_p = max(float(0), outgoing_direction.dot_product(intersection.normal));

        float D = blinnDistribution(cos_a_h);
        float G = geometryTerm(incoming_direction, outgoing_direction, halfway_vector, intersection.normal);
        float F = fresnelReflectance(material, cos_b);

        if (specular_component_fresnel == true)
        {
            diffuse_component = (1 - F) * diffuse_component;
        }

        brdf_component =
            (diffuse_component / PI_NUMBER) + (specular_component * ((D * F * G) / (4 * cos_t * (cos_p + 0.00001))));
    }

    return irradiance * brdf_component * cos_t;
}

float TorranceSparrowBRDF::blinnDistribution(float cos_a) const
{
    float result = ((exponent + 2) / (2 * PI_NUMBER)) * pow(cos_a, exponent);
    return result;
}

float TorranceSparrowBRDF::geometryTerm(const Vector3 &incoming_direction, const Vector3 &outgoing_direction,
                                        const Vector3 &halfway_vector, const Vector3 &normal) const
{
    float left_term = (2 * normal.dot_product(halfway_vector) * normal.dot_product(outgoing_direction)) /
                      (outgoing_direction.dot_product(halfway_vector));
    float right_term = (2 * normal.dot_product(halfway_vector) * normal.dot_product(incoming_direction)) /
                       (outgoing_direction.dot_product(halfway_vector));
    float term = min(left_term, right_term);
    return min(term, float(1));
}

float TorranceSparrowBRDF::fresnelReflectance(const Material &material, float cos_b) const
{
    float refr_index = material.refraction_index;
    float abs_index = material.absorption_index;

    float rp = ((pow(refr_index, 2) + pow(abs_index, 2)) * pow(cos_b, 2) - 2 * refr_index * cos_b + 1) /
               ((pow(refr_index, 2) + pow(abs_index, 2)) * pow(cos_b, 2) + 2 * refr_index * cos_b + 1);
    float rs = ((pow(refr_index, 2) + pow(abs_index, 2)) - 2 * refr_index * cos_b + pow(cos_b, 2)) /
               ((pow(refr_index, 2) + pow(abs_index, 2)) + 2 * refr_index * cos_b + pow(cos_b, 2));

    return (rs + rp) / 2;
}

