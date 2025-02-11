#include "texture.h"

Texture::Texture()
{
    is_normal = false;
    is_bump = false;
}
void Texture::find_decal_mode(const string &a_decal_mode)
{
    if (a_decal_mode == "replace_all")
        decal_mode = replace_all;
    else if (a_decal_mode == "replace_normal")
    {
        decal_mode = replace_normal;
        is_normal = true;
    }
    else if (a_decal_mode == "replace_background")
        decal_mode = replace_bg;
    else if (a_decal_mode == "replace_kd")
        decal_mode = replace_kd;
    else if (a_decal_mode == "replace_ks")
        decal_mode = replace_ks;
    else if (a_decal_mode == "bump_normal")
    {
        decal_mode = replace_bump_normal;
        is_bump = true;
    }
    else if (a_decal_mode == "blend_kd")
        decal_mode = blend_kd;
}

