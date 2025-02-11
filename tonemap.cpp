#include "tonemap.h"

Tonemap::Tonemap()
{
    type = none;
    key = 0;
    gamma = 0;
    burn_ration = 0;
    saturation = 0;
}

Tonemap::Tonemap(string &tmo_type)
{
    if (tmo_type == "Photographic")
        type = photographic;
    else
        type = none;
}


