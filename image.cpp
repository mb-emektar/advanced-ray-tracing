#include "image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb-image/stb_image.h"
#include "stb-image/stb_image_write.h"
#define TINYEXR_USE_STB_ZLIB (1)
#define TINYEXR_IMPLEMENTATION
// #include "zlib.h"
#include "tinyexr/tinyexr.h"
#define EPSILON 0.00001

Image::Image()
{
}
Image::Image(const vector<Color> &a_pixel_colors, int a_width, int a_height)
{
    width = a_width;
    height = a_height;
    pixel_colors = a_pixel_colors;
}

Image::Image(const string &a_image_name)
{
    load_image(a_image_name);
}

int Image::get_width()
{
    return width;
}
int Image::get_height()
{
    return height;
}

/**
 * Save file as png
 */
void Image::save_as_png()
{
    unsigned error = lodepng_encode24_file(image_name, data, width, height);
    if (error)
        cerr << "Error " << error << ": " << lodepng_error_text(error);
}

void Image::save_image(const string &a_image_name, const Tonemap &tonemap) const
{
    int pos = a_image_name.find_last_of(".");
    string extension = a_image_name.substr(pos + 1);
    string onlyImageName = a_image_name.substr(0, pos);
    string pngImageName = onlyImageName + ".png";
    string exrImageName = onlyImageName + ".exr";

    if (extension == "exr")
    {
        EXRHeader header;
        InitEXRHeader(&header);

        EXRImage image_exr;
        InitEXRImage(&image_exr);

        image_exr.num_channels = 4;

        vector<float> images[4];
        images[0].resize(width * height);
        images[1].resize(width * height);
        images[2].resize(width * height);
        images[3].resize(width * height);

        for (int i = 0; i < width * height; i++)
        {
            images[0][i] = pixel_colors[i].red;
            images[1][i] = pixel_colors[i].green;
            images[2][i] = pixel_colors[i].blue;
            images[3][i] = 1;
        }

        float *image_ptr[4];
        image_ptr[0] = &(images[3].at(0)); // B
        image_ptr[1] = &(images[2].at(0)); // G
        image_ptr[2] = &(images[1].at(0)); // R
        image_ptr[3] = &(images[0].at(0)); // A

        image_exr.images = (unsigned char **)image_ptr;
        image_exr.width = width;
        image_exr.height = height;

        header.num_channels = 4;
        header.channels = (EXRChannelInfo *)malloc(sizeof(EXRChannelInfo) * header.num_channels);
        // Must be BGR(A) order, since most of EXR viewers expect this channel order.
        strncpy(header.channels[0].name, "B", 255);
        header.channels[0].name[strlen("B")] = '\0';
        strncpy(header.channels[1].name, "G", 255);
        header.channels[1].name[strlen("G")] = '\0';
        strncpy(header.channels[2].name, "R", 255);
        header.channels[2].name[strlen("R")] = '\0';
        strncpy(header.channels[3].name, "A", 255);
        header.channels[3].name[strlen("A")] = '\0';

        header.pixel_types = (int *)malloc(sizeof(int) * header.num_channels);
        header.requested_pixel_types = (int *)malloc(sizeof(int) * header.num_channels);
        for (int i = 0; i < header.num_channels; i++)
        {
            header.pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT; // pixel type of input image
            header.requested_pixel_types[i] =
                TINYEXR_PIXELTYPE_FLOAT; // pixel type of output image to be stored in .EXR
        }

        const char *err;
        int ret = SaveEXRImageToFile(&image_exr, &header, exrImageName.c_str(), &err);
        if (ret != TINYEXR_SUCCESS)
        {
            fprintf(stderr, "Save EXR err: %s\n", err);
        }
        printf("Saved exr file. [ %s ] \n", exrImageName.c_str());

        free(header.channels);
        free(header.pixel_types);
        free(header.requested_pixel_types);
    }
    unsigned char *p_data = calculate_tonemap(tonemap);
    unsigned error = lodepng_encode24_file(pngImageName.c_str(), p_data, width, height);
    if (error)
        cerr << "Error " << error << ": " << lodepng_error_text(error);
}

