# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Raytracer VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define the OpenCV directory path
set(SDL2_DIR C:/Program Files/SDL2)

#find_package( OpenCV REQUIRED )
find_package(SDL2 REQUIRED)

# Library directories
include_directories( C:/Program Files/SDL2/include )
link_directories( C:/Program Files/SDL2/lib)

# Source files
file(GLOB SOURCES "*.cpp" "lodepng/*.cpp" "SDL2/include/*.h" "tinyxml2/*.cpp" "tinyexr/*.h" "tinyexr/*.c")

# Specify the executable target
add_executable(raytracer ${SOURCES})

# Link libraries
target_link_libraries( raytracer mingw32 ${SDL2_LIBRARIES} )
