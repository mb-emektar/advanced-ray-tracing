#include "shape.h"

#include "scene.h"
#include "vertex.h"
#include <algorithm> // for for_each
#include <chrono>

#define EPSILON 0.000001

#define EPSILON_REFRACT 0.0006

void Shape::calculate_matrices()
{
}

Ray Shape::ray_transformation(Ray &ray)
{
    // Transform ray to local coordinates.
    Ray ray_transformed = ray;

    if (motion_blur == false)
    {
        if (transform == false)
            return ray_transformed;
        else
        {
            ray_transformed.origin = point_multiplication(transform_matrix_inverse, ray.origin);
            ray_transformed.direction = vector_multiplication(transform_matrix_inverse, ray.direction);
        }
    }
    else
    {
        Vector3 blur_istance = motion_blur_vector * ray.time;
        Translation translation = Translation();
        translation.m_translation_vector = blur_istance;
        translation.calculate_matrix();
        Matrix4 translation_matrix_inverse = inverse(translation.get_transformation_matrix());

        if (transform == true)
            translation_matrix_inverse = transform_matrix_inverse * translation_matrix_inverse;

        ray_transformed.origin = point_multiplication(translation_matrix_inverse, ray.origin);
        ray_transformed.direction = vector_multiplication(translation_matrix_inverse, ray.direction);
    }

    ray_transformed.inside_obj = ray.inside_obj;
    ray_transformed.time = ray.time;

    return ray_transformed;
}

Intersection Shape::intersection_transformation(Ray &ray)
{
    Intersection ts_intersection = ray.intersection;
    if (motion_blur == false)
    {
        if (transform == false)
            return ts_intersection;
        else
        {
            ts_intersection.intersection_point =
                point_multiplication(transformation_matrix, ts_intersection.intersection_point);
            ts_intersection.normal =
                (vector_multiplication(transform_matrix_normal, ts_intersection.normal)).unit_vector();
        }
    }
    else
    {
        if (transform == false)
        {
            Translation translation = Translation();
            translation.m_translation_vector = motion_blur_vector * ray.time;
            translation.calculate_matrix();
            Matrix4 motion_blur_matrix = translation.get_transformation_matrix();

            ts_intersection.intersection_point =
                point_multiplication(motion_blur_matrix, ts_intersection.intersection_point);
            ts_intersection.normal =
                vector_multiplication(calculate_transpose(inverse(motion_blur_matrix)), ts_intersection.normal);
            ts_intersection.normal.normalize();
        }
        else
        {
            Translation translation = Translation();
            translation.m_translation_vector = motion_blur_vector * ray.time;
            translation.calculate_matrix();

            Matrix4 motion_blur_matrix = translation.get_transformation_matrix();

            ts_intersection.intersection_point =
                point_multiplication(motion_blur_matrix * transformation_matrix, ts_intersection.intersection_point);
            ts_intersection.normal = vector_multiplication(
                calculate_transpose(transform_matrix_inverse * inverse(motion_blur_matrix)), ts_intersection.normal);
            ts_intersection.normal.normalize();
        }
    }
    ray.intersection = ts_intersection;
    return ts_intersection;
}

