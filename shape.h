#ifndef SHAPE_H
#define SHAPE_H

#include <vector>

#include "boundingbox.h"
#include "matrix3.h"
#include "matrix4.h"
#include "ray.h"
#include "texture.h"
#include "vector3.h"
#include <memory>

using namespace std;

enum MaterialType
{
    NONE,
    MIRROR,
    DIELECTRIC,
    CONDUCTOR
};

struct Material
{
    float refraction_index;
    float absorption_index;
    float phong_exponent;
    float roughness;
	  int brdf_index;
    bool degamma;
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
    Vector3 mirror;
    Vector3 absorption;
    MaterialType material_type;
};

struct Face
{
    int v0;
    int v1;
    int v2;
};

class Shape
{
  public:
    bool transform;
    bool motion_blur;
    int material_id;
    Matrix4 transformation_matrix;
    Matrix4 transform_matrix_inverse;
    Matrix4 transform_matrix_normal;
    Vector3 motion_blur_vector;

    shared_ptr<Texture> texture;
    shared_ptr<Texture> np_texture;

    BoundingBox bounding_box;

    // constructor&destructor
    virtual Intersection intersect(Ray &ray) = 0;
    virtual void print_info() = 0; // for debugging purposes
    virtual int get_material_id() = 0;
    virtual Shape *get_bvh() = 0;
    Ray ray_transformation(Ray &ray);
    Intersection intersection_transformation(Ray &ray);
    void calculate_matrices();
    BoundingBox &get_bounding_box()
    {
        return bounding_box;
    }
};

class Scene;
class BVH : public Shape
{
  public:
    shared_ptr<Shape> p_left_child, p_right_child;
    Scene *p_scene;
    BVH(vector<shared_ptr<Shape>> &shapes, int axis, int start, int end);
    ~BVH();
    Intersection intersect(Ray &ray) override;
    void print_info() override // for debugging purposes
    {
        cout << "BVH print_info" << endl;
    }
    int get_material_id() override
    {
        return -1;
    }
    Shape *get_bvh() override
    {
        return nullptr;
    }
};

class Sphere : public Shape
{
  public:
    // data
    int center_vertex_id;
    float radius;
    Scene *p_scene;

    // constructor&destructor
    Sphere(Scene *a_p_scene);
    ~Sphere();

    // functions
    Intersection intersect(Ray &ray) override;
    void print_info() override; // for debugging purposes
    int get_material_id() override;
    Shape *get_bvh() override
    {
        return nullptr;
    }
    void calculate_properties();

  private:
    Vector2 get_tex_coordinates(Vector3 &point, shared_ptr<Texture> tex) const;
    Matrix3 calculate_TBN(Vector3 &point, Vector3 &normal);
};

class Triangle : public Shape
{
  public:
    // data
    bool smooth;
    int debug_count;
    float area;
    Face vertex_indices;
    Face texture_indices;
    Scene *p_scene;
    Vector3 normal;
    Matrix3 matrix_TBN;
    int v_offset;
    int t_offset;

    // constructor&destructor
    Triangle(Scene *a_p_scene);
    ~Triangle();

    // functions
    Intersection intersect(Ray &ray) override;
    void print_info() override; // for debugging purposes
    int get_material_id() override;
    Shape *get_bvh() override
    {
        return nullptr;
    }
    void calculate_properties();
    float determinant(Vector3 v0, Vector3 v1, Vector3 v2);
    Vector2 get_tex_coordinates(float beta, float gamma, shared_ptr<Texture> tex) const;
    float calculate_area(Matrix4 & matrix);
  private:
    Matrix3 calculate_TBN();
};
class Mesh : public Shape
{
  public:
    // data
    vector<shared_ptr<Shape>> triangles;
    Scene *p_scene;
    Shape *bvh;

    // constructor&destructor
    Mesh(Scene *a_p_scene);
    ~Mesh();

    // functions
    Intersection intersect(Ray &ray) override;
    void print_info() override; // for debugging purposes
    int get_material_id() override;
    Shape *get_bvh() override
    {
        return bvh;
    }

    void calculate_properties();
};

class MeshInstance : public Shape
{
  public:
    // data
    Shape *baseMeshBVH;
    Scene *p_scene;

    // constructor&destructor
    MeshInstance();
    ~MeshInstance();

    // functions
    Intersection intersect(Ray &ray) override;
    void print_info() override; // for debugging purposes
    int get_material_id() override;
    Shape *get_bvh() override
    {
        return baseMeshBVH;
    }

    void calculate_properties();
};

#endif // SHAPE_H


