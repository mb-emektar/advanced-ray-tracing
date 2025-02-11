#ifndef TONEMAP_H_
#define TONEMAP_H_

#include <string>

using namespace std;

enum tmo_type
{
    none = 0,
    photographic
};

class Tonemap
{
  public:
    tmo_type type;
    float key, gamma;
    float burn_ration;
    float saturation;

    Tonemap();
    Tonemap(string &tmo_type);
};

#endif