BVH::BVH(vector<shared_ptr<Shape>> &shapes, int split_axis, int start_index, int end_index)
{
    transform = false;
    motion_blur = false;
    material_id = -1;
    transformation_matrix = Matrix4();
    transform_matrix_inverse = Matrix4();
    transform_matrix_normal = Matrix4();
    motion_blur_vector = Vector3();
    texture = nullptr;
    np_texture = nullptr;

    int num_shapes = end_index - start_index;

    if (num_shapes > 2)
    {
        int mid_index = start_index;
        float centroid;

        for (int i = start_index; i < end_index; i++)
            bounding_box.expand(shapes[i]->get_bounding_box());

        if (split_axis == 0)
            centroid = bounding_box.centroid.get_x();
        else if (split_axis == 1)
            centroid = bounding_box.centroid.get_y();
        else if (split_axis == 2)
            centroid = bounding_box.centroid.get_z();

        for (int i = start_index; i < end_index; i++)
        {
            float objectCenter;
            if (split_axis == 0)
                objectCenter = shapes[i]->get_bounding_box().centroid.get_x();
            else if (split_axis == 1)
                objectCenter = shapes[i]->get_bounding_box().centroid.get_y();
            else if (split_axis == 2)
                objectCenter = shapes[i]->get_bounding_box().centroid.get_z();
            if (centroid > objectCenter)
            {
                swap(shapes[i], shapes[mid_index]);
                mid_index++;
            }
        }
        if (start_index == mid_index || end_index == mid_index)
            mid_index = start_index + (end_index - start_index) / 2;

        p_left_child = make_shared<BVH>(BVH(shapes, (split_axis + 1) % 3, start_index, mid_index));
        p_right_child = make_shared<BVH>(BVH(shapes, (split_axis + 1) % 3, mid_index, end_index));
    }
    else if (num_shapes == 2)
    {
        p_right_child = shapes[end_index - 1];
        p_left_child = shapes[start_index];
        bounding_box = p_left_child->get_bounding_box();
        bounding_box.expand(p_right_child->get_bounding_box());
    }
    else if (num_shapes == 1)
    {
        p_right_child = nullptr;
        p_left_child = shapes[start_index];
        bounding_box = p_left_child->get_bounding_box();
    }
    else if (num_shapes == 0)
    {
        p_right_child = nullptr;
        p_left_child = nullptr;
        bounding_box = BoundingBox();
    }
}

Intersection BVH::intersect(Ray &ray)
{
    float distance = bounding_box.intersect(ray);

    if (distance == FLT_MAX || distance < 0)
    {
        return ray.intersection;
    }

    if (p_left_child != nullptr)
    {
        Intersection left_intersection = p_left_child->intersect(ray);

        if (left_intersection.does_hit && (left_intersection.distance > 0) &&
            (ray.intersection.distance > left_intersection.distance))
        {
            ray.intersection = left_intersection;
        }
    }
    if (p_right_child != nullptr)
    {
        Ray temp_ray = Ray();
        temp_ray = ray;
        Intersection intersection = Intersection();
        temp_ray.intersection = intersection;
        Intersection right_intersection = p_right_child->intersect(temp_ray);

        if (right_intersection.does_hit && (right_intersection.distance > 0) &&
            (ray.intersection.distance > right_intersection.distance))
            ray.intersection = right_intersection;
    }
    return ray.intersection;
}

BVH::~BVH()
{
}

Sphere::Sphere(Scene *a_pScene)
{
    p_scene = a_pScene;
    center_vertex_id = 0;
    radius = 0;
    transform = false;
    motion_blur = false;
    material_id = -1;
    transformation_matrix = Matrix4();
    transform_matrix_inverse = Matrix4();
    transform_matrix_normal = Matrix4();
    motion_blur_vector = Vector3();
    texture = nullptr;
    np_texture = nullptr;
}

void Sphere::calculate_properties()
{
    Vector3 centroid = p_scene->vertex_data[center_vertex_id].position;
    Vector3 r = Vector3(radius);
    Vector3 min_point = centroid - r;
    Vector3 max_point = centroid + r;

    if (motion_blur == true)
    {
        Vector3 lower_bound = bounding_box.min_point - motion_blur_vector;
        Vector3 upper_bound = bounding_box.max_point + motion_blur_vector;
        BoundingBox temp = BoundingBox(lower_bound, upper_bound);
        bounding_box.expand(temp);
    }

    bounding_box = BoundingBox(min_point, max_point);
    if (transform == true)
    {
        // Transform bounding box to world coords.
        bounding_box.box_transformation(transformation_matrix);
    }

    transform_matrix_inverse = inverse(transformation_matrix);
    transform_matrix_normal = calculate_transpose(transform_matrix_inverse);
}
Sphere::~Sphere()
{
}

