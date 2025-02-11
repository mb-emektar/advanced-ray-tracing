#include "boundingbox.h"

using namespace std;

BoundingBox::BoundingBox()
{
    // cout << "BoundingBox::BoundingBox() 1" << endl;
    min_point.set_x(FLT_MAX);
    min_point.set_y(FLT_MAX);
    min_point.set_z(FLT_MAX);
    max_point.set_x(-FLT_MAX);
    max_point.set_y(-FLT_MAX);
    max_point.set_z(-FLT_MAX);
}

BoundingBox::BoundingBox(Vector3 a_min_point, Vector3 a_max_point)
{
    // cout << "BoundingBox::BoundingBox() 2" << endl;
    min_point.set_x(a_min_point.get_x());
    min_point.set_y(a_min_point.get_y());
    min_point.set_z(a_min_point.get_z());
    max_point.set_x(a_max_point.get_x());
    max_point.set_y(a_max_point.get_y());
    max_point.set_z(a_max_point.get_z());
    centroid = (max_point + min_point) / 2;
    diagonal = max_point - min_point;
}

void BoundingBox::expand(BoundingBox &a_bounding_box)
{
    /*cout << "BoundingBox before max_point = " << max_point<< endl;
    cout << "BoundingBox before min_point = " << min_point<< endl;*/
    // update max_point coordinates to be the maximum of the current and the new bounding box
    max_point.set_x(max(a_bounding_box.max_point.get_x(), max_point.get_x()));
    max_point.set_y(max(a_bounding_box.max_point.get_y(), max_point.get_y()));
    max_point.set_z(max(a_bounding_box.max_point.get_z(), max_point.get_z()));

    // update min_point coordinates to be the maximum of the current and the new bounding box
    min_point.set_x(min(a_bounding_box.min_point.get_x(), min_point.get_x()));
    min_point.set_y(min(a_bounding_box.min_point.get_y(), min_point.get_y()));
    min_point.set_z(min(a_bounding_box.min_point.get_z(), min_point.get_z()));

    /*cout << "BoundingBox after max_point = " << max_point<< endl;
    cout << "BoundingBox after min_point = " << min_point<< endl;*/
    // recalculate the centroid as the average of the new min and max points
    centroid.set_x((min_point.get_x() + max_point.get_x()) / 2);
    centroid.set_y((min_point.get_y() + max_point.get_y()) / 2);
    centroid.set_z((min_point.get_z() + max_point.get_z()) / 2);

    // recalculate the diagonal as the difference between the new max and min points
    diagonal.set_x(max_point.get_x() - min_point.get_x());
    diagonal.set_y(max_point.get_y() - min_point.get_y());
    diagonal.set_z(max_point.get_z() - min_point.get_z());
}

float BoundingBox::intersect(Ray &a_ray)
{
    // initialize distances for near and far intersections
    float dist_adj = -FLT_MAX;
    float dist_far = FLT_MAX;

    // calculate inverse direction for axes
    float inv_dir[3] = {1 / a_ray.direction.get_x(), 1 / a_ray.direction.get_y(), 1 / a_ray.direction.get_z()};

    // calculate intersection distances for min and max points
    float dist_min[3] = {inv_dir[0] * (min_point.get_x() - a_ray.origin.get_x()),
                         inv_dir[1] * (min_point.get_y() - a_ray.origin.get_y()),
                         inv_dir[2] * (min_point.get_z() - a_ray.origin.get_z())};
    float dist_max[3] = {inv_dir[0] * (max_point.get_x() - a_ray.origin.get_x()),
                         inv_dir[1] * (max_point.get_y() - a_ray.origin.get_y()),
                         inv_dir[2] * (max_point.get_z() - a_ray.origin.get_z())};

    for (int i = 0; i < 3; i++)
    {
        // swap min and max distances if the direction is negative
        if (inv_dir[i] < 0)
            swap(dist_min[i], dist_max[i]);

        // update nearest and farthest distances
        dist_adj = max(dist_min[i], dist_adj);
        dist_far = min(dist_max[i], dist_far);

        // no intersection, if the nearest distance is greater than the farthest
        if (dist_far < dist_adj)
        {
            return FLT_MAX;
        }
    }

    if (dist_adj < 0)
        swap(dist_adj, dist_far);

    // return  nearest intersection
    return dist_adj;
}

void BoundingBox::box_transformation(const Matrix4 &a_transformation_matrix)
{
    vector<Vector3> vertices;
    get_box_points(vertices);

    int i = 1;
    for (auto it = vertices.begin(); it != vertices.end(); ++it)
    {
        auto &vertex = *it;
        vertex = point_multiplication(a_transformation_matrix, vertex);
        if (it == vertices.begin())
        {
            min_point = vertex;
            max_point = vertex;
            continue;
        }

        // update min max points based on transformed vertices
        min_point.set_x(min(min_point.get_x(), vertex.get_x()));
        min_point.set_y(min(min_point.get_y(), vertex.get_y()));
        min_point.set_z(min(min_point.get_z(), vertex.get_z()));

        max_point.set_x(max(max_point.get_x(), vertex.get_x()));
        max_point.set_y(max(max_point.get_y(), vertex.get_y()));
        max_point.set_z(max(max_point.get_z(), vertex.get_z()));
    }
}

void BoundingBox::get_box_points(vector<Vector3> &vertices)
{
    float low_coordinates[3] = {min_point.get_x(), min_point.get_y(), min_point.get_z()};
    float high_coordinates[3] = {max_point.get_x(), max_point.get_y(), max_point.get_z()};

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                float x = (i == 0) ? low_coordinates[0] : high_coordinates[0];
                float y = (j == 0) ? low_coordinates[1] : high_coordinates[1];
                float z = (k == 0) ? low_coordinates[2] : high_coordinates[2];

                vertices.push_back(Vector3(x, y, z));
            }
        }
    }
};

