//---------------------------------------
// Program: tetris.cpp
// Purpose: Demonstrate basic OpenGL commands
//          to display, rotate, and place the seven tetris shapes.
// Author:  Luke Miller
// Date:    Feb. 9 2021
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector> 
#include <iterator> 

#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>

using namespace std;

//Const Global Variables
const float size = 1;
const float radius = size/2;

//Static Global Variables
bool LOCK = false;
static int currentX;
static int currentY;

//Vectors to track Squares on board
vector<float> lockedSquares, currentSquares;

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   glClearColor(1.0, 1.0, 1.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-10.1, 10.1, -20.1, 20.1, -10.0, 10.0);
}
//---------------------------------------
// Draws Square at (x, y) with color (R,G,B) 
// Option to append point to currentSquares Vector
//---------------------------------------
void drawSquare(float x, float y, float R, float G, float B, bool append){

   //Draw square
   glBegin(GL_POLYGON);
   glColor3f(R, G, B);
   glVertex2f(x-radius, y-radius);
   glVertex2f(x-radius, y+radius);
   glVertex2f(x+radius, y+radius);
   glVertex2f(x+radius, y-radius);
   glEnd();

   //Draw outline of square
   glLineWidth(3);
   glBegin(GL_LINE_LOOP);
   glColor3f(R/2, G/2, B/2);
   glVertex2f(x-radius, y-radius);
   glVertex2f(x-radius, y+radius);
   glVertex2f(x+radius, y+radius);
   glVertex2f(x+radius, y-radius);
   glEnd();

   //Appends the current points into the currentSquares vector
   if(append){
      currentSquares.push_back(x);
      currentSquares.push_back(y);
      currentSquares.push_back(R);
      currentSquares.push_back(G);
      currentSquares.push_back(B);
   }
}