Intersection Sphere::intersect(Ray &ray)
{
    Ray ts_ray = ray_transformation(ray);

    Intersection intersection = Intersection();

    Vertex center_vertex = p_scene->vertex_data[center_vertex_id];
    Vector3 center_data = center_vertex.position;

    float val_1, val_2, val_3;
    val_1 = ts_ray.direction.dot_product(ts_ray.direction);
    val_2 = 2 * ts_ray.direction.dot_product(ts_ray.origin - center_data);
    Vector3 temp = ts_ray.origin - center_data;
    float temp_dot = temp.dot_product(temp);
    val_3 = temp_dot - pow(radius, 2);

    float discriminant = pow(val_2, 2) - 4 * val_1 * val_3;
    // checks intersection
    if (discriminant >= 0)
    {
        float t1, t2, distance;

        t1 = (-1 * val_2 + sqrt(discriminant)) / (2 * val_1);
        t2 = (-1 * val_2 - sqrt(discriminant)) / (2 * val_1);

        distance = min(t1, t2);
        if (t1 - EPSILON_REFRACT < 0)
            distance = t2;
        else if (t2 < 0)
            distance = t1;

        Vector3 intersection_point = ts_ray.calculate(distance);

        intersection.does_hit = true;
        intersection.p_shape = this;
        intersection.distance = distance;
        intersection.normal = (intersection_point - center_vertex.position).unit_vector();
        intersection.material_id = this->material_id;
        intersection.intersection_point = ts_ray.calculate(distance);
        intersection.texture = texture;

        Vector3 converted_point = intersection.intersection_point - center_data;
        if (texture)
            intersection.texture_vector = get_tex_coordinates(converted_point, texture);

        if (np_texture)
        {
            Vector2 normal_texture_vector = get_tex_coordinates(converted_point, np_texture);
            Matrix3 matrix_TBN = calculate_TBN(converted_point, intersection.normal);
            if (np_texture->is_normal)
                intersection.normal = np_texture->calculate_normal_map_vector(matrix_TBN, normal_texture_vector);
            else if (np_texture->is_bump)
                intersection.normal = np_texture->calculate_bump_vector(matrix_TBN, normal_texture_vector,
                                                                        converted_point, intersection.normal);
        }
        ts_ray.intersection = intersection;
        intersection = intersection_transformation(ts_ray);
    }
    else
    {
    }
    ts_ray.intersection = intersection;
    return intersection;
}
void Sphere::print_info()
{
    cout << "Sphere print_info" << endl;
    cout << "material_id = " << material_id << endl;
    cout << "center_vertex_id = " << center_vertex_id << endl;
    cout << "radius = " << radius << endl;
}

int Sphere::get_material_id()
{
    return material_id;
}
Vector2 Sphere::get_tex_coordinates(Vector3 &point, shared_ptr<Texture> a_texture) const
{
    Vector2 uv = Vector2();

    float theta = acos(point.get_y() / radius);
    float phi = atan2(point.get_z(), point.get_x());
    uv.x = (PI_NUMBER - phi) / (2 * PI_NUMBER);
    uv.y = theta / PI_NUMBER;

    return uv;
}
Matrix3 Sphere::calculate_TBN(Vector3 &point, Vector3 &normal)
{
    Vector3 tangent_col = Vector3();
    Vector3 bitangent_col = Vector3();
    vector<Vector3> cols;

    tangent_col.set_x(2 * PI_NUMBER * point.get_z());
    tangent_col.set_y(0);
    tangent_col.set_z(-2 * PI_NUMBER * point.get_x());

    bitangent_col.set_x(point.get_y() * cos(atan2(point.get_z(), point.get_x())) * PI_NUMBER);
    bitangent_col.set_y(-1 * radius * sin(acos(point.get_y() / radius)) * PI_NUMBER);
    bitangent_col.set_z(point.get_y() * sin(atan2(point.get_z(), point.get_x())) * PI_NUMBER);

    if (np_texture->is_normal)
    {
        tangent_col.normalize();
        bitangent_col.normalize();
    }
    cols.push_back(tangent_col);
    cols.push_back(bitangent_col);
    cols.push_back(normal);
    Matrix3 matrix_tbn = Matrix3(cols);
    return matrix_tbn;
}

