#include "vector2.h"

// constructors
Vector2::Vector2() : x(0), y(0)
{
}

Vector2::Vector2(float a_x, float a_y) : x(a_x), y(a_y)
{
}

Vector2::Vector2(float a_k) : x(a_k), y(a_k)
{
}

// functions

inline float Vector2::dot_product(const Vector2 &r_v2) const
{
    return x * r_v2.x + y * r_v2.y;
}

inline float Vector2::length() const
{
    return sqrt(pow(x, 2) + pow(y, 2));
}

inline float Vector2::length_sqr() const
{
    return pow(x, 2) + pow(y, 2);
}

inline Vector2 Vector2::unit_vector()
{
    return (*this / this->length());
}
void Vector2::normalize()
{
    Vector2 temp = ((*this) / (this->length()));
    x = temp.x;
    y = temp.y;
}
void Vector2::print_axis()
{
    cout << "x = " << x << ", y = " << y << endl;
}

// operator overloading

Vector2 Vector2::operator-(float r_fl) const
{
    return Vector2(x - r_fl, y - r_fl);
}

Vector2 Vector2::operator+(float r_fl) const
{
    return Vector2(x + r_fl, y + r_fl);
}

Vector2 Vector2::operator/(float r_fl) const
{
    return Vector2(x / r_fl, y / r_fl);
}

Vector2 Vector2::operator*(float r_fl) const
{
    return Vector2(x * r_fl, y * r_fl);
}

Vector2 &Vector2::operator-=(const Vector2 &r_v2)
{
    x -= r_v2.x;
    y -= r_v2.y;
    return *this;
}

Vector2 &Vector2::operator+=(const Vector2 &r_v2)
{
    x += r_v2.x;
    y += r_v2.y;
    return *this;
}

Vector2 &Vector2::operator/=(const Vector2 &r_v2)
{
    x /= r_v2.x;
    y /= r_v2.y;
    return *this;
}

Vector2 &Vector2::operator*=(const Vector2 &r_v2)
{
    x *= r_v2.x;
    y *= r_v2.y;
    return *this;
}

Vector2 Vector2::operator-(const Vector2 &r_v2) const
{
    return Vector2(x - r_v2.x, y - r_v2.y);
}

Vector2 Vector2::operator+(const Vector2 &r_v2) const
{
    return Vector2(x + r_v2.x, y + r_v2.y);
}

Vector2 Vector2::operator/(const Vector2 &r_v2) const
{
    return Vector2(x / r_v2.x, y / r_v2.y);
}

Vector2 Vector2::operator*(const Vector2 &r_v2) const
{
    return Vector2(x * r_v2.x, y * r_v2.y);
}
bool Vector2::operator==(const Vector2 &r_v2) const
{
    return x == r_v2.x && y == r_v2.y;
}

bool Vector2::operator!=(const Vector2 &r_v2) const
{
    return !(*this == r_v2);
}

float &Vector2::operator[](int i)
{
    if (i == 0)
        return x;
    else if (i == 1)
        return y;
}

Vector2 operator*(float t, const Vector2 &v)
{
    return Vector2(v.x * t, v.y * t);
}

Vector2 operator/(float t, const Vector2 &v)
{
    return Vector2(v.x / t, v.y / t);
}

ostream &operator<<(ostream &os, const Vector2 &v)
{
    os << "Vector2(" << v.x << ", " << v.y << ")";
    return os;
}


