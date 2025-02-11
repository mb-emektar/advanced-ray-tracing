#ifndef CAMERA_H_
#define CAMERA_H_
#include <string>

#include "ray.h"
#include "tonemap.h"
#include "vector3.h"
#include <memory>

struct Vec4f
{
    float x, y, z, w;
};
class Camera
{
  public:
    // data
    float left, right, bottom, top; // image plane
    float near_distance;            // distance
    float focus_distance;
    float aperture_size;
    float fov_y;
    int number_of_samples;
    int image_width, image_height;
    bool is_left_handed;
    Vector3 position;
    Vector3 gaze_point;
    Vector3 gaze;
    Vector3 minus_gaze;      // w -gaze vector
    Vector3 side_vector;     // u side vector
    Vector3 center;          // m center of the image plane
    Vector3 top_left_corner; // q top left corner
    Vector3 up;              // v up vector
    Vec4f near_plane;
    Tonemap tonemap;
    string image_name;

    // constructor&destructor
    Camera();
    ~Camera();

    // functions
    void calculate_lookat_parameters();
    void calculate_parameters();
    shared_ptr<Ray> generate_primary_ray(int a_i, int a_j, bool is_dof, float a_time, float x_offset = 0.5,
                                              float y_offset = 0.5, float x_random = 1, float y_random = 1);
    void print_info(); // for debugging purposes
};

#endif


