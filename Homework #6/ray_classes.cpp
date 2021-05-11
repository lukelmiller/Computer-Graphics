//----------------------------------------------
// Program: ray_classes.cpp
// Purpose: Implementation of several C++ classes
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
#include "ray_classes.h"

void ColorRGB::set(float r, float g, float b) 
{ 
   R = r; 
   G = g; 
   B = b; 
   clamp();
}

//----------------------------------------------
string ColorRGB::print() 
{ 
   cout << "(" << int(R) << "," << int(G) << "," << int(B) << ")"; 
   return "";
}

//----------------------------------------------
void ColorRGB::mult(float c)
{ 
   R *= c; 
   G *= c; 
   B *= c; 
   clamp();
}

//----------------------------------------------
void ColorRGB::mult(ColorRGB c)
{ 
   R *= c.R/255; 
   G *= c.G/255; 
   B *= c.B/255; 
   clamp();
}

//----------------------------------------------
void ColorRGB::add(ColorRGB c)
{ 
   R += c.R; 
   G += c.G; 
   B += c.B; 
   clamp();
}

//----------------------------------------------
void ColorRGB::sub(ColorRGB c)
{ 
   R -= c.R; 
   G -= c.G; 
   B -= c.B; 
   clamp();
}

//----------------------------------------------
void ColorRGB::clamp()
{ 
   if (R < 0) R = 0;
   if (G < 0) G = 0;
   if (B < 0) B = 0;
   if (R > 255) R = 255;
   if (G > 255) G = 255;
   if (B > 255) B = 255;
}

//----------------------------------------------
void Point3D::set(float x, float y, float z) 
{ 
   px = x; 
   py = y; 
   pz = z; 
}

//----------------------------------------------
string Point3D::print() 
{ 
   cout << "(" << px << "," << py << "," << pz << ")"; 
   return "";
}

//----------------------------------------------
float Point3D::distance(Point3D p)
{ 
   float dx = px-p.px; 
   float dy = py-p.py; 
   float dz = pz-p.pz;
   return sqrt(dx*dx + dy*dy + dz*dz); 
}

//----------------------------------------------
void Vector3D::set(float x, float y, float z) 
{ 
   vx = x; 
   vy = y; 
   vz = z; 
}

//----------------------------------------------
string Vector3D::print() 
{ 
   cout << "(" << vx << "," << vy << "," << vz << ")"; 
   return "";
}

//----------------------------------------------
void Vector3D::normalize()
{ 
   float length = sqrt(vx*vx + vy*vy + vz*vz); 
   vx /= length; 
   vy /= length; 
   vz /= length; 
}

//----------------------------------------------
float Vector3D::dot(Vector3D v)
{ 
   return vx*v.vx + vy*v.vy + vz*v.vz; 
}

//----------------------------------------------
void Vector3D::mult(float c)
{ 
   vx *= c;
   vy *= c;
   vz *= c;
}

//----------------------------------------------
void Vector3D::add(Vector3D v)
{ 
   vx += v.vx;
   vy += v.vy;
   vz += v.vz;
}

//----------------------------------------------
void Vector3D::sub(Vector3D v)
{ 
   vx -= v.vx;
   vy -= v.vy;
   vz -= v.vz;
}

//----------------------------------------------
void Ray3D::set(Point3D p, Vector3D d) 
{ 
   point = p; 
   dir = d; 
   dir.normalize(); 
}

//----------------------------------------------
void Ray3D::set(Point3D p1, Point3D p2) 
{ 
   point = p1; 
   dir.set(p2.px-p1.px, p2.py-p1.py, p2.pz-p1.pz); 
   dir.normalize(); 
}

//----------------------------------------------
string Ray3D::print() 
{ 
   point.print(); cout << " "; dir.print(); 
   return "";
}

//----------------------------------------------
Point3D Ray3D::get_sample(float t)
{ 
   Point3D result; 
   result.set(point.px+t*dir.vx, point.py+t*dir.vy, point.pz+t*dir.vz); 
   return result; 
}

//----------------------------------------------
void Sphere3D::set(Point3D p, float r) 
{ 
   center = p; 
   radius = r; 
}

//----------------------------------------------
string Sphere3D::print() 
{ 
   center.print(); 
   cout << " " << radius; 
   return "";
}

