//---------------------------------------
// Program: tetris.cpp
// Purpose: Demonstrate basic OpenGL commands
//          to display the seven tetris shapes.
// Author:  Luke Miller
// Date:    Jan. 24 2021
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>

using namespace std;

//Global Variables
GLenum line = GL_LINE_LOOP;
GLenum box = GL_POLYGON;
float size = 0.08;
float halfSize = size/2;
int R = 0;
int G = 0;
int B = 0;

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   glClearColor(1.0, 1.0, 1.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}
void drawSquare(float midx, float midy){

   //Find edges relative to exact middle of square
   float radius = size/2;
   float leftx = midx - radius;
   float rightx = midx + radius;
   float topy = midy + radius;
   float bottomy = midy - radius;
   
   //Draw square
   glBegin(box);
   glColor3ub(R,G,B);
   glVertex2f(leftx, bottomy);
   glVertex2f(leftx, topy);
   glVertex2f(rightx, topy);
   glVertex2f(rightx, bottomy);
   glEnd();

   //Find edges relative to exact middle of square scaled down
   radius = size/2.7;
   leftx = midx - radius;
   rightx = midx + radius;
   topy = midy + radius;
   bottomy = midy - radius;

   //Draw inner outline
   glBegin(line);
   glColor3ub(0, 0, 0);
   glVertex2f(leftx, bottomy);
   glVertex2f(leftx, topy);
   glVertex2f(rightx, topy);
   glVertex2f(rightx, bottomy);
   glEnd();

}

//---------------------------------------
// Function to draw O piece
//---------------------------------------
void drawO(float midx, float midy)
{
   R = 240;
   G = 239;
   B = 79;
   drawSquare(midx-halfSize, midy+halfSize);
   drawSquare(midx+halfSize, midy+halfSize);
   drawSquare(midx+halfSize, midy-halfSize);
   drawSquare(midx-halfSize, midy-halfSize);
}
//---------------------------------------
// Function to draw I piece
//---------------------------------------
void drawI(float midx, float midy)
{
   R = 105;
   G = 236;
   B = 238;
   drawSquare(midx-halfSize, midy+halfSize);
   drawSquare(midx-(size + halfSize), midy+halfSize);
   drawSquare(midx+halfSize, midy+halfSize);
   drawSquare(midx+size+halfSize, midy+halfSize);
}
//---------------------------------------
// Function to draw J piece
//---------------------------------------
void drawJ(float midx, float midy)
{
   R = 0;
   G = 26;
   B = 231;
   drawSquare(midx+halfSize, midy+halfSize);
   drawSquare(midx+halfSize, midy+halfSize+size);
   drawSquare(midx+halfSize+(2*size), midy+halfSize);
   drawSquare(midx+size+halfSize, midy+halfSize);
}
//---------------------------------------
// Function to draw L piece
//---------------------------------------
void drawL(float midx, float midy)
{
   R = 229;
   G = 163;
   B = 57;
   drawSquare(midx+halfSize, midy+halfSize);
   drawSquare(midx+halfSize+(2*size), midy+halfSize+size);
   drawSquare(midx+halfSize+(2*size), midy+halfSize);
   drawSquare(midx+size+halfSize, midy+halfSize);
}
//---------------------------------------
// Function to draw T piece
//---------------------------------------
void drawT(float midx, float midy)
{
   R = 147;
   G = 41;
   B = 232;
   drawSquare(midx+halfSize, midy+halfSize);
   drawSquare(midx+halfSize+size, midy+halfSize+size);
   drawSquare(midx+halfSize+(2*size), midy+halfSize);
   drawSquare(midx+size+halfSize, midy+halfSize);
}
//---------------------------------------
// Function to draw S piece
//---------------------------------------
void drawS(float midx, float midy)
{
   R = 107;
   G = 235;
   B = 71;
   drawSquare(midx+halfSize, midy+halfSize);
   drawSquare(midx+halfSize+size, midy+halfSize+size);
   drawSquare(midx+halfSize+(2*size), midy+halfSize+size);
   drawSquare(midx+size+halfSize, midy+halfSize);
}
//---------------------------------------
// Function to draw Z piece
//---------------------------------------
void drawZ(float midx, float midy)
{
   R = 222;
   G = 47;
   B = 33;
   drawSquare(midx+halfSize, midy+halfSize);
   drawSquare(midx+halfSize, midy+halfSize+size);
   drawSquare(midx-halfSize, midy+halfSize+size);
   drawSquare(midx+size+halfSize, midy+halfSize);
}
//---------------------------------------
// Function generates random # (-1 -> 1)
//---------------------------------------
float rando(){

   //srand(1040);
   float randy = (float) (rand()-rand()) / (RAND_MAX);
   if(rand()%2==0){
      randy = -randy;
   }
   return randy;

}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   glClear(GL_COLOR_BUFFER_BIT);

   //Drawing the 7 shapes
   drawO(rando(),rando());
   drawI(rando(),rando());
   drawJ(rando(),rando());
   drawL(rando(),rando());
   drawT(rando(),rando());
   drawS(rando(),rando());
   drawZ(rando(),rando());

   glFlush();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(1000, 1000);
   glutInitWindowPosition(500, 500);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("Tetris");
   glutDisplayFunc(display);
   init();
   glutMainLoop();
   return 0;
}
