//---------------------------------------
// Program: hw3.cpp
// Purpose: Generate and display 3D models
//          of drawn objects of rotation.
// Author:  Luke Miller
// Date:    Feburary 2021
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using std::vector;

// Global constants
#define MIN_X_VIEW -50
#define MAX_X_VIEW 50
#define MIN_Y_VIEW -50
#define MAX_Y_VIEW 50
#define MIN_Z_VIEW -50
#define MAX_Z_VIEW 50
#define MIN_X_SCREEN 0
#define MAX_X_SCREEN 500
#define MIN_Y_SCREEN 0
#define MAX_Y_SCREEN 500
#define RECT_COUNT 1000

// Surface class
const int SIZE = 50;
class Surface 
{
   public:
      float Px[SIZE][SIZE];
      float Py[SIZE][SIZE];
      float Pz[SIZE][SIZE];
      float Nx[SIZE][SIZE];
      float Ny[SIZE][SIZE];
      float Nz[SIZE][SIZE];
};

// Global variables 
float xangle = 0;
float yangle = 0;
float zangle = 0;
int count = 50;
bool drawing = true;
bool initDraw = false;
//float point[RECT_COUNT][2];
vector<vector<float>> point(RECT_COUNT, vector<float>(2,0));

Surface shape;
Surface surface;


//---------------------------------------
// Initialize shape surface and normals
//---------------------------------------
void init_shape(Surface &s)
{
   // Create X-Y profile
   for (int i = 0; i <= SIZE; i++)
   {
      //float theta = i * 2 * M_PI / SIZE;
      s.Px[i][0] = 20 * sin(i/5) + 10;
      s.Py[i][0] = 30 * sin(i/5);
      s.Pz[i][0] = 0.0;
   }

   //Rotate around the x-axis 
   for (int j = 0; j < SIZE; j++)
   {
      float theta = j * 2 * M_PI / SIZE;
      float cos_theta = cos(theta);
      float sin_theta = sin(theta);
      for (int i = 0; i < SIZE; i++)
      {
         s.Px[i][j] = s.Px[i][0];
         s.Py[i][j] = s.Py[i][0] * cos_theta - s.Pz[i][0] * sin_theta;;
         s.Pz[i][j] = s.Py[i][0] * sin_theta + s.Pz[i][0] * cos_theta;
      }
   }
}

//---------------------------------------
// Initialize surface and normals
//---------------------------------------
void init_surface(Surface &s)
{
   //Reset surface
   for(int i = 0; i< SIZE; i++){
      for(int j = 0; j < SIZE; j++){
         s.Px[i][j] = 0;
         s.Py[i][j] = 0;
         s.Pz[i][j] = 0.0;
      }
   }
   int div = 1;
   if(count>SIZE){
      float ceiler = (float)count/(float)SIZE;
      div = floor(ceiler);
   }
   //printf("count: %d\n div: %d\n",count,div);

   // Initialize surface
   for (int i = 0; i<=SIZE && i<=count; i++)
   {
      if(i==0){
         s.Px[i][0] = point[0][0];
         s.Py[i][0] = point[0][1];
         s.Pz[i][0] = 0.0;
      }
      else if((i*div) < count){
         s.Px[i][0] = point[i*div][0];
         s.Py[i][0] = point[i*div][1];
         s.Pz[i][0] = 0.0;
      }
      else{
         s.Px[i][0] = point[count][0];
         s.Py[i][0] = point[count][1];
         s.Pz[i][0] = 0.0;
      }
   }

   //Rotate around the x-axis
   for (int j = 0; j < SIZE; j++)
   {
      float theta = j * 2 * M_PI / SIZE;
      float cos_theta = cos(theta);
      float sin_theta = sin(theta);
      for (int i = 0; i < SIZE; i++)
      {
         s.Px[i][j] = s.Px[i][0];
         s.Py[i][j] = s.Py[i][0] * cos_theta - s.Pz[i][0] * sin_theta;;
         s.Pz[i][j] = s.Py[i][0] * sin_theta + s.Pz[i][0] * cos_theta;
      }
   }
}

//---------------------------------------
// Displays the line the user is drawing
//---------------------------------------
void display_draw(){
   glColor3f(0, 100, 200);
   if(count>0){
      glBegin(GL_LINE_STRIP);
      for (int i=0; i<count; i++){
         glVertex2f(point[i][0], point[i][1]);
      }  
      glEnd();
   }
   glutPostRedisplay();
}

//---------------------------------------
// Print surface coordinates and normals.
//---------------------------------------
void print_surface(Surface &s)
{
   for (int u = 0; u < SIZE; u++)
      for (int v = 0; v < SIZE; v++)
         printf("%d %d %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f\n", u, v, 
            s.Px[u][v], s.Py[u][v], s.Pz[u][v],
            s.Nx[u][v], s.Ny[u][v], s.Nz[u][v]);
}

