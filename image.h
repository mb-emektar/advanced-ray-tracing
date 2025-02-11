#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "Vector3.h"
#include "color.h"
#include "tonemap.h"
#include <iostream>

#include "lodepng/lodepng.h"

class Image
{
  public:
    // constructor&destructor
    Image();
    Image(const vector<Color> &a_pixel_colors, int a_width, int a_height);
    Image(const string &imageName);

    // functions
    int get_width();
    int get_height();
    void save_as_png();
    void save_image(const string &imageName, const Tonemap &tonemap) const;
    unsigned char get_pixel_color(int a_index);
    void set_pixel_color(int &a_index, Color a_color);

    // for texture
    Color get_texture_img_color(int i, int j);

  private:
    // data
    vector<Color> pixel_colors;
    int width;
    int height;
    unsigned char *data;
    const char *image_name;
    void load_image(const string &a_image_name);
    unsigned char *calculate_tonemap(const Tonemap &tonemap) const;
};

#endif


