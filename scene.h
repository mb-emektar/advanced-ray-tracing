#ifndef __HW1__SCENE__
#define __HW1__SCENE__

#include <string>
#include <vector>
#include <iostream>
#include "camera.h"
#include "image.h"
#include "light.h"
#include "shape.h"
#include "texturecheckerboard.h"
#include "textureimage.h"
#include "textureperlin.h"
#include "LightShape.h"
#include "tonemap.h"
#include "transformation.h"
#include "vector2.h"
#include "vertex.h"
#include <chrono>
#include <memory>
#include <random>
#include "brdf.h"
#include <windows.h>
#include "SDL2/include/SDL.h"
#include "threadpool.h"
#include <pthread.h>
#include <omp.h>

#include "tinyxml2/tinyxml2.h"

#define XML_PARSE_LOGS false
#define BVH_ON true
#define PI_NUMBER 3.141592

using namespace std;

struct Vec3f
{
    float x, y, z;
};

struct Vec3i
{
    int x, y, z;
};

class Scene
{
  public:
    // data
    BVH *p_bounding_volume;
    Vec3i background_color;
    float shadow_ray_epsilon;
    float test_epsilon;
    int max_recursion_depth;
    Vector3 ambient_light;
    shared_ptr<ImageTexture> bg_texture;

    vector<Camera> v_cameras;
    vector<PointLight> point_lights;
    vector<AreaLight> area_lights;
    vector<DirectionalLight> dir_lights;
    vector<SpotLight> spot_lights;
    vector<shared_ptr<Light>> all_lights;
    vector<Material> materials;
    vector<Vertex> vertex_data;
    vector<Mesh> meshes;
    vector<shared_ptr<Shape>> base_meshes;
    vector<Triangle> triangles;
    vector<Sphere> spheres;
    vector<shared_ptr<Shape>> p_all_shapes;
    vector<Translation> translations;
    vector<Scaling> scalings;
    vector<Rotation> rotations;
    vector<Composite> composites;
    vector<shared_ptr<Texture>> textures;
    vector<Vector2> texture_coordinates;
    shared_ptr<SphericalDirLight> spherical_dir_light;
	  vector<shared_ptr<BRDF>> brdfs;
    
    vector<Color> pixelColors;
    SDL_Window* window ;
    SDL_Renderer* renderer;
    SDL_PixelFormat* format;
    SDL_Texture* texture;
    SDL_Event SDL_event;
    Uint32* pixels;
    int sdl_image_width;
    int sdl_image_height;
    bool program_running;
    /*
    SDL_Window* SDL_window = nullptr;
    SDL_Renderer* SDL_renderer = nullptr;
    */
    mt19937 gRandomGenerator;
    uniform_real_distribution<float> gNURandomDistribution{-0.5, 0.5};

    // constructor&destructor
    Scene();
    ~Scene(){};

    // functions

    void loadFromXml(const string &a_filepath);
    void process_scene();

  private:
    void ply_parser(const string &filepath, const char *ply_file, Mesh &mesh, bool smooth, int vertexOffset,
                    int textureOffset);

    bool check_light_shadow(Ray &ray, Vector3 direction, shared_ptr<Light> a_light, Vector3 &a_light_point);

    void trace_ray(Ray &ray);
    void ray_sampling(Camera &camera, bool is_dof, int i, int j, vector<shared_ptr<Ray>> &raysPerPixel);

    void render_chunk(int start_row, int end_row, int start_col, int end_col, int image_width, Camera &camera);
    float reflection_ratio_conductor(Ray &ray, float refractiveIndex, float absorptionIndex);
    float reflection_ratio_dielectric(float cosine_angle, float n1, float n2);
    Vector3 ray_refraction(Vector3 direction, Vector3 normal, float currentRefractiveIndex, float nextRefractiveIndex);

    Vector3 calculate_irradiance(PointLight &pointLight, const Vector3 &intersectionPoint);
    Vector3 calculate_light_shadings(const Camera &camera, const Material &material, const Vector3 &irradiance,
                                     Vector3 &wi, Vector3 &intersection_point, const Vector3 &normal, Vector2 uvTexture,
                                     shared_ptr<Texture> a_texture);
    Vector3 calculate_roughness(Vector3 normal_wi, Material material);
    Matrix4 calculate_transformation_matrix(const vector<char> types, const vector<int> ids,
                                            Matrix4 transformation_matrix);

    Color find_color(Camera &camera, Ray &ray, int recursion_depth, int i, int j);
    Color calculate_bg_pixel_color(Ray &ray, int i, int j) const;
    void calculate_degamma(Material &material, const Tonemap &tonemap);

    void SDL_thread_start(int image_width, int image_height, std::string image_name );
    Uint32 vector3_to_uint32(Color color);
    void convertToUint32() ;
    int SDL_image_create(int image_width, int image_height, std::string image_name);
    void SDL_image_renderer();
    void SDL_image_finalize();
};

#endif


