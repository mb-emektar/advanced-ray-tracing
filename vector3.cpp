#include "vector3.h"

// constructors
Vector3::Vector3() : x(0), y(0), z(0)
{
}

Vector3::Vector3(float a_x, float a_y, float a_z) : x(a_x), y(a_y), z(a_z)
{
}

Vector3::Vector3(float a_k) : x(a_k), y(a_k), z(a_k)
{
}

// getters
float Vector3::get_x()
{
    return x;
}
float Vector3::get_y()
{
    return y;
}
float Vector3::get_z()
{
    return z;
}

// setters
void Vector3::set_x(float a_x)
{
    x = a_x;
}
void Vector3::set_y(float a_y)
{
    y = a_y;
}
void Vector3::set_z(float a_z)
{
    z = a_z;
}

float Vector3::dot_product(const Vector3 &v) const
{
    // kahan algorithm to be more precise
    double sum = 0.0;
    double c = 0.0;

    auto kahan_sum = [](double &sum, double &c, double value) {
        double y = value - c;
        double t = sum + y;
        c = (t - sum) - y;
        sum = t;
    };

    kahan_sum(sum, c, static_cast<double>(x) * static_cast<double>(v.x));
    kahan_sum(sum, c, static_cast<double>(y) * static_cast<double>(v.y));
    kahan_sum(sum, c, static_cast<double>(z) * static_cast<double>(v.z));

    return sum;
}

// functions

Vector3 Vector3::unit_vector()
{
    return (*this / this->length());
}
void Vector3::normalize()
{
    Vector3 temp = ((*this) / (this->length()));
    x = temp.get_x();
    y = temp.get_y();
    z = temp.get_z();
}

Vector3 Vector3::cross_product(const Vector3 &v) const
{
    float vx = y * v.z - z * v.y;
    float vy = z * v.x - x * v.z;
    float vz = x * v.y - y * v.x;
    Vector3 output = Vector3(vx, vy, vz);
    return output;
}
float Vector3::length() const
{
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

float Vector3::length_sqr() const
{
    return pow(x, 2) + pow(y, 2) + pow(z, 2);
}

int Vector3::get_smallest_index()
{
    int smallest_index = 0;

    if (abs(y) < abs(x))
        smallest_index = 1;

    if (abs(z) < abs(x) || abs(z) < abs(y))
        smallest_index = 2;

    return smallest_index;
}
void Vector3::print_axis()
{
    cout << "x = " << x << ", y = " << y << ", z = " << z << endl;
}

// operator overloading

Vector3 Vector3::operator-(float r_fl) const
{
    return Vector3(x - r_fl, y - r_fl, z - r_fl);
}
Vector3 Vector3::operator+(float r_fl) const
{
    return Vector3(x + r_fl, y + r_fl, z + r_fl);
}

Vector3 Vector3::operator/(float r_fl) const
{
    return Vector3(x / r_fl, y / r_fl, z / r_fl);
}

Vector3 Vector3::operator*(float r_fl) const
{
    return Vector3(x * r_fl, y * r_fl, z * r_fl);
}

Vector3 &Vector3::operator-=(const Vector3 &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

Vector3 &Vector3::operator+=(const Vector3 &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vector3 &Vector3::operator/=(const Vector3 &v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
}

Vector3 &Vector3::operator*=(const Vector3 &v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}

Vector3 Vector3::operator-(const Vector3 &v) const
{
    // Perform subtraction using double precision for better accuracy
    double new_x = static_cast<double>(x) - static_cast<double>(v.x);
    double new_y = static_cast<double>(y) - static_cast<double>(v.y);
    double new_z = static_cast<double>(z) - static_cast<double>(v.z);

    // Return a new Vector3 with the computed values
    return Vector3(static_cast<float>(new_x), static_cast<float>(new_y), static_cast<float>(new_z));
}

Vector3 Vector3::operator+(const Vector3 &v) const
{
    return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator/(const Vector3 &v) const
{
    return Vector3(x / v.x, y / v.y, z / v.z);
}

Vector3 Vector3::operator*(const Vector3 &v) const
{
    return Vector3(x * v.x, y * v.y, z * v.z);
}

bool Vector3::operator==(const Vector3 &v) const
{
    return x == v.x && y == v.y && z == v.z;
}

bool Vector3::operator!=(const Vector3 &v) const
{
    return !(*this == v);
}

Vector3 operator*(float t, const Vector3 &v)
{
    return Vector3(v.x * t, v.y * t, v.z * t);
}

Vector3 operator/(float t, const Vector3 &v)
{
    return Vector3(v.x / t, v.y / t, v.z / t);
}
float Vector3::getAvg() const
{
    return (x + y + z) / 3;
}
ostream &operator<<(ostream &os, Vector3 &v)
{
    os << "Vector3(" << v.get_x() << ", " << v.get_y() << ", " << v.get_z() << ")";
    return os;
}