Triangle::Triangle(Scene *a_pScene)
{
    p_scene = a_pScene;
    smooth = false;
    debug_count = 0;
    v_offset = 0;
    matrix_TBN = Matrix3();
    t_offset = 0;
    material_id = -1;
    transform = false;
    motion_blur = false;
    area = 0;
    vertex_indices = Face();
    texture_indices = Face();
    transformation_matrix = Matrix4();
    transform_matrix_inverse = Matrix4();
    transform_matrix_normal = Matrix4();
    motion_blur_vector = Vector3();
    texture = nullptr;
    np_texture = nullptr;
}

Triangle::~Triangle()
{
}

Intersection Triangle::intersect(Ray &ray)
{
    debug_count++;
    Ray ts_ray = ray_transformation(ray);
    Intersection intersection = Intersection();
    Vector3 org, dir, pos_1, pos_2, pos_3;
    pos_1 = p_scene->vertex_data[vertex_indices.v0].position;
    pos_2 = p_scene->vertex_data[vertex_indices.v1].position;
    pos_3 = p_scene->vertex_data[vertex_indices.v2].position;
    dir = ts_ray.direction;

    float det = determinant(pos_1 - pos_2, pos_1 - pos_3, dir);
    if (det == 0)
        return intersection;

    org = ts_ray.origin;
    float distance = (determinant(pos_1 - pos_2, pos_1 - pos_3, pos_1 - org)) / det;
    if (-EPSILON > distance)
        return intersection;

    float gamma = (determinant(pos_1 - pos_2, pos_1 - org, dir)) / det;
    if ((-EPSILON > gamma) || (gamma > 1 + EPSILON))
        return intersection;

    float beta = (determinant(pos_1 - org, pos_1 - pos_3, dir)) / det;
    if ((-EPSILON > beta) || (beta > (1 + EPSILON - gamma)))
        return intersection;

    if (smooth)
    {
        intersection.normal = p_scene->vertex_data[vertex_indices.v2].normal * gamma +
                              p_scene->vertex_data[vertex_indices.v1].normal * beta +
                              p_scene->vertex_data[vertex_indices.v0].normal * (1 - (beta + gamma));
    }
    else
    {
        intersection.normal = normal;
    }

    intersection.does_hit = true;
    intersection.p_shape = this;
    intersection.distance = distance;
    intersection.material_id = this->material_id;
    intersection.intersection_point = ts_ray.calculate(distance);
    intersection.texture = texture;
    if (texture && texture->texture_type == image)
    {
        intersection.texture_vector = get_tex_coordinates(beta, gamma, texture);
    }
    if (np_texture)
    {
        Vector2 normal_texture_vector = get_tex_coordinates(beta, gamma, np_texture);
        if (np_texture->is_normal)
            intersection.normal = np_texture->calculate_normal_map_vector(matrix_TBN, normal_texture_vector);
        else if (np_texture->is_bump)
            intersection.normal = np_texture->calculate_bump_vector(
                matrix_TBN, normal_texture_vector, intersection.intersection_point, intersection.normal);
    }
    ts_ray.intersection = intersection;
    intersection = intersection_transformation(ts_ray); // mesh e ait olmayan triangle lar için sorun cıkabilir

    ray.intersection = intersection;
    return intersection;
}

void Triangle::print_info()
{
    cout << "Triangle print_info" << endl;
    cout << "material_id = " << material_id << endl;
    cout << "v indices.v0_id = " << vertex_indices.v0 << endl;
    cout << "v indices.v1_id = " << vertex_indices.v1 << endl;
    cout << "v indices.v2_id = " << vertex_indices.v2 << endl;
    cout << "t indices.v0_id = " << texture_indices.v0 << endl;
    cout << "t indices.v1_id = " << texture_indices.v1 << endl;
    cout << "t indices.v2_id = " << texture_indices.v2 << endl;
}

int Triangle::get_material_id()
{
    return material_id;
}

