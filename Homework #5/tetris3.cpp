//---------------------------------------
// Program: texture3.cpp
// Purpose: Texture map brick photograph
//          onto a tetris model.
// Author:  Luke Miller
// Date:    April 2021
//---------------------------------------
#include <array>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "libim/im_color.h"

// Class to Store Coordinates
class Cord {
public:
  float x;
  float y;
  float z;
  char color;
};
// Global Variables
unsigned char *blue;
unsigned char *green;
unsigned char *light_blue;
unsigned char *orange;
unsigned char *purple;
unsigned char *red;
unsigned char *yellow;

const float size = 1;
const float radius = size / 2;

bool LOCK = false;
static int currentX;
static int currentY;
static int currentZ;

// Vectors to track cubes on board
vector<Cord> lockedSquares, currentSquares;

//---------------------------------------
//---------------------------------------
// Utility Functions
//---------------------------------------
//---------------------------------------

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init() {
  // Init view
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-11.1, 11.1, -21.1, 21.1, -11.0, 11.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
//---------------------------------------
// Initialize texture image
//---------------------------------------
void init_texture(char *name, unsigned char *&texture, int &xdim, int &ydim) {
  // Read jpg image
  im_color image;
  image.ReadJpg(name);
  // printf("Reading image %s\n", name);
  xdim = 1;
  while (xdim < image.R.Xdim)
    xdim *= 2;
  xdim /= 2;
  ydim = 1;
  while (ydim < image.R.Ydim)
    ydim *= 2;
  ydim /= 2;
  image.Interpolate(xdim, ydim);
  // printf("Interpolating to %d by %d\n", xdim, ydim);

  // Copy image into texture array
  texture = (unsigned char *)malloc((unsigned int)(xdim * ydim * 3));
  int index = 0;
  for (int y = 0; y < ydim; y++)
    for (int x = 0; x < xdim; x++) {
      texture[index++] = (unsigned char)(image.R.Data2D[y][x]);
      texture[index++] = (unsigned char)(image.G.Data2D[y][x]);
      texture[index++] = (unsigned char)(image.B.Data2D[y][x]);
    }
}
//---------------------------------------
// Function to change textures
//---------------------------------------
void changeTexture(char co) {
  // printf("changing textures %c", co);
  string color;
  unsigned char *sColor;
  if (co == 'l') {
    color = "light_blue";
    sColor = light_blue;
  } else if (co == 'y') {
    color = "yellow";
    sColor = yellow;
  } else if (co == 'b') {
    color = "blue";
    sColor = blue;
  } else if (co == 'g') {
    color = "green";
    sColor = green;
  } else if (co == 'r') {
    color = "red";
    sColor = red;
  } else if (co == 'o') {
    color = "orange";
    sColor = orange;
  } else {
    color = "purple";
    sColor = purple;
  }
  string str = "textures/" + color + ".jpg";
  char arr[str.length()];
  strcpy(arr, str.c_str());
  int xdim, ydim;
  init_texture(arr, sColor, xdim, ydim);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB,
               GL_UNSIGNED_BYTE, sColor);
}

//---------------------------------------
//---------------------------------------
// Draw Shape Functions
//---------------------------------------
//---------------------------------------