//---------------------------------------
// Display surface coordinates and normals.
//---------------------------------------
void display_surface(Surface &s)
{
   for (int u = 0; u<SIZE-1 && u<count-1; u++)
      for (int v = 0; v < SIZE-1; v++)
      {
         glBegin(GL_LINE_LOOP);
         glNormal3f(s.Nx[u][v], s.Ny[u][v], s.Nz[u][v]);
         glVertex3f(s.Px[u][v], s.Py[u][v], s.Pz[u][v]);
         glNormal3f(s.Nx[u + 1][v], s.Ny[u + 1][v], s.Nz[u + 1][v]);
         glVertex3f(s.Px[u + 1][v], s.Py[u + 1][v], s.Pz[u + 1][v]);
         glNormal3f(s.Nx[u + 1][v + 1], s.Ny[u + 1][v + 1], s.Nz[u + 1][v + 1]);
         glVertex3f(s.Px[u + 1][v + 1], s.Py[u + 1][v + 1], s.Pz[u + 1][v + 1]);
         glNormal3f(s.Nx[u][v + 1], s.Ny[u][v + 1], s.Nz[u][v + 1]);
         glVertex3f(s.Px[u][v + 1], s.Py[u][v + 1], s.Pz[u][v + 1]);
         glEnd();
      }
}

//---------------------------------------
// Display surface coordinates and normals.
//---------------------------------------
void display_surface(Surface &s, float scale, float dx, float dy, float dz)
{
   glPushMatrix();
   glTranslatef(dx, dy, dz);
   glScalef(scale, scale, scale);
   display_surface(s);
   glPopMatrix();
}

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   // Initialize OpenGL
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(MIN_X_VIEW, MAX_X_VIEW, 
      MIN_Y_VIEW, MAX_Y_VIEW, 
      MIN_Z_VIEW, MAX_Z_VIEW);
   glEnable(GL_DEPTH_TEST);

   // Initialize surfaces
   //init_donut(donut);
   //init_vase(vase);
   init_shape(shape);
   

}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Update angles
   if (key == 'x')
      xangle -= 5;
   else if (key == 'X')
      xangle += 5;
   else if (key == 'y')
      yangle -= 5;
   else if (key == 'Y')
      yangle += 5;
   else if (key == 'z')
      zangle -= 5;
   else if (key == 'Z')
      zangle += 5;

   // Redraw objects
   glutPostRedisplay();
}

//---------------------------------------
// Mouse callback for OpenGL
//---------------------------------------
void mouse(int button, int state, int x, int y)
{
   // Calculate scale factors
   float x_scale = (MAX_X_VIEW - MIN_X_VIEW) / 
      (float)(MAX_X_SCREEN - MIN_X_SCREEN);
   float y_scale = (MIN_Y_VIEW - MAX_Y_VIEW) / 
      (float)(MAX_Y_SCREEN - MIN_Y_SCREEN);

   if (state == GLUT_DOWN)
   {
      //Set Global Vars
      for(int i = 0; i<= count; i++){
         point[i][0] = 0;
         point[i][1] = 0;
      }
      count = 0;
      xangle = 0;
      yangle = 0;
      zangle = 0;
      drawing = true;
      initDraw = true;

      point[count][0] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
      point[count][1] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
      count++;
   }
   else if(state == GLUT_UP){
      drawing = false;
      init_surface(surface);
   }
   glutPostRedisplay();
}

//---------------------------------------
// Motion callback for OpenGL
//---------------------------------------
void motion(int x, int y)
{
   // Calculate scale factors
   float x_scale = (MAX_X_VIEW - MIN_X_VIEW) / 
      (float)(MAX_X_SCREEN - MIN_X_SCREEN);
   float y_scale = (MIN_Y_VIEW - MAX_Y_VIEW) / 
      (float)(MAX_Y_SCREEN - MIN_Y_SCREEN);

   // Handle mouse motion
   if(((point[count-1][0] == MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale)
      && point[count-1][1] == MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale) != true){
         point[count][0] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
         point[count][1] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
         count++;
   }
   glutPostRedisplay();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   // Incrementally rotate objects
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glRotatef(xangle, 1.0, 0.0, 0.0);
   glRotatef(yangle, 0.0, 1.0, 0.0);
   glRotatef(zangle, 0.0, 0.0, 1.0);

   if(!initDraw)
      display_surface(shape);
   if(drawing)
      display_draw();
   else
      display_surface(surface);
   glFlush();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(MAX_Y_SCREEN, MAX_X_SCREEN);
   glutInitWindowPosition(MAX_Y_SCREEN/2, MAX_X_SCREEN/2);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("Homework 3");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   init();
   printf("Keyboard commands:\n");
   printf("   'x' - rotate x-axis -5 degrees\n");
   printf("   'X' - rotate x-axis +5 degrees\n");
   printf("   'y' - rotate y-axis -5 degrees\n");
   printf("   'Y' - rotate y-axis +5 degrees\n");
   printf("Mouse operations:\n");
   printf("   'mouse down' - start drawing line\n");
   printf("   'mouse motion' - draw continuous line\n");
   printf("   'mouse up' - finish drawing line\n");
   glutMainLoop();
   return 0;
}