//----------------------------------------------
bool Sphere3D::get_intersection(Ray3D ray, Point3D &point, Vector3D &normal)
{  
   // Define oc vector
   Point3D p1 = center; 
   Point3D p2 = ray.point;
   Vector3D oc; 
   oc.set(p2.px-p1.px, p2.py-p1.py, p2.pz-p1.pz); 

   // Calculate quadratic equation 
   float A = ray.dir.dot(ray.dir);
   float B = 2 * oc.dot(ray.dir);
   float C = oc.dot(oc) - radius*radius;
   // cout << "A = " << A << endl;
   // cout << "B = " << B << endl;
   // cout << "C = " << C << endl;

   // Solve quadratic equation for intersection points
   float discriminant = B*B - 4*A*C;
   // cout << "discriminant = " << discriminant << endl;
   if (discriminant >= 0)
   { 
      // Calculate two roots
      float root1 = (-B - sqrt(discriminant)) / 2*A;
      float root2 = (-B + sqrt(discriminant)) / 2*A;
      // cout << "root1 = " << root1 << endl;
      // cout << "root2 = " << root2 << endl;
      float solution = 0;
   
      // No positive roots found
      if ((root1 < 0) && (root2 < 0))
         return false;

      // One positive root
      else if ((root1 < 0) && (root2 >= 0))
         solution = root2;
         
      // One positive root
      else if ((root2 < 0) && (root1 >= 0))
         solution = root1;
         
      // Two positive roots
      else if (root1 <= root2)
         solution = root1;
         
      // Two positive roots
      else if (root2 <= root1)
         solution = root2;
         
      // Get intersection point
      point = ray.get_sample(solution); 
         
      // Get surface normal
      normal.set(point.px-center.px, point.py-center.py, point.pz-center.pz); 
      normal.normalize();
      return true; 
   }
   return false; 
}

//----------------------------------------------
Phong::Phong()
{
   CameraPos.set(0,0,0);
   LightColor.set(0,0,0);
   LightDir.set(0,0,0);
   ObjectColor.set(0,0,0);
   Ka = 0; Kd = 0; Ks = 0; Kp = 0;
}

//----------------------------------------------
Phong::~Phong()
{
}

//----------------------------------------------
void Phong::SetCamera(Point3D pos)
{
   CameraPos = pos;
   if (DEBUG) cout << CameraPos.print() << endl;
}

//----------------------------------------------
void Phong::SetLight(ColorRGB color, Vector3D dir)
{
   LightColor = color;
   if (DEBUG) cout << LightColor.print() << endl;
   LightDir = dir;
   LightDir.normalize();
   if (DEBUG) cout << LightDir.print() << endl;
}

//----------------------------------------------
void Phong::SetObject(ColorRGB color, float ka, float kd, float ks, float kp)
{
   ObjectColor = color;
   if (DEBUG) cout << ObjectColor.print() << endl;
   Ka = ka; Kd = kd; Ks = ks; Kp = kp;
}

//----------------------------------------------
void Phong::GetShade(Point3D point, Vector3D normal, ColorRGB & color)
{
   // Normalize surface normal
   normal.normalize();

   // Calculate ambient term
   ColorRGB ambient;
   ambient = LightColor;
   ambient.mult(ObjectColor);
   ambient.mult(Ka);
   if (DEBUG) cout << "ambient " << ambient.print() << endl;
   color = ambient;

   // Calculate NdotL value
   float NdotL = normal.dot(LightDir);
   if (DEBUG) cout << "NdotL " << NdotL << endl;
   if (NdotL > 0)
   {
      // Calculate diffuse term
      ColorRGB diffuse;
      diffuse = LightColor;
      diffuse.mult(ObjectColor);
      diffuse.mult(Kd * NdotL);
      if (DEBUG) cout << "diffuse " << diffuse.print() << endl;
      color.add(diffuse);

      // Calculate R vector
      Vector3D R = normal;
      R.mult(2 * NdotL);
      R.sub(LightDir);
      R.normalize();
   
      // Calculate V vector
      Vector3D V;
      float Vx = CameraPos.px - point.px;
      float Vy = CameraPos.py - point.py;
      float Vz = CameraPos.pz - point.pz;
      V.set(Vx, Vy, Vz);
      V.normalize();

      // Calculate RdotV value 
      float RdotV = R.dot(V);
      if (DEBUG) cout << "RdotV " << RdotV << endl;
      if (RdotV > 0)
      {
         // Calculate specular term
         RdotV = pow(RdotV, Kp);
         ColorRGB specular;
         specular = LightColor;
         specular.mult(Ks * RdotV);
         if (DEBUG) cout << "specular " << specular.print() << endl;
         color.add(specular);
      }
   }
}

