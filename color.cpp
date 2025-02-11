#include "color.h"

#include <algorithm>
#include <cmath>

Color::Color() : red(0), green(0), blue(0)
{
}

Color::Color(float a_f) : red(a_f), green(a_f), blue(a_f)
{
}

Color::Color(float a_red, float a_green, float a_blue) : red(a_red), green(a_green), blue(a_blue)
{
}

Color::~Color()
{
}

void Color::clamp()
{
    if (red > 255)
        red = 255;
    else if (red < 0)
        red = 0;

    if (green > 255)
        green = 255;
    else if (green < 0)
        green = 0;

    if (blue > 255)
        blue = 255;
    else if (blue < 0)
        blue = 0;
}

void Color::div_num_samples(int number_of_samples)
{
    red /= number_of_samples;
    green /= number_of_samples;
    blue /= number_of_samples;
}

void Color::gamma_correction(float a_exposure, float a_gamma)
{
    red = pow(red * a_exposure, a_gamma);
    green = pow(green * a_exposure, a_gamma);
    blue = pow(blue * a_exposure, a_gamma);
}

Color Color::operator-(float a_f)
{
    return Color(red - a_f, blue - a_f, green - a_f);
}

Color &Color::operator*=(float a_f)
{
    red *= a_f;
    green *= a_f;
    blue *= a_f;
    return *this;
}
Color &Color::operator/=(float a_f)
{
    red /= a_f;
    green /= a_f;
    blue /= a_f;
    return *this;
}

Color Color::operator-(const Color &a_color)
{
    float r = red - a_color.red;
    float g = green - a_color.green;
    float b = blue - a_color.blue;
    return Color(r, g, b);
}

Color &Color::operator=(const Color &a_color)
{
    red = a_color.red;
    green = a_color.green;
    blue = a_color.blue;
    return *this;
}

Color &Color::operator+=(const Color &a_color)
{
    red += a_color.red;
    green += a_color.green;
    blue += a_color.blue;
    return *this;
}

Color &Color::operator*=(const Color &a_color)
{
    red *= a_color.red;
    green *= a_color.green;
    blue *= a_color.blue;
    return *this;
}

float Color::calculate_average() const
{
    return (red + green + blue) / 3;
}


