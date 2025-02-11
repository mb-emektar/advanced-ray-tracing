#ifndef COLOR_H
#define COLOR_H

struct Color
{
    // data
    float red, green, blue;

    // constructor&destructor
    Color();
    Color(float a_f);
    Color(float a_red, float a_green, float a_blue);

    virtual ~Color();

    // functions
    void clamp();
    void div_num_samples(int number_of_samples);
    void gamma_correction(float a_exposure, float a_gamma);

    // operator overloading
    Color operator-(float a_f);
    Color &operator*=(float a_f);
    Color &operator/=(float a_f);

    Color operator-(const Color &a_color);
    Color &operator=(const Color &a_color);
    Color &operator+=(const Color &a_color);
    Color &operator*=(const Color &a_color);
    float calculate_average() const;
};

inline Color operator+(const Color &l_color, const Color &r_color)
{
    return Color(l_color.red + r_color.red, l_color.green + r_color.green, l_color.blue + r_color.blue);
}

inline Color operator*(const Color &l_color, const Color &r_color)
{
    return Color(l_color.red * r_color.red, l_color.green * r_color.green, l_color.blue * r_color.blue);
}

inline Color operator*(const Color &a_color, float a_f)
{
    return Color(a_color.red * a_f, a_color.green * a_f, a_color.blue * a_f);
}

inline Color operator*(float a_f, const Color &a_color)
{
    return Color(a_f * a_color.red, a_f * a_color.green, a_f * a_color.blue);
}

#endif // COLOR_H


