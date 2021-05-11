//----------------------------------------------
// Program: ray_classes.h
// Purpose: Definition of several C++ classes
//          to support ray tracing application.
//        
//          ColorRGB - store RGB color values.
//          Point3D - store XYZ point coordinates.
//          Vector3D - store XYZ vector coordinates.
//          Ray3D - store ray (point and vector).
//          Sphere3D - store sphere (point and radius).
//          Phong - implement Phong shading.
//
//    These classes have no private variables so
//    they should be used with care (since there
//    is no data hiding provided).
//    
// Author:  John Gauch
// Date:    Spring 2019
//----------------------------------------------
#include <iostream>
#include <cmath>
using namespace std;

#ifndef _RAY_CLASSES_
#define _RAY_CLASSES_

const bool DEBUG = false;

//----------------------------------------------
class ColorRGB
{
   public:
      float R, G, B;

      void set(float r, float g, float b);
      string print();
      void mult(float c);
      void mult(ColorRGB c);
      void add(ColorRGB c);
      void sub(ColorRGB c);
      void clamp();
};

//----------------------------------------------
class Point3D
{
   public:
      float px, py, pz;

      void set(float x, float y, float z);
      string print();
      float distance(Point3D p);
};

//----------------------------------------------
class Vector3D
{
   public:
      float vx, vy, vz;

      void set(float x, float y, float z);
      string print();
      void normalize();
      float dot(Vector3D v);
      void mult(float c);
      void add(Vector3D v);
      void sub(Vector3D v);
};

//----------------------------------------------
class Ray3D
{
   public:
      Point3D point;
      Vector3D dir;

      void set(Point3D p, Vector3D d);
      void set(Point3D p1, Point3D p2);
      string print();
      Point3D get_sample(float t);
};

//----------------------------------------------
class Sphere3D
{
   public:
      Point3D center;
      float radius;

      void set(Point3D p, float r) ;
      string print() ;
      bool get_intersection(Ray3D ray, Point3D &point, Vector3D &normal);
};

//----------------------------------------------
class Phong
{
 public:
   // Constructors
   Phong();
   ~Phong();

   // Set methods
   void SetCamera(Point3D pos);
   void SetLight(ColorRGB color, Vector3D dir);
   void SetObject(ColorRGB color, float ka, float kd, float ks, float kp);
    
   // Get method
   void GetShade(Point3D point, Vector3D normal, ColorRGB & color);

 private:
   // Camera
   Point3D CameraPos;

   // Light
   ColorRGB LightColor;
   Vector3D LightDir;
   
   // Object
   ColorRGB ObjectColor;
   float Ka, Kd, Ks, Kp;
};

#endif