void Triangle::calculate_properties()
{
    Vector3 v1 = p_scene->vertex_data[vertex_indices.v0].position;
    Vector3 v2 = p_scene->vertex_data[vertex_indices.v1].position;
    Vector3 v3 = p_scene->vertex_data[vertex_indices.v2].position;

    area = (v2 - v1).cross_product(v3 - v1).length() / 2;
    normal = (v2 - v1).cross_product(v3 - v1).unit_vector();

    float x_min = min(min(v1.get_x(), v2.get_x()), v3.get_x());
    float y_min = min(min(v1.get_y(), v2.get_y()), v3.get_y());
    float z_min = min(min(v1.get_z(), v2.get_z()), v3.get_z());
    Vector3 min_point = Vector3(x_min, y_min, z_min);

    float x_max = max(max(v1.get_x(), v2.get_x()), v3.get_x());
    float y_max = max(max(v1.get_y(), v2.get_y()), v3.get_y());
    float z_max = max(max(v1.get_z(), v2.get_z()), v3.get_z());
    Vector3 max_point = Vector3(x_max, y_max, z_max);

    bounding_box = BoundingBox(min_point, max_point);
    if (transform == true)
        bounding_box.box_transformation(transformation_matrix);

    transform_matrix_inverse = inverse(transformation_matrix);
    transform_matrix_normal = calculate_transpose(transform_matrix_inverse);
    if (np_texture)
        matrix_TBN = calculate_TBN();
}

float Triangle::determinant(Vector3 v0, Vector3 v1, Vector3 v2)
{
    return (v0.get_x() * (v1.get_y() * v2.get_z() - v2.get_y() * v1.get_z()) +
            v0.get_y() * (v2.get_x() * v1.get_z() - v1.get_x() * v2.get_z()) +
            v0.get_z() * (v1.get_x() * v2.get_y() - v1.get_y() * v2.get_x()));
}
Vector2 Triangle::get_tex_coordinates(float beta, float gamma, shared_ptr<Texture> tex) const
{
    Vector2 uv = Vector2();

    Vector2 coord_X = p_scene->texture_coordinates[texture_indices.v0];
    Vector2 coord_Y = p_scene->texture_coordinates[texture_indices.v1];
    Vector2 coord_Z = p_scene->texture_coordinates[texture_indices.v2];

    uv = coord_X + beta * (coord_Y - coord_X) + gamma * (coord_Z - coord_X);
    return uv;
}

Matrix3 Triangle::calculate_TBN()
{
    Vector3 tangent_col, bitangent_col;
    Vector3 posv0, posv1, posv2;

    Vector2 uv0, uv1, uv2;
    uv0 = p_scene->texture_coordinates[texture_indices.v0];
    uv1 = p_scene->texture_coordinates[texture_indices.v1];
    uv2 = p_scene->texture_coordinates[texture_indices.v2];

    posv0 = p_scene->vertex_data[vertex_indices.v0].position;
    posv1 = p_scene->vertex_data[vertex_indices.v1].position;
    posv2 = p_scene->vertex_data[vertex_indices.v2].position;

    float determinant_A = ((uv1.x - uv0.x) * (uv2.y - uv0.y)) - ((uv1.y - uv0.y) * (uv2.x - uv0.x));

    float inverse_A[2][2] = {0};

    inverse_A[0][0] = (uv2.y - uv0.y) / determinant_A;
    inverse_A[0][1] = -1 * (uv1.y - uv0.y) / determinant_A;
    inverse_A[1][0] = -1 * (uv2.x - uv0.x) / determinant_A;
    inverse_A[1][1] = (uv1.x - uv0.x) / determinant_A;

    tangent_col = (posv1 - posv0) * inverse_A[0][0] + (posv2 - posv0) * inverse_A[0][1];
    bitangent_col = (posv1 - posv0) * inverse_A[1][0] + (posv2 - posv0) * inverse_A[1][1];

    vector<Vector3> cols;
    cols.push_back(tangent_col);
    cols.push_back(bitangent_col);
    cols.push_back(normal);
    Matrix3 matrix_TBN = Matrix3(cols);
    return matrix_TBN;
}
float Triangle::calculate_area( Matrix4& matrix)
{
	// Apply the transformations and calculate the triangle's area.
	Vector3 a = p_scene->vertex_data[vertex_indices.v0].position;
	Vector3 b = p_scene->vertex_data[vertex_indices.v1].position;
	Vector3 c = p_scene->vertex_data[vertex_indices.v2].position;

	Vector3 aNew = point_multiplication(matrix,a);
	Vector3 bNew = point_multiplication(matrix,b); 
	Vector3 cNew = point_multiplication(matrix,c);

	float area = (bNew - aNew).cross_product(cNew - aNew).length() / 2.0f;
	return area;
}