unsigned char *Image::calculate_tonemap(const Tonemap &tonemap) const
{
    // initialize color index and allocate memory for the image data
    int color_index = 0;
    unsigned char *p_data = new unsigned char[3 * height * width];

    // check the type of tonemapping to be applied
    if (tonemap.type == none)
    {
        // no tonemapping, clamp color values and store them in p_data
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                Color pixelColor = pixel_colors[i * width + j];
                pixelColor.clamp();
                p_data[color_index++] = pixelColor.red;
                p_data[color_index++] = pixelColor.green;
                p_data[color_index++] = pixelColor.blue;
            }
        }
    }
    else if (tonemap.type == photographic)
    {
        // photographic tonemapping, calculate world luminances and tonemapped luminances
        int number_of_pixels = width * height;
        vector<float> world_lums(number_of_pixels, 0), scaled_lums(number_of_pixels, 0);

        // calculate the sum of logarithms of world luminances
        float sum_log_world_lum = 0;
        for (int i = 0; i < number_of_pixels; i++)
        {
            Color rgb = pixel_colors[i];
            world_lums[i] = (0.2126) * rgb.red + (0.7152) * rgb.green + (0.0722) * rgb.blue;
            sum_log_world_lum += log(EPSILON + world_lums[i]);
        }

        // calculate scaled luminance for each pixel
        for (int i = 0; i < number_of_pixels; i++)
            scaled_lums[i] = (tonemap.key * world_lums[i]) / (exp((1.0 / number_of_pixels) * sum_log_world_lum));

        // determine the white point for burnout
        float burn_ration = (100 - tonemap.burn_ration) / 100;
        int white_index = (int)(number_of_pixels * burn_ration) - 1;

        float white_lum;
        vector<float> sorted_lums(scaled_lums);
        sort(sorted_lums.begin(), sorted_lums.end());
        if (white_index < 0)
            white_lum = sorted_lums[0];
        else
            white_lum = sorted_lums[white_index];

        // apply tone mapping to each pixel
        vector<float> result_lums(number_of_pixels, 0);
        for (int i = 0; i < number_of_pixels; i++)
            result_lums[i] = (scaled_lums[i] * (1 + (scaled_lums[i] / (white_lum * white_lum)))) / (1 + scaled_lums[i]);

        // convert final luminances to RGB and apply gamma correction
        for (int i = 0; i < number_of_pixels; i++)
        {
            float red = result_lums[i] * pow((pixel_colors[i].red / world_lums[i]), tonemap.saturation);
            float green = result_lums[i] * pow((pixel_colors[i].green / world_lums[i]), tonemap.saturation);
            float blue = result_lums[i] * pow((pixel_colors[i].blue / world_lums[i]), tonemap.saturation);
            Color color;

            color.red = pow(red, 1 / tonemap.gamma) * 255;
            color.green = pow(green, 1 / tonemap.gamma) * 255;
            color.blue = pow(blue, 1 / tonemap.gamma) * 255;
            color.clamp();

            p_data[color_index++] = color.red;
            p_data[color_index++] = color.green;
            p_data[color_index++] = color.blue;
        }
    }

    return p_data;
}

unsigned char Image::get_pixel_color(int a_index)
{
    return data[a_index];
}

void Image::set_pixel_color(int &a_index, Color a_color)
{
    data[a_index++] = a_color.red;
    data[a_index++] = a_color.green;
    data[a_index++] = a_color.blue;
}

Color Image::get_texture_img_color(int i, int j)
{
    return pixel_colors[j * width + i];
}

void Image::load_image(const string &a_image_name)
{
    int pos = a_image_name.find_last_of(".");
    string file_type = a_image_name.substr(pos + 1);
    float red, green, blue;
    if (file_type == "exr")
    {
        float *p_data;
        const char *err = NULL;
        EXRImage exr_image;
        InitEXRImage(&exr_image);

        LoadEXR(&p_data, &width, &height, a_image_name.c_str(), &err);
        if (err)
        {
            fprintf(stderr, "ERR : %s\n", err);
            FreeEXRErrorMessage(err);
        }

        pixel_colors = vector<Color>(width * height, Color());
        for (int i = 0; i < width * height; i++)
        {
            red = p_data[(i * 4)];
            green = p_data[(i * 4) + 1];
            blue = p_data[(i * 4) + 2];
            Color pixel_color = Color(red, green, blue);
            pixel_colors[i] = pixel_color;
        }

        free(p_data);
    }
    else // png
    {
        int channel_numbers;
        unsigned char *p_data = stbi_load(a_image_name.c_str(), &width, &height, &channel_numbers, 3);

        pixel_colors = vector<Color>(width * height, Color());
        for (int i = 0; i < width * height; i++)
        {
            red = p_data[(i * 3)];
            green = p_data[(i * 3) + 1];
            blue = p_data[(i * 3) + 2];
            Color pixel_color = Color(red, green, blue);
            pixel_colors[i] = pixel_color;
        }

        free(p_data);
    }
}
