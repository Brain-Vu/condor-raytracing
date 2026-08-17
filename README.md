# Condor-Raytracing

## Class assignment to render hundreds of ray-traced images into a video using a script that submits jobs to different machines on the Open Science Grid (OSG) Condor pool. 

The project uses Persistence of Vision Raytracer (POV-Ray), an open-source rendering tool, to produce ray-traced frames. The assignment-provided code specified an image of a Rubik's Cube, which I customized into different positions/orientations (as seen below). To create a video, different frames were stitched together with FFmpeg, another open-source tool. The challenge was to create a script to render a 1-minute video at increasingly higher aspect ratios (480p, 720p, up to 1080p) at 10 fps within a reasonable amount of time.

<img width="1280" height="720" alt="cubes" src="https://github.com/user-attachments/assets/c70d3ca9-1964-49ca-9360-602ab0f1b670" />

## Approaches 
There were two approaches to this problem: serial or concurrent rendering. Below, I list the performance of each. 

- **./cpp_source/local_povray.cpp**

  For the 1080p video, a serialized approach of rendering each frame one by one required 1.5+ hours to complete. 

- **./cpp_source/condor_povray.cpp**

  This script submits 600 job requests (10 fps * 60 seconds = 600 frames to render) to the OSG Condor pool, a collaborative collection of idle machines "donated" from various academic institutions for free use. This approach allows for concurrent rendering and thus, faster times. For the 1080p video, this took about 10 minutes.  

<img width="975" height="561" alt="image" src="https://github.com/user-attachments/assets/8a0102f3-dd16-4cdb-a877-d442faa16756" />

Note to self: Probably don't use C++ to script. It was super annoying to keep recompiling.