//---------------------------------------
// Function to draw 3D block with texture
//---------------------------------------
void block(float x, float y, float z) {
  float xmin = x - radius;
  float ymin = y - radius;
  float zmin = z - radius;
  float xmax = x + radius;
  float ymax = y + radius;
  float zmax = z + radius;

  // Define 8 vertices
  float ax = xmin, ay = ymin, az = zmax;
  float bx = xmax, by = ymin, bz = zmax;
  float cx = xmax, cy = ymax, cz = zmax;
  float dx = xmin, dy = ymax, dz = zmax;

  float ex = xmin, ey = ymin, ez = zmin;
  float fx = xmax, fy = ymin, fz = zmin;
  float gx = xmax, gy = ymax, gz = zmin;
  float hx = xmin, hy = ymax, hz = zmin;

  // Draw 6 faces
  glBegin(GL_POLYGON); // Max texture coord 1.0
  glTexCoord2f(0.0, 0.0);
  glVertex3f(ax, ay, az);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(bx, by, bz);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(cx, cy, cz);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(dx, dy, dz);
  glEnd();

  glBegin(GL_POLYGON); // Max texture coord 1.0
  glTexCoord2f(0.0, 0.0);
  glVertex3f(ex, ey, ez);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(ax, ay, az);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(dx, dy, dz);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(hx, hy, hz);
  glEnd();

  glBegin(GL_POLYGON); // Max texture coord 1.0
  glTexCoord2f(0.0, 0.0);
  glVertex3f(fx, fy, fz);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(ex, ey, ez);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(hx, hy, hz);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(gx, gy, gz);
  glEnd();

  glBegin(GL_POLYGON); // Max texture coord 1.0
  glTexCoord2f(0.0, 0.0);
  glVertex3f(bx, by, bz);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(fx, fy, fz);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(gx, gy, gz);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(cx, cy, cz);
  glEnd();

  glBegin(GL_POLYGON); // Max texture coord 3.0
  glTexCoord2f(0.0, 0.0);
  glVertex3f(ax, ay, az);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(ex, ey, ez);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(fx, fy, fz);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(bx, by, bz);
  glEnd();

  glBegin(GL_POLYGON); // Max texture coord 3.0
  glTexCoord2f(0.0, 0.0);
  glVertex3f(gx, gy, gz);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(cx, cy, cz);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(dx, dy, dz);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(hx, hy, hz);
  glEnd();
}
//---------------------------------------
// Current List Appender
//---------------------------------------
void cordAppender(float x, float y, float z, char c) {
  Cord cord;
  cord.x = x;
  cord.y = y;
  cord.z = z;
  cord.color = c;
  currentSquares.push_back(cord);
}
//---------------------------------------
// Function to draw O piece
//---------------------------------------
void drawO(float x, float y, float z) {
  cordAppender(x - radius, y + radius, z, 'y');
  cordAppender(x + radius, y + radius, z, 'y');
  cordAppender(x + radius, y - radius, z, 'y');
  cordAppender(x - radius, y - radius, z, 'y');
}
//---------------------------------------
// Function to draw I piece
//---------------------------------------
void drawI(float x, float y, float z) {
  cordAppender(x - radius, y + radius, z, 'l');
  cordAppender(x - (size + radius), y + radius, z, 'l');
  cordAppender(x + radius, y + radius, z, 'l');
  cordAppender(x + size + radius, y + radius, z, 'l');
}
//---------------------------------------
// Function to draw J piece
//---------------------------------------
void drawJ(float x, float y, float z) {
  cordAppender(x + radius, y - radius, z, 'p');
  cordAppender(x - radius, y + radius, z, 'p');
  cordAppender(x - radius, y - radius, z, 'p');
  cordAppender(x + size + radius, y - radius, z, 'p');
}
//---------------------------------------
// Function to draw L piece
//---------------------------------------
void drawL(float x, float y, float z) {
  cordAppender(x + radius, y + radius, z, 'o');
  cordAppender(x + radius, y - radius, z, 'o');
  cordAppender(x - radius, y - radius, z, 'o');
  cordAppender(x - size - radius, y - radius, z, 'o');
}
//---------------------------------------
// Function to draw T piece
//---------------------------------------
void drawT(float x, float y, float z) {
  cordAppender(x + radius, y + radius, z, 'b');
  cordAppender(x + radius, y - radius, z, 'b');
  cordAppender(x - radius, y - radius, z, 'b');
  cordAppender(x + size + radius, y - radius, z, 'b');
}
//---------------------------------------
// Function to draw S piece
//---------------------------------------
void drawS(float x, float y, float z) {
  cordAppender(x + radius, y - radius, z, 'g');
  cordAppender(x + radius + size, y + radius, z, 'g');
  cordAppender(x + radius, y + radius, z, 'g');
  cordAppender(x - radius, y - radius, z, 'g');
}
//---------------------------------------
// Function to draw Z piece
//---------------------------------------
void drawZ(float x, float y, float z) {
  cordAppender(x + radius, y - radius, z, 'r');
  cordAppender(x - radius - size, y + radius, z, 'r');
  cordAppender(x - radius, y - radius, z, 'r');
  cordAppender(x - radius, y + radius, z, 'r');
}

//---------------------------------------
//---------------------------------------
// Rotation/Translation Functions
//---------------------------------------
//---------------------------------------

//---------------------------------------
// Rotate point (x,y,z) around X axis by theta
//---------------------------------------
array<float, 3> rotateX(float x, float y, float z, float theta) {
  array<float, 3> ret = array<float, 3>();
  ret[0] = (x - currentX);
  ret[1] = (y - currentY) * cos(theta) - (z - currentZ) * sin(theta);
  ret[2] = (y - currentY) * sin(theta) + (z - currentZ) * cos(theta);
  return ret;
}
//---------------------------------------
// Rotate point (x,y,z) around Y axis by theta
//---------------------------------------
array<float, 3> rotateY(float x, float y, float z, float theta) {
  array<float, 3> ret = array<float, 3>();
  ret[0] = (x - currentX) * cos(theta) + (z - currentZ) * sin(theta);
  ret[1] = (y - currentY);
  ret[2] = (z - currentZ) * cos(theta) - (x - currentX) * sin(theta);
  return ret;
}
//---------------------------------------
// Rotate point (x,y,z) around Z axis by theta
//---------------------------------------
array<float, 3> rotateZ(float x, float y, float z, float theta) {
  array<float, 3> ret = array<float, 3>();
  ret[0] = (x - currentX) * cos(theta) - (y - currentY) * sin(theta);
  ret[1] = (x - currentX) * sin(theta) + (y - currentY) * cos(theta);
  ret[2] = (z - currentZ);
  return ret;
}

//---------------------------------------
// Rotates everything 90 Degress clockwise in currentSquares[] list around
// X-axis
//---------------------------------------
void rotateXax() {
  for (int i = 0; i < currentSquares.size(); i++) {
    array<float, 3> newCord = rotateX(currentSquares[i].x, currentSquares[i].y,
                                      currentSquares[i].z, (-1.5708));
    currentSquares[i].x = newCord[0] + currentX;
    currentSquares[i].y = newCord[1] + currentY;
    currentSquares[i].z = newCord[2] + currentZ;
  }
  glutPostRedisplay();
}
//---------------------------------------
// Rotates everything 90 Degress clockwise in currentSquares[] list around
// Y-axis
//---------------------------------------
void rotateYax() {
  for (int i = 0; i < currentSquares.size(); i++) {
    array<float, 3> newCord = rotateY(currentSquares[i].x, currentSquares[i].y,
                                      currentSquares[i].z, (-1.5708));
    currentSquares[i].x = newCord[0] + currentX;
    currentSquares[i].y = newCord[1] + currentY;
    currentSquares[i].z = newCord[2] + currentZ;
  }
  glutPostRedisplay();
}
//---------------------------------------
// Rotates everything 90 Degress clockwise in currentSquares[] list around
// Z-axis
//---------------------------------------
void rotateZax() {
  for (int i = 0; i < currentSquares.size(); i++) {
    array<float, 3> newCord = rotateZ(currentSquares[i].x, currentSquares[i].y,
                                      currentSquares[i].z, (-1.5708));
    currentSquares[i].x = newCord[0] + currentX;
    currentSquares[i].y = newCord[1] + currentY;
    currentSquares[i].z = newCord[2] + currentZ;
  }
  glutPostRedisplay();
}

//---------------------------------------
// Subtracts 1 from x in currentSquares[] list
//---------------------------------------
void moveLeft() {
  bool flag = false;
  for (int i = 0; i < currentSquares.size(); i++) {
    if (currentSquares[i].x - 1 < -9.6) {
      flag = true;
    }
  }
  if (!flag) {
    for (int i = 0; i < currentSquares.size(); i++) {
      currentSquares[i].x -= 1;
    }
    currentX -= 1;
  }
  glutPostRedisplay();
}
//---------------------------------------
// Adds 1 to x in currentSquares[] list
//---------------------------------------
void moveRight() {
  bool flag = false;
  for (int i = 0; i < currentSquares.size(); i++) {
    if (currentSquares[i].x + 1 > 9.6) {
      flag = true;
    }
  }
  if (!flag) {
    for (int i = 0; i < currentSquares.size(); i++) {
      currentSquares[i].x += 1;
    }
    currentX += 1;
  }
  glutPostRedisplay();
}
//---------------------------------------
// Subtracts 1 from y in currentSquares[] list
//---------------------------------------
void moveDown() {
  bool flag = false;
  for (int i = 0; i < currentSquares.size(); i++) {
    if (currentSquares[i].y - 1 < -19.6) {
      flag = true;
    }
  }
  if (!flag) {
    for (int i = 0; i < currentSquares.size(); i++) {
      currentSquares[i].y -= 1;
    }
    currentY -= 1;
  }
  glutPostRedisplay();
}
//---------------------------------------
// Freeze current piece in the current location
// Pushes currentSquares into lockedSquares
//---------------------------------------
void lockSquares() {
  for (int i = 0; i < currentSquares.size(); i++) {
    lockedSquares.push_back(currentSquares[i]);
  }
  currentSquares.clear();
  LOCK = true;
  currentX = 0;
  currentY = 19;
  currentZ = 0;
}

//---------------------------------------
//---------------------------------------
// Display Functions
//---------------------------------------
//---------------------------------------

//---------------------------------------
// Function displays console instructions
//---------------------------------------
void prompt() {
  printf("\n\n\n              WELCOME TO:   T E T R I S  3 D \n");
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
  printf("   Up-Arrow: Rotate Tetris Piece 90 Degrees (Z-Axis) \n");
  printf("          Y: Rotate Tetris Piece 90 Degrees (Y-Axis) \n");
  printf("          X: Rotate Tetris Piece 90 Degrees (X-Axis) \n");
  printf("      Space: Lock Tetris Piece in Place \n ");
}
//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y) {
  if (key == ' ') {
    lockSquares();
  } else if (key == 'i' || key == 'I') {
    lockSquares();
    drawI(0, 19, 0);
    LOCK = false;
  } else if (key == 'j' || key == 'J') {
    lockSquares();
    drawJ(0, 19, 0);
    LOCK = false;
  } else if (key == 'l' || key == 'L') {
    lockSquares();
    drawL(0, 19, 0);
    LOCK = false;
  } else if (key == 'o' || key == 'O') {
    lockSquares();
    drawO(0, 19, 0);
    LOCK = false;
  } else if (key == 's' || key == 'S') {
    lockSquares();
    drawS(0, 19, 0);
    LOCK = false;
  } else if (key == 't' || key == 'T') {
    lockSquares();
    drawT(0, 19, 0);
    LOCK = false;
  } else if (key == 'z' || key == 'Z') {
    lockSquares();
    drawZ(0, 19, 0);
    LOCK = false;
  } else if (key == 'x' || key == 'X')
    rotateXax();
  else if (key == 'y' || key == 'Y')
    rotateYax();
  glutPostRedisplay();
}
//---------------------------------------
// Special callback for OpenGL
//---------------------------------------
void special(int key, int x, int y) {
  if (LOCK == false) {
    switch (key) {
    case GLUT_KEY_UP:
      if (!LOCK) {
        rotateZax();
      }
      break;
    case GLUT_KEY_DOWN:
      if (!LOCK) {
        moveDown();
      }
      break;
    case GLUT_KEY_LEFT:
      if (!LOCK) {
        moveLeft();
      }
      break;
    case GLUT_KEY_RIGHT:
      if (!LOCK) {
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
// Function displays list
//---------------------------------------
void displayList(vector<Cord> list) {
  for (int i = 0; i < list.size(); i++) {
    changeTexture(list[i].color);
    block(list[i].x, list[i].y, list[i].z);
  }
}
//---------------------------------------
// Function displays border for game but
// breaks textures :(
//---------------------------------------
void displayBorder() {
  glColor3f(0.0f, 0.0f, 0.0f);
  glBegin(GL_LINE_LOOP);
  glVertex3f(-10, -20, -1);
  glVertex3f(10, -20, -1);
  glVertex3f(10, 20, -1);
  glVertex3f(-10, 20, -1);
  glEnd();
  glBegin(GL_LINE_LOOP);
  glVertex3f(-10, -20, 1);
  glVertex3f(10, -20, 1);
  glVertex3f(10, 20, 1);
  glVertex3f(-10, 20, 1);
  glEnd();
}
//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display() {
  // Incrementally rotate objects
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotatef(10, 1.0, 0.0, 0.0);
  glRotatef(-30, 0.0, 1.0, 0.0);

  // displayBorder();
  displayList(lockedSquares);
  displayList(currentSquares);

  glFlush();
}
//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[]) {
  // Create OpenGL window
  glutInit(&argc, argv);
  glutInitWindowSize(500, 1000);
  glutInitWindowPosition(250, 250);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
  glutCreateWindow("3D-Tetris");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  init();
  prompt();
  glutMainLoop();
  return 0;
}
