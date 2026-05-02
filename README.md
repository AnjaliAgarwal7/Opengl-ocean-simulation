OpenGL Ocean Simulation

A real-time ocean surface simulation built using OpenGL, featuring dynamic waves, realistic lighting, and atmospheric effects.

Features: -
1. Dynamic wave simulation using sine and cosine functions
2. Real-time surface normal computation for lighting
3. Fresnel-based shading for realistic reflections
4. Volumetric mist effect using alpha blending
5. Smooth camera movement and animation

Tech Stack: -
1. C++
2. OpenGL (FreeGLUT)

How It Works: -
1. Ocean waves are generated using multiple sine functions
2. Surface normals are calculated using nearby height differences
3. Lighting is applied using a Fresnel approximation
4. Mist is created using layered transparent quads

How to Run: -
1. Install OpenGL and FreeGLUT
2. Compile the code:
   g++ main.cpp -lGL -lGLU -lglut
3. Run the program

My Contribution: -
1. Implemented ocean wave simulation
2. Developed lighting and shading system
3. Created volumetric mist effect
4. Built the complete OpenGL rendering pipeline

Note: -
This project is based on a team research work. This repository contains my implementation of the system.