//----------------------------------------------
int test_main()
{
   cout << "\nTest point class\n";
   Point3D p1;
   p1.set(1,2,3);
   cout << "p1 = ";
   p1.print();
   cout << endl;

   Point3D p2;
   p2.set(3,4,5);
   cout << "p2 = ";
   p2.print();
   cout << endl;

   float dist12 = p1.distance(p2);
   float dist21 = p2.distance(p1);
   cout << "dist12 = " << dist12 << endl;
   cout << "dist21 = " << dist21 << endl;

   cout << "\nTest vector class\n";
   Vector3D v1;
   v1.set(2,1,0);
   cout << "v1 = ";
   v1.print();
   cout << endl;

   Vector3D v2;
   v2.set(-1,1,0);
   cout << "v2 = ";
   v2.print();
   cout << endl;

   v2.normalize();
   cout << "v2 = ";
   v2.print();
   cout << endl;

   float v1dotv1 = v1.dot(v1);
   float v1dotv2 = v1.dot(v2);
   float v2dotv2 = v2.dot(v2);
   cout << "v1dotv1 = " << v1dotv1 << endl;
   cout << "v1dotv2 = " << v1dotv2 << endl;
   cout << "v2dotv2 = " << v2dotv2 << endl;

   cout << "\nTest ray class\n";
   Ray3D r1;
   r1.set(p1, v1);
   cout << "r1(p1,v1) = ";
   r1.print();
   cout << endl;

   Ray3D r2;
   r2.set(p2, p1);
   cout << "r2(p2,p1) = ";
   r2.print();
   cout << endl;

   for (int i=0; i<=10; i++)
   {
      cout << "r1(" << i/10.0 << ") = ";
      r1.get_sample(i/10.0).print();
      cout << endl; 
   }

   cout << "\nTest sphere class\n";
   Sphere3D sphere;
   Point3D center;
   center.set(0,0,0);
   sphere.set(center, 2);
   cout << "sphere = ";
   sphere.print();
   cout << endl;

   for (int px = -3; px <= 3; px++)
   {
      Point3D point;
      point.set(px,0,0);
      Vector3D dir;
      dir.set(1,0,0);
      Ray3D ray;
      ray.set(point, dir);
      cout << "ray = ";
      ray.print();
      cout << endl;

      Point3D p;
      Vector3D n;
      if (!sphere.get_intersection(ray, p, n))
         cout << "no intersection\n";
      else
      {
         cout << "point = ";
         p.print();
         cout << endl;
         cout << "normal = ";
         n.print();
         cout << endl;
      }
   }

   cout << "\nTest phong class\n";
   Phong shader;
   string name;
   while (cin >> name)
   {
      if (name == "Camera")
      {
         // Read and save camera information
         Point3D pos;
         cin >> pos.px >> pos.py >> pos.pz;
         shader.SetCamera(pos);
      }

      else if (name == "Light")
      {
         // Read and save light information
         ColorRGB color;
         Vector3D dir;
         cin >> color.R >> color.G >> color.B;
         cin >> dir.vx >> dir.vy >> dir.vz;
         shader.SetLight(color, dir);
      }

      else if (name == "Object")
      {
         // Read and save object information
         ColorRGB color;
         float ka, kd, ks, kp;
         cin >> color.R >> color.G >> color.B;
         cin >> ka >> kd >> ks >> kp;
         shader.SetObject(color, ka,kd,ks,kp);
      }

      else if (name == "Point")
      {
         // Read and use point information
         Point3D point;
         Vector3D normal;
         ColorRGB color;
         cin >> point.px >> point.py >> point.pz;
         cin >> normal.vx >> normal.vy >> normal.vz;
         shader.GetShade(point, normal, color);
         point.print();
         cout << " ";
         color.print();
         cout << endl;
      }

      else
      {
         cout << "Ignoring '" << name << "' token\n";
      }
   }

   return 0;
}