# Ray Tracing Performance Enhancement with Multithreading and Real-time Visualization

This repository contains the source code for my final project on optimizing ray tracing rendering performance using various multithreading techniques and implementing real-time visualization of rendered blocks.  The project explores and compares different multithreading approaches, including basic thread configurations and a thread pool implementation, to accelerate the rendering process.  It also addresses the challenges of real-time display using SDL2, detailing the solutions implemented to overcome synchronization and stability issues.

## Key Features

*   Implementation of a ray tracer developed throughout the CENG795 course, incorporating advanced features like BVH, texture mapping, and various lighting models.
*   Comparison of different multithreading techniques: Single Thread, 8 Threads, 20 Threads, and Thread Pool with varying chunk sizes (80x80 and 20x20).
*   Real-time rendering visualization using SDL2, showcasing the progressive rendering of image blocks.  Includes details on overcoming SDL2 threading challenges.
*   Performance analysis and comparison of the different multithreading approaches across a range of complex scenes.
*   Automated compilation and execution using CMake and a batch file for streamlined testing with various input XML files.
*   Detailed report (included in the repository) documenting the project's development, implementation details, performance results, and future work.

## Technologies Used

*   C++11
*   MinGW 14.2.0 (with -O3 optimization)
*   Visual Studio Code
*   CMake
*   SDL2
*   Task Thread Pool Library (https://github.com/alugowski/task-thread-pool)

## Build Instructions (Example)

After installing the same development environment (which is more difficult than writing code), you can simply run create.bat.

## Future Work (Potential)
*   Dynamic Chunk Size Allocation
*   GPU Acceleration (e.g., CUDA, OpenCL)
*   Advanced Thread Pool Strategies (e.g., job stealing)