//---------------------------------------
// Function to draw O piece
//---------------------------------------
void drawO(float x, float y)
{
   float R=1, G=1, B=0;
   drawSquare(x-radius, y+radius, R,G,B, true);
   drawSquare(x+radius, y+radius, R,G,B, true);
   drawSquare(x+radius, y-radius, R,G,B, true);
   drawSquare(x-radius, y-radius, R,G,B, true);
}
//---------------------------------------
// Function to draw I piece
//---------------------------------------
void drawI(float x, float y)
{
   float R=0.65, G=0.85, B=1;
   drawSquare(x-radius, y+radius, R,G,B, true);
   drawSquare(x-(size+radius), y+radius, R,G,B, true);
   drawSquare(x+radius, y+radius, R,G,B, true);
   drawSquare(x+size+radius, y+radius, R,G,B, true);
}
//---------------------------------------
// Function to draw J piece
//---------------------------------------
void drawJ(float x, float y)
{
   float R=0, G=0, B=1;
   drawSquare(x+radius, y-radius, R,G,B, true);
   drawSquare(x-radius, y+radius, R,G,B, true);
   drawSquare(x-radius, y-radius, R,G,B, true);
   drawSquare(x+size+radius, y-radius, R,G,B, true);
}
//---------------------------------------
// Function to draw L piece
//---------------------------------------
void drawL(float x, float y)
{
   float R=1, G=0.65, B=0;
   drawSquare(x+radius, y+radius, R,G,B, true);
   drawSquare(x+radius, y-radius, R,G,B, true);
   drawSquare(x-radius, y-radius, R,G,B, true);
   drawSquare(x-size-radius, y-radius, R,G,B, true);
}
//---------------------------------------
// Function to draw T piece
//---------------------------------------
void drawT(float x, float y)
{
   float R=1, G=0, B=1;
   drawSquare(x+radius, y+radius, R,G,B, true);
   drawSquare(x+radius, y-radius, R,G,B, true);
   drawSquare(x-radius, y-radius, R,G,B, true);
   drawSquare(x+size+radius, y-radius, R,G,B, true);
}
//---------------------------------------
// Function to draw S piece
//---------------------------------------
void drawS(float x, float y)
{
   float R=0, G=1, B=0;
   drawSquare(x+radius, y-radius, R,G,B, true);
   drawSquare(x+radius+size, y+radius, R,G,B, true);
   drawSquare(x+radius, y+radius, R,G,B, true);
   drawSquare(x-radius, y-radius, R,G,B, true);
}
//---------------------------------------
// Function to draw Z piece
//---------------------------------------
void drawZ(float x, float y)
{
   float R=1, G=0, B=0;
   drawSquare(x+radius, y-radius, R,G,B, true);
   drawSquare(x-radius-size, y+radius, R,G,B, true);
   drawSquare(x-radius, y-radius, R,G,B, true);
   drawSquare(x-radius, y+radius, R,G,B, true);
}
//---------------------------------------
// Function generates random # (-1 -> 1)
// Currently Not In use
//---------------------------------------
// float rando(){
//    float randy = (float) (rand()-rand()) / (RAND_MAX);
//    if(rand()%2==0){
//       randy = -randy;
//    }
//    return randy;
// }
//---------------------------------------
// Freeze current piece in the current location
// Pushes currentSquares into lockedSquares
// List repeats x, y, r, g, b elements (4 pieces)
//---------------------------------------
void lockSquares()
{
   for(int i = 0; i < currentSquares.size(); i++){
      lockedSquares.push_back(currentSquares[i]);
   }
   currentSquares.clear();
   LOCK = true;
   currentX = 0;
   currentY = 19;
}
//---------------------------------------
// Subtracts 1 from x in currentSquares[] list
//---------------------------------------
void moveLeft(){
   bool flag = false;
   for(int i = 0; i < currentSquares.size(); i+=5){
      if(currentSquares[i]-1 < -9.6){
         flag = true;
      }
   }
   if(!flag){
      for(int i = 0; i < currentSquares.size(); i+=5){
         currentSquares[i]-=1;
      }
      currentX-=1;
   }
   glutPostRedisplay();
}
//---------------------------------------
// Adds 1 to x in currentSquares[] list
//---------------------------------------
void moveRight(){
   bool flag = false;
   for(int i = 0; i < currentSquares.size(); i+=5){
      if(currentSquares[i]+1 > 9.6){
         flag = true;
      }
   }
   if(!flag){
      for(int i = 0; i < currentSquares.size(); i+=5){
         currentSquares[i]+=1;
      }
      currentX+=1;
   }
   glutPostRedisplay();
}
//---------------------------------------
// Subtracts 1 from y in currentSquares[] list
//---------------------------------------
void moveDown(){
   bool flag = false;
   for(int i = 1; i < currentSquares.size(); i+=5){
      if(currentSquares[i]-1 < -19.6){
         flag = true;
      }
   }
   if(!flag){
      for(int i = 1; i < currentSquares.size(); i+=5){
         currentSquares[i]-=1;
      }
      currentY-=1;

   }
   glutPostRedisplay();
}
//---------------------------------------
// Moves point back to the origin then returns rotated x position
//---------------------------------------
float rotateX(float x, float y){
   float rads = (-1.5708);
   float xPrime = (x-currentX)*cos(rads)-(y-currentY)*sin(rads);
   return xPrime;
}
//---------------------------------------
// Moves point back to the origin then returns rotated y position
//---------------------------------------
float rotateY(float x, float y){
   float rads = (-1.5708);
   float yPrime = (x-currentX)*sin(rads)+(y-currentY)*cos(rads);
   return yPrime;
}
//---------------------------------------
// Returns false if rotation will move piece out of bounds
//---------------------------------------
bool checkRotate(){
   for(int i = 0; i < currentSquares.size(); i+=5){
      float newX = (rotateX(currentSquares[i], currentSquares[i+1])) + currentX;
      float newY = (rotateY(currentSquares[i], currentSquares[i+1])) + currentY;
      if(newX>9.6 || newX<-9.6 || newY>19.6 || newY<-19.6){
         printf("\n   Error: Rotation Out of Bounds\n");
         return false;
      }
   }
   return true;
}
//---------------------------------------
// Rotates everything 90 Degress clockwise in currentSquares[] list
//---------------------------------------
void rotateClockwise(){
   if(checkRotate()){
      for(int i = 0; i < currentSquares.size(); i+=5){
         float newX = (rotateX(currentSquares[i], currentSquares[i+1]));
         float newY = (rotateY(currentSquares[i], currentSquares[i+1]));
         currentSquares[i] = newX+currentX;
         currentSquares[i+1] = newY+currentY;
      }
   }
   glutPostRedisplay();
}
//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Update angles
   if (key == ' '){
      lockSquares();
   }
   else if (key == 'i' || key == 'I'){
      lockSquares();
      drawI(0,19);
      LOCK = false;
   }
   else if (key == 'j' || key == 'J') {
      lockSquares();
      drawJ(0,19);
      LOCK = false;
   }
   else if (key == 'l' || key == 'L'){
      lockSquares();
      drawL(0,19);
      LOCK = false;
   }
   else if (key == 'o' || key == 'O') {
      lockSquares();
      drawO(0,19);
      LOCK = false;
   }
   else if (key == 's' || key == 'S') {
      lockSquares();
      drawS(0,19);
      LOCK = false;
   }
   else if (key == 't' || key == 'T'){
      lockSquares();
      drawT(0,19);
      LOCK = false;
   }
   else if (key == 'z' || key == 'Z') {
      lockSquares();
      drawZ(0,19);
      LOCK = false;
   }
   // Redraw objects
   glutPostRedisplay();
}
//---------------------------------------
// Special callback for OpenGL
//---------------------------------------
void special(int key, int x, int y)
{
   if (LOCK == false){
      switch(key)
      {
      case GLUT_KEY_UP:
         if(!LOCK){
            rotateClockwise();
         }
         break;
      case GLUT_KEY_DOWN:
         if(!LOCK){
            moveDown();
         }
         break;
      case GLUT_KEY_LEFT:
         if(!LOCK){
            moveLeft();
         }
         break;
      case GLUT_KEY_RIGHT:
         if(!LOCK){
            moveRight();
         }
         break;
      default:
         break;
      }
   }
   // Redraw objects
   glutPostRedisplay();
}
//---------------------------------------
// Function displays console instructions
//---------------------------------------
void displayConsoleInstructions(){
   printf("\n\n\n              WELCOME TO:   T E T R I S   \n");
   printf("\nI: Create New I-Tetris Piece Near the Top of the Screen \n");
   printf("J: Create New J-Tetris Piece Near the Top of the Screen \n");
   printf("L: Create New L-Tetris Piece Near the Top of the Screen \n");
   printf("O: Create New O-Tetris Piece Near the Top of the Screen \n");
   printf("S: Create New S-Tetris Piece Near the Top of the Screen \n");
   printf("T: Create New T-Tetris Piece Near the Top of the Screen \n");
   printf("Z: Create New Z-Tetris Piece Near the Top of the Screen \n \n");
   
   printf(" Left-Arrow: Move Tetris Piece One Space Left \n");
   printf("Right-Arrow: Move Tetris Piece One Space Right \n");
   printf(" Down-Arrow: Move Tetris Piece One Space Down \n");
   printf("   Up-Arrow: Rotate Tetris Piece 90 Degrees Clockwise \n");
   printf("      Space: Lock Tetris Piece in Place \n ");
}
//---------------------------------------
// Function displays list
// List repeats x, y, r, g, b elements
//---------------------------------------
void displayList(vector<float> list){
   for(int i = 0; i < list.size(); i+=5){
      drawSquare(list[i],list[i+1],list[i+2],list[i+3],list[i+4],false);
   }
}
//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   glClear(GL_COLOR_BUFFER_BIT);
   displayList(lockedSquares);
   displayList(currentSquares);
   glFlush();
}
//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(500, 1000);
   glutInitWindowPosition(250, 250);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("Tetris");
   glutDisplayFunc(display);
   displayConsoleInstructions();
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   init();
   glutMainLoop();
   return 0;
}