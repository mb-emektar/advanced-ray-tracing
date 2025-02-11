#include "camera.h"

#define PI_NUMBER 3.14

Camera::Camera()
{
    left = 0;
    right = 0;
    bottom = 0;
    top = 0;
    near_distance = 0;
    focus_distance = 0;
    aperture_size = 0;
    fov_y = 0;
    number_of_samples = 0;
    image_width = 0;
    image_height = 0;
    is_left_handed = false;
    tonemap = Tonemap();
}
Camera::~Camera()
{
}
void Camera::calculate_lookat_parameters()
{
    gaze = gaze_point - position;
    top = near_distance * tan((PI_NUMBER * fov_y) / (2 * 180));
    bottom = (-1) * top;
    right = top * (float(image_width) / float(image_height));
    left = (-1) * right;
}

void Camera::calculate_parameters()
{
    gaze.normalize();
    minus_gaze = gaze * (-1);
    side_vector = up.cross_product(minus_gaze).unit_vector();
    up = minus_gaze.cross_product(side_vector).unit_vector();

    if (is_left_handed)
        side_vector *= (-1);
    center = position - (minus_gaze * near_distance);
    top_left_corner = center + (left * side_vector) + (top * up);
}

shared_ptr<Ray> Camera::generate_primary_ray(int a_i, int a_j, bool is_dof, float a_time, float x_offset,
                                                  float y_offset, float x_random, float y_random)
{
    float s_v, s_u;
    s_v = (y_offset + a_i) * (top - bottom) / image_height;
    s_u = (x_offset + a_j) * (right - left) / image_width;

    Vector3 pixel_pos = s_u * side_vector - s_v * up + top_left_corner;

    Vector3 dir;
    if (is_dof)
    {
        dir = (pixel_pos - position);
        dir.normalize();
    }

    shared_ptr<Ray> ray = make_shared<Ray>();

    if (is_dof)
    {
        ray->origin = position;
        ray->direction = dir;
        ray->direction.normalize();

        Vector3 focal_plane_point = ray->calculate(focus_distance / (dir.dot_product(gaze)));
        Vector3 aperture_sample_point = aperture_size * (up * y_random + side_vector * x_random) + position;
        ray->direction = focal_plane_point - aperture_sample_point;
        ray->direction.normalize();
        ray->origin = aperture_sample_point;
        ray->time = a_time;
    }
    else
    {
        ray->direction = (pixel_pos - position);
        ray->direction.normalize();
        ray->origin = position;
        ray->time = a_time;
    }
    return ray;
}

void Camera::print_info()
{
    cout << "Camera  position = " << position << endl;
    cout << "Camera  focus_distance = " << focus_distance << endl;
    cout << "Camera  aperture_size = " << aperture_size << endl;
    cout << "Camera  fov_y = " << fov_y << endl;
    cout << "Camera  gaze = " << gaze << endl;
    cout << "Camera  gaze_point = " << gaze_point << endl;
    cout << "Camera  minus_gaze = " << minus_gaze << endl;
    cout << "Camera  side_vector = " << side_vector << endl;
    cout << "Camera  center = " << center << endl;
    cout << "Camera  top_left_corner = " << top_left_corner << endl;
    cout << "Camera  up = " << up << endl;
    cout << "Camera  near_plane = " << near_plane.x << " " << near_plane.y << " " << near_plane.z << " "
              << near_plane.w << endl;
    cout << "Camera  left = " << left << " right = " << right << " bottom = " << bottom << " top = " << top
              << endl;
    cout << "Camera  near_distance = " << near_distance << endl;
    cout << "Camera  image_width = " << image_width << endl;
    cout << "Camera  image_height = " << image_height << endl;
    cout << "Camera  image_name = " << image_name << endl;
}


