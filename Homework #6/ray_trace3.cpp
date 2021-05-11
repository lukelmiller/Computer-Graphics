//---------------------------------------
// Program: ray_trace3.cpp
// Purpose: Demonstrate added features to ray tracing.
// Author:  Luke Miller
// Project: 6
// Date:    Spring 2021
//---------------------------------------
#include <cmath>
#include <cstdio>
#include <cstdlib>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
using namespace std;

// Include ray tracing and phong shading classes
#include "ray_classes.h"

// Global variables
#define SLEEP_TIME 20
#define XDIM 800
#define YDIM 800
#define ZDIM 800
unsigned char image[YDIM][XDIM][3];
float position = -2;
string mode = "phong";
const int SPHERES = 10;
const int LIGHTS = 5;
Sphere3D sphere[SPHERES];
Vector3D light_dir[LIGHTS];
Point3D trag[SPHERES];
bool flag = false;
ColorRGB color[SPHERES];
ColorRGB light_color;


//---------------------------------------
// Calculate random value between [min..max]
//---------------------------------------
float myrand(float min, float max) {
  return rand() * (max - min) / RAND_MAX + min;
}

//---------------------------------------
// Check to see if point is in shadow
//---------------------------------------
bool in_shadow(Point3D pt, Vector3D dir, int current, Sphere3D sphere[],
               int count) {
  // Define ray to light source
  Ray3D shadow_ray;
  shadow_ray.set(pt, dir);

  // Check to see ray intersects any sphere
  Point3D point;
  Vector3D normal;
  for (int index = 0; index < count; index++)
    if ((index != current) &&
        (sphere[index].get_intersection(shadow_ray, point, normal)))
      return true;
  return false;
}

//---------------------------------------
// Initialize Spheres
//---------------------------------------
void init_spheres() {
  // Define array of spheres
  for (int s = 0; s < SPHERES; s++) {
    float cx = myrand(-2, 2);
    float cy = myrand(-2, 2);
    float cz = myrand(3, 6);
    Point3D center;
    center.set(cx, cy, cz);
    float radius = myrand(0.5, 1);
    sphere[s].set(center, radius);
    int R = rand() % 255;
    int G = rand() % 255;
    int B = rand() % 255;
    color[s].set(R, G, B);
  }
}
//---------------------------------------
// Initialize Lights
//---------------------------------------
void init_lights() {
  light_color.set(70, 70, 70);
  // Define array of lights
  for (int l = 0; l < LIGHTS; l++) {
    // Define light direction
    float randx = myrand(-1, 1);
    float randy = myrand(-1, 1);
    float randz = myrand(-1, 1);
    light_dir[l].set(randx, randy, randz);
    light_dir[l].normalize();
  }
}

//---------------------------------------
// Move Spheres
//---------------------------------------
void move_spheres() {
  float distance = .1;

  // Define array of spheres
  for (int s = 0; s < SPHERES; s++) {

    Point3D center = sphere[s].center;
    float radius = sphere[s].radius;
    Point3D newCenter;

    // if the spheres have NOT moved before
    if (!flag) {
      float tragX = myrand(-distance, distance);
      float tragY = myrand(-distance, distance);
      float tragZ = myrand(-distance, distance);
      newCenter.set(center.px + tragX, center.py + tragY, center.pz + tragZ);
      sphere[s].set(newCenter, radius);
      trag[s].set(tragX, tragY, tragZ);
    } else { // if the spheres are moving again

      float tragX = trag[s].px;
      float tragY = trag[s].py;
      float tragZ = trag[s].pz;
      if ((center.px + tragX) > 2.5 || (center.px + tragX) < -2.5)
        tragX *= -1;
      if ((center.py + tragY) > 2.5 || (center.py + tragY) < -2.5)
        tragY *= -1;
      if ((center.pz + tragZ) > 3 || (center.pz + tragZ) < -3)
        tragZ *= -1;

      newCenter.set(center.px + tragX, center.py + tragY, center.pz + tragZ);
      sphere[s].set(newCenter, radius);
      trag[s].set(tragX, tragY, tragZ);
    }
  }
  flag = true;
}

//---------------------------------------
// Perform ray tracing of scene
//---------------------------------------
void ray_trace() {
  // Define camera point
  Point3D camera;
  camera.set(0, 0, position);

  // Define shader
  Phong shader;
  shader.SetCamera(camera);

  // Perform ray tracing
  for (int y = 0; y < YDIM; y++)
    for (int x = 0; x < XDIM; x++) {
      // Clear image
      image[y][x][0] = 0;
      image[y][x][1] = 0;
      image[y][x][2] = 0;

      // Define sample point on image plane
      float xpos = (x - XDIM / 2) * 2.0 / XDIM;
      float ypos = (y - YDIM / 2) * 2.0 / YDIM;
      Point3D point;
      point.set(xpos, ypos, 0);

      // Define ray from camera through image
      Ray3D ray;
      ray.set(camera, point);

      // Perform sphere intersection
      int closest = -1;
      Point3D p, closest_p;
      Vector3D n, closest_n;
      closest_p.set(0, 0, ZDIM);
      for (int s = 0; s < SPHERES; s++) {
        if ((sphere[s].get_intersection(ray, p, n)) && (p.pz < closest_p.pz)) {
          closest = s;
          closest_p = p;
          closest_n = n;
        }
      }

      // Calculate pixel color
      if (closest >= 0) {
        // Display surface normal
        if (mode == "normal") {
          image[y][x][0] = 127 + closest_n.vx * 127;
          image[y][x][1] = 127 + closest_n.vy * 127;
          image[y][x][2] = 127 + closest_n.vz * 127;
        }

        // Calculate Phong shade
        if (mode == "phong") {
          ColorRGB colors;
          colors.set(0, 0, 0);
          for (int l = 0; l < LIGHTS; l++) {
            shader.SetLight(light_color, light_dir[l]);
            // Check to see if in shadow
            if (in_shadow(closest_p, light_dir[l], closest, sphere, SPHERES))
              shader.SetObject(color[closest], 0.4, 0.0, 0.0, 1);
            else
              shader.SetObject(color[closest], 0.4, 0.4, 0.4, 10);

            // Calculate pixel color
            ColorRGB pixel;
            shader.GetShade(closest_p, closest_n, pixel);
            colors.add(pixel);
          }
          image[y][x][0] = colors.R;
          image[y][x][1] = colors.G;
          image[y][x][2] = colors.B;
        }
      }
    }
}

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init() {
  // Initialize OpenGL
  glClearColor(0.0, 0.0, 0.0, 1.0);

  // Perform ray tracing
  cout << "camera: 0,0," << position << endl;
  init_spheres();
  init_lights();
  ray_trace();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display() {
  // Display image
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawPixels(XDIM, YDIM, GL_RGB, GL_UNSIGNED_BYTE, image);
  glFlush();
}
//---------------------------------------
// Timer callback for OpenGL
//---------------------------------------
void timer(int value) {
  move_spheres();
  ray_trace();
  glutPostRedisplay();
  glutTimerFunc(SLEEP_TIME, timer, 0);
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[]) {
  // Create OpenGL window
  glutInit(&argc, argv);
  glutInitWindowSize(XDIM, YDIM);
  glutInitWindowPosition(0, 0);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutCreateWindow("Ray Trace");
  init();

  // Specify callback function
  glutDisplayFunc(display);
  glutTimerFunc(SLEEP_TIME, timer, 0);
  glutMainLoop();
  return 0;
}