Mesh::Mesh(Scene *a_pScene)
{
    p_scene = a_pScene;
    bvh = nullptr;
    material_id = -1;
    transform = false;
    motion_blur = false;
    transformation_matrix = Matrix4();
    transform_matrix_inverse = Matrix4();
    transform_matrix_normal = Matrix4();
    motion_blur_vector = Vector3();
    texture = nullptr;
    np_texture = nullptr;
}

Mesh::~Mesh()
{
}
void Mesh::calculate_properties()
{
    bvh = new BVH(triangles, 0, 0, triangles.size());

    bounding_box = bvh->get_bounding_box();

    if (transform == true)
        bounding_box.box_transformation(transformation_matrix);

    if (motion_blur == true)
    {
        Vector3 lower_bound = bounding_box.min_point - motion_blur_vector;
        Vector3 upper_bound = bounding_box.max_point + motion_blur_vector;
        BoundingBox temp = BoundingBox(lower_bound, upper_bound);
        bounding_box.expand(temp);
    }

    transform_matrix_inverse = inverse(transformation_matrix);
    transform_matrix_normal = calculate_transpose(transform_matrix_inverse);
}
Intersection Mesh::intersect(Ray &ray)
{
    Intersection intersection = Intersection();
    if (BVH_ON)
    {	
        Ray ray_transformed = ray_transformation(ray);
        /*auto start = chrono::high_resolution_clock::now();*/
        intersection = bvh->intersect(ray_transformed);
        /*auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;*/
        if (intersection.does_hit)
        {
            intersection = intersection_transformation(ray_transformed);
            ray_transformed.intersection.material_id = material_id;
            intersection.material_id = material_id;
            intersection.texture = texture;
            ray.intersection = intersection;
        }
    }
    else
    {
        for (int i = 0; i < triangles.size(); i++)
        {
            intersection = triangles[i]->intersect(ray);
            if (intersection.does_hit && intersection.distance > 0 &&
                intersection.distance < ray.intersection.distance - EPSILON)
            {
                ray.intersection = intersection;
                intersection.texture = texture;
            }
        }
    }

    return intersection;
}

void Mesh::print_info()
{
    cout << "Mesh print_info" << endl;
    for (int i = 0; i < triangles.size(); i++)
    {
        triangles[i]->print_info();
    }
}

int Mesh::get_material_id()
{
    return material_id;
}

MeshInstance::MeshInstance()
{
    p_scene = nullptr;
    material_id = -1;
    transformation_matrix = Matrix4();
    transform = false;
    motion_blur_vector = Vector3();
    motion_blur = false;
    baseMeshBVH = nullptr;
    texture = nullptr;
    np_texture = nullptr;
}

Intersection MeshInstance::intersect(Ray &ray)
{
    Ray ray_transformed = ray_transformation(ray);

    Intersection intersection = baseMeshBVH->intersect(ray_transformed);

    if (intersection.does_hit)
    {
        intersection = intersection_transformation(ray_transformed);
        intersection.material_id = material_id;
        intersection.texture = texture;
    }

    return intersection;
}
MeshInstance::~MeshInstance()
{
    if (baseMeshBVH)
    {
        delete baseMeshBVH;
    }
}

void MeshInstance::print_info()
{
    cout << "MeshInstance print_info" << endl;
}
int MeshInstance::get_material_id()
{
    return -1;
}
void MeshInstance::calculate_properties()
{
    calculate_matrices();
    if (motion_blur == true)
    {
        Vector3 min_point_lb = bounding_box.min_point - motion_blur_vector;
        Vector3 max_point_ub = bounding_box.max_point + motion_blur_vector;
        BoundingBox temp = BoundingBox(min_point_lb, max_point_ub);
        bounding_box.expand(temp);
    }

    bounding_box = baseMeshBVH->get_bounding_box();
    if (transform == true)
    {
        bounding_box.box_transformation(transformation_matrix);
    }
    transform_matrix_inverse = inverse(transformation_matrix);
    transform_matrix_normal = calculate_transpose(transform_matrix_inverse);
}


