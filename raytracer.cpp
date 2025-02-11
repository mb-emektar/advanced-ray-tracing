#include <iostream>

#include "ray.h"
#include "scene.h"
#include "vector3.h"

int main(int argc, char *argv[])

{
    Scene scene = Scene();
    // take scene as an input xml file
    scene.loadFromXml(argv[1]);

    scene.process_scene();

    return 0;
}


