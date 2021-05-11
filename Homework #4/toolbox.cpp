#include <iterator>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#ifdef MAC
#include <GLUT/glut.h>
#else
// #include <GL/glut.h>
#endif
using namespace std;

//---------------------------------------
//---------------------------------------
// Global Class/Variables
//---------------------------------------
//---------------------------------------

//---------------------------------------
// Surface class for color, point, and normal values
//---------------------------------------
class Surface 
{
   public:
        float R[4];
        float G[4];
        float B[4];
        float Px[4];
        float Py[4];
        float Pz[4];
        float Nx[4];
        float Ny[4];
        float Nz[4];

        void printSurface(){
            for(int i = 0; i < 4; i++){
                printf("R: %f G: %f B: %f\n", R[i], G[i], B[i]);
                printf("Px: %f Py: %f Pz: %f\n", Px[i], Py[i], Pz[i]);
                printf("Nx: %f Ny: %f Nz: %f\n\n", Nx[i], Ny[i], Nz[i]);
            }
        }
};
int objectSize = 9;
Surface object[9];

//---------------------------------------
//---------------------------------------
// Geometric Operations
//---------------------------------------
//---------------------------------------

//---------------------------------------
// Translate point (x,y,z) by (Tx,Ty,Tz)
//---------------------------------------
array<float, 3> translate(float x, float y, float z, float tx, float ty, float tz){
    array<float, 3> ret = array<float, 3>();
    ret[0] = x + tx;
    ret[1] = y + ty;
    ret[2] = z + tz;
    return ret;
}
//---------------------------------------
// Scale point (x,y,z) by (Sx,Sy,Sx)
//---------------------------------------
array<float, 3> scale(float x, float y, float z, float sx, float sy, float sz){
    array<float, 3> ret = array<float, 3>();
    ret[0] = x * sx;
    ret[1] = y * sy;
    ret[2] = z * sz;
    return ret;
}
//---------------------------------------
// Rotate point (x,y,z) around X axis by theta
//---------------------------------------
array<float, 3> rotateX(float x, float y, float z, float theta){
    array<float, 3> ret = array<float, 3>();
    ret[0] = x;
    ret[1] = y*cos(theta) - z*sin(theta);
    ret[2] = y*sin(theta) + z*cos(theta);
    return ret;
}
//---------------------------------------
// Rotate point (x,y,z) around Y axis by theta
//---------------------------------------
array<float, 3> rotateY(int x, int y, int z, int theta){
    array<float, 3> ret = array<float, 3>();
    ret[0] = x*cos(theta) + z*sin(theta);
    ret[1] = y;
    ret[2] = z*cos(theta) - x*sin(theta);
    return ret;
}
//---------------------------------------
// Rotate point (x,y,z) around Z axis by theta
//---------------------------------------
array<float, 3> rotateZ(int x, int y, int z, int theta){
    array<float, 3> ret = array<float, 3>();
    ret[0] = x*cos(theta) - y*sin(theta);
    ret[1] = x*sin(theta) + y*cos(theta);
    ret[2] = z;
    return ret;
}
//---------------------------------------
// Project point (x,y,z) onto Z=d plane and return (Xp,Yp,Zp)
//---------------------------------------
array<float, 3> project(float x, float y, float z, float d){
    array<float, 3> ret = array<float, 3>();
    ret[0] = (x*d)/z;
    ret[1] = (y*d)/z;
    ret[2] = d;
    return ret;
}

//---------------------------------------
//---------------------------------------
// Interpolation Operations
//---------------------------------------
//---------------------------------------

//---------------------------------------
// Linearly interpolate from a float value a 
// from range [aMin..aMax] to a float value 
// b in the range [bMin..bMax].
//---------------------------------------
float linInterp(float a, float aMin, float aMax,float bMin, float bMax){
    return ((bMax - bMin)*(a-aMin)/(aMax-aMin) + bMin);
}
//---------------------------------------
// Bilinearly interpolate between four 
// float values A,B,C,D to find the value 
// V that is alpha percent of the way between 
// [A..B] and [C..D] and beta percent of the 
// way between [A..C] and [B..D], where alpha = [0..1] 
// and beta = [0..1].
//---------------------------------------
float bilinInterp(float a, float b, float c, float d, float alpha, float beta){
    float V = (d*alpha*beta + c*(1-alpha)*beta - b*alpha*(1-beta) + a*(1-alpha)*(1-beta));
    return V;
}

//---------------------------------------
// Linearly interpolate coordinates to the
// find point (x,y,z) that is alpha percent between the
// two points (x1,y1,z1) and (x2,y2,z2), where alpha = [0..1].
//---------------------------------------
array<float, 3> linInterpCoor(float x1, float y1, float z1, float x2, float y2, float z2, float alpha){
    array<float, 3> ret = array<float, 3>();
    ret[0] = (1-alpha)*x1 + alpha*x2;
    ret[1] = (1-alpha)*y1 + alpha*y2;
    ret[2] = (1-alpha)*z1 + alpha*z2;
    return ret;
}

array<float, 6> bilinearInterp(float x1, float y1, float z1, float x2, float y2, float z2, float alpha, float beta){
    array<float, 6> ret = array<float, 6>();

    array<float, 3> split = linInterpCoor(x1,y1,z1,x2,y2,z2,alpha);
    ret[0] = split[0];
    ret[1] = split[1];
    ret[2] = split[2];
    split = linInterpCoor(x1,y1,z1,x2,y2,z2,beta);
    ret[3] = split[0];
    ret[4] = split[1];
    ret[5] = split[2];

    return ret;
}
//---------------------------------------
// Linearly interpolate unit length surface
// normals to find (Nx,Ny,Nz) that is alpha
// percent between (Nx1,Ny1,Nz1) and (Nx2,Ny2,Nz2), alpha = [0..1]
//---------------------------------------
array<float, 3> linInterpUnit(float nx1, float ny1, float nz1, float nx2, float ny2, float nz2, float alpha){
    return linInterpCoor(nx1, ny1, nz1, nx2, ny2, nz2, alpha);
}

//---------------------------------------
//---------------------------------------
// Shading Operations
//---------------------------------------
//---------------------------------------

//---------------------------------------
// Calculate CrossProduct of vectors Ax,Ay,Az
// and Bx,By,Bz and return Cx,Cy,Cz
//---------------------------------------
array<float, 3> crossProduct(float ax, float ay, float az, float bx, float by, float bz){
    array<float, 3> ret = array<float, 3>();
    ret[0] = ay*bz - az*by;
    ret[1] = ax*bz - az*bx;
    ret[2] = ax*by - ay*bx;
    return ret;
}
//---------------------------------------
// Normalize length of vector Ax,Ay,Az
//---------------------------------------
array<float, 3> normalize(float ax, float ay, float az){
    array<float, 3> ret = array<float, 3>();
    float length = abs(sqrt((ax * ax) + (ay * ay) + (az * az)));
    
    ret[0] = ax/length;
    ret[1] = ay/length;
    ret[2] = az/length;

    return ret;
} 
//---------------------------------------
// Calculate DotProduct of vectors Ax,Ay,Az 
// and Bx,By,Bz and return scalar
//---------------------------------------
float dotProduct(float ax, float ay, float az, float bx, float by, float bz){
    return (ax*bx + ay*by + az*bz);
}
//---------------------------------------
// Calculate DiffuseTerm using Lx,Ly,Lz 
// and Nx,Ny,Nz and return scalar
//---------------------------------------
float diffuseTerm(float lx, float ly, float lz, float nx, float ny, float nz){
    return (dotProduct(lx, ly, lz, nx, ny, nz));
}
//---------------------------------------
// Calculate IdealReflection using Lx,Ly,Lz
// and Nx,Ny,Nz and return Rx,Ry,Rz
//---------------------------------------
array<float, 3> idealReflection(float lx, float ly, float lz, float nx, float ny, float nz){
    array<float, 3> ret = array<float, 3>();
    float dp = dotProduct(lx, ly,  lz, nx, ny, nz);
    ret[0] = 2*dp*nx - 1;
    ret[1] = 2*dp*ny - 1;
    ret[2] = 2*dp*nz - 1;
    return ret;
}
//---------------------------------------
// Calculate SpecularTerm using Vx,Vy,Vz
// and Rx,Ry,Rz and P and return scalar
//---------------------------------------
float specularTerm(float vx, float vy, float vz, float rx, float ry, float rz, float p){
    return pow(dotProduct(vx, vy, vz, rx, ry, rz), p);
}
//---------------------------------------
// Calculate Phong shading color Cr,Cb,Cb 
// at point Px,Py,Pz given the light
// direction Lx,Ly,Lz, normal vector Nx,Ny,Nz,
// material color Mr,Mg,Mb and Phong coefficients
// Ka,Kd,Ks,Kp. Assume the light color is white
// (1,1,1) and the camera is located at (0,0,0)
//---------------------------------------
array<float, 3> phongShading(float px, float py, float pz,
                             float lx, float ly, float lz,
                             float nx, float ny, float nz,
                             float mr, float mg, float mb,
                             float ka, float kd, float ks, float kp){
    
    float vx, vy, vz = 0;
    array<float, 3> phongColor = array<float, 3>();
    array<float, 3> r = idealReflection(lx,ly,lz,nx,ny,nz);
    float specTerm = specularTerm(vx,vy,vz,r[0],r[1],r[2],kp);
    float ldn = dotProduct(lx,ly,lz,nx,ny,nz);

    phongColor[0] = (kd*mr*ldn) + (ks*mr*specTerm) + ka*mr;
    phongColor[1] = (kd*mg*ldn) + (ks*mg*specTerm) + ka*mg;
    phongColor[2] = (kd*mb*ldn) + (ks*mb*specTerm) + ka*mb;

    return phongColor;
}

//---------------------------------------
//---------------------------------------
// Utility Operations
//---------------------------------------
//---------------------------------------

//---------------------------------------
// Split Up init surface into 9 surfaces
// This is bad programming, for that I'm sorry
// I just couldn't think through a good loop for it
// I am very ashamed of this function
// :(
//---------------------------------------
void splitObject(){
    float oneThird = (1/3);
    float twoThird = (2/3);
    Surface init = object[0];
    array<float, 6> splitter;
    //corners
    object[2].Px[1] = init.Px[1];
    object[2].Py[1] = init.Px[1];
    object[2].Pz[1] = init.Px[1];
    object[6].Px[2] = init.Px[2];
    object[6].Py[2] = init.Px[2];
    object[6].Pz[2] = init.Px[2];
    object[8].Px[3] = init.Px[3];
    object[8].Py[3] = init.Px[3];
    object[8].Pz[3] = init.Px[3];
    //outer edges
    splitter = bilinearInterp(init.Px[0], init.Py[0], init.Pz[0], init.Px[1], init.Py[1], init.Pz[1], oneThird, twoThird);
    object[0].Px[1] = splitter[0];
    object[0].Py[1] = splitter[1];
    object[0].Pz[1] = splitter[2];
    object[1].Px[0] = splitter[0];
    object[1].Py[0] = splitter[1];
    object[1].Pz[0] = splitter[2];
    object[1].Px[1] = splitter[3];
    object[1].Py[1] = splitter[4];
    object[1].Pz[1] = splitter[5];
    object[2].Px[0] = splitter[3];
    object[2].Py[0] = splitter[4];
    object[2].Pz[0] = splitter[5];
    splitter = bilinearInterp(init.Px[2], init.Py[2], init.Pz[2], init.Px[3], init.Py[3], init.Pz[3], oneThird, twoThird);
    object[6].Px[3] = splitter[0];
    object[6].Py[3] = splitter[1];
    object[6].Pz[3] = splitter[2];
    object[7].Px[2] = splitter[0];
    object[7].Py[2] = splitter[1];
    object[7].Pz[2] = splitter[2];
    object[7].Px[3] = splitter[3];
    object[7].Py[3] = splitter[4];
    object[7].Pz[3] = splitter[5];
    object[8].Px[2] = splitter[3];
    object[8].Py[2] = splitter[4];
    object[8].Pz[2] = splitter[5];
    splitter = bilinearInterp(init.Px[0], init.Py[0], init.Pz[0], init.Px[2], init.Py[2], init.Pz[2], oneThird, twoThird);
    object[0].Px[2] = splitter[0];
    object[0].Py[2] = splitter[1];
    object[0].Pz[2] = splitter[2];
    object[3].Px[0] = splitter[0];
    object[3].Py[0] = splitter[1];
    object[3].Pz[0] = splitter[2];
    object[3].Px[2] = splitter[3];
    object[3].Py[2] = splitter[4];
    object[3].Pz[2] = splitter[5];
    object[6].Px[0] = splitter[3];
    object[6].Py[0] = splitter[4];
    object[6].Pz[0] = splitter[5];
    splitter = bilinearInterp(init.Px[1], init.Py[1], init.Pz[1], init.Px[3], init.Py[3], init.Pz[3], oneThird, twoThird);
    object[2].Px[3] = splitter[0];
    object[2].Py[3] = splitter[1];
    object[2].Pz[3] = splitter[2];
    object[5].Px[1] = splitter[0];
    object[5].Py[1] = splitter[1];
    object[5].Pz[1] = splitter[2];
    object[5].Px[3] = splitter[3];
    object[5].Py[3] = splitter[4];
    object[5].Pz[3] = splitter[5];
    object[8].Px[1] = splitter[3];
    object[8].Py[1] = splitter[4];
    object[8].Pz[1] = splitter[5];
    // Inner edges
    splitter = bilinearInterp(object[0].Px[1], object[0].Py[1], object[0].Pz[1], object[6].Px[3], object[6].Py[3], object[6].Pz[3], oneThird, twoThird);
    object[0].Px[3] = splitter[0];
    object[0].Py[3] = splitter[1];
    object[0].Pz[3] = splitter[2];
    object[3].Px[1] = splitter[0];
    object[3].Py[1] = splitter[1];
    object[3].Pz[1] = splitter[2];
    object[3].Px[3] = splitter[3];
    object[3].Py[3] = splitter[4];
    object[3].Pz[3] = splitter[5];
    object[6].Px[1] = splitter[3];
    object[6].Py[1] = splitter[4];
    object[6].Pz[1] = splitter[5];
    object[1].Px[2] = splitter[0];
    object[1].Py[2] = splitter[1];
    object[1].Pz[2] = splitter[2];
    object[4].Px[0] = splitter[0];
    object[4].Py[0] = splitter[1];
    object[4].Pz[0] = splitter[2];
    object[4].Px[2] = splitter[3];
    object[4].Py[2] = splitter[4];
    object[4].Pz[2] = splitter[5];
    object[7].Px[0] = splitter[3];
    object[7].Py[0] = splitter[4];
    object[7].Pz[0] = splitter[5];

    splitter = bilinearInterp(object[1].Px[1], object[1].Py[1], object[1].Pz[1], object[8].Px[2], object[8].Py[2], object[8].Pz[2], oneThird, twoThird);
    object[1].Px[3] = splitter[0];
    object[1].Py[3] = splitter[1];
    object[1].Pz[3] = splitter[2];
    object[4].Px[1] = splitter[0];
    object[4].Py[1] = splitter[1];
    object[4].Pz[1] = splitter[2];
    object[4].Px[3] = splitter[3];
    object[4].Py[3] = splitter[4];
    object[4].Pz[3] = splitter[5];
    object[7].Px[1] = splitter[3];
    object[7].Py[1] = splitter[4];
    object[7].Pz[1] = splitter[5];
    object[2].Px[2] = splitter[0];
    object[2].Py[2] = splitter[1];
    object[2].Pz[2] = splitter[2];
    object[5].Px[0] = splitter[0];
    object[5].Py[0] = splitter[1];
    object[5].Pz[0] = splitter[2];
    object[5].Px[2] = splitter[3];
    object[5].Py[2] = splitter[4];
    object[5].Pz[2] = splitter[5];
    object[8].Px[0] = splitter[3];
    object[8].Py[0] = splitter[4];
    object[8].Pz[0] = splitter[5];


    // array<float, 3> splitter;
    // for(int i = 0; i < 9; i++){
    //     if(i%2==0 && i!=4){
    //         for(int j = 0; j<3; j++){
    //             splitter = linInterpCoor(object[0].Px[j], object[0].Py[j], object[0].Pz[j], object[0].Px[j+1], object[0].Py[j+1], object[0].Pz[j+1], oneThird);

    //         }   
    //    }
    // }

        

    
}
//---------------------------------------
// User Prompt
//---------------------------------------
float userPrompt(string prompt){
    float ret = 0;
    cout << prompt<< endl;
    cin >> ret;
    return ret;
}
//---------------------------------------
// Calculate New Normals of 9 surfaces
//---------------------------------------
void calcNormals(){
    for(int i=0; i<objectSize; i++){
        for(int j=0; j<4; j++){
            array<float, 3> norms = normalize(object[i].Px[j],object[i].Py[j],object[i].Pz[j]);
            object[i].Nx[j] = norms[0];
            object[i].Ny[j] = norms[1];
            object[i].Nz[j] = norms[2];
        }
    }
}
//---------------------------------------
// Calculate Shading of 9 surfaces
//---------------------------------------
void calcShading(float lx, float ly, float lz, float ka, float kd, float ks,float kp){
    
    float px, py, pz,nx, ny, nz,mr, mg, mb;
    
    mr = object[0].R[0];
    mg = object[0].G[0];
    mb = object[0].B[0];

    for(int i=0; i<objectSize; i++){
        for(int j=0; j<4; j++){
            px = object[i].Px[j];
            py = object[i].Py[j];
            pz = object[i].Pz[j];
            nx = object[i].Nx[j];
            ny = object[i].Ny[j];
            nz = object[i].Nz[j];
            array<float, 3> newColors = phongShading(px, py, pz,lx, ly, lz,nx, ny, nz,
                                                    mr, mg, mb,ka, kd, ks, kp);
            object[i].R[j] = newColors[0];
            object[i].G[j] = newColors[1];
            object[i].B[j] = newColors[2];
        }
    }
}
//---------------------------------------
// Open Input file and read values into Surface object
//---------------------------------------
void inFile(string fileName){

    ifstream din;
    din.open(fileName);

    while (!din.eof())
    {
        string word;
        // Read four points of rectangle
        for (int i=0; i<4; i++)
        {
            if ((i==0) && (din >> word) && (word == "color"))
                din >> object[0].R[i] >> object[0].G[i] >> object[0].B[i];
            if ((din >> word) && (word == "normal"))
                din >> object[0].Nx[i] >> object[0].Ny[i] >> object[0].Nz[i];
            if ((din >> word) && (word == "point"))
                din >> object[0].Px[i] >> object[0].Py[i] >> object[0].Pz[i];
        }
        // Check for end of file
        if (din.eof())
            continue;
    }
}
//---------------------------------------
// Create output file and write values from Surface object
//---------------------------------------
void outFile(){

    ofstream dout("output.txt");

    if (dout.is_open())
    {
        string word;
        // Read four points of rectangle
        for (int j=0; j<objectSize; j++)
        {
            for(int i=0; i<4; i++){
                dout << "color";
                dout <<" "<< object[j].R[i] <<" "<< object[j].G[i] <<" "<< object[j].B[i] <<"\n";
                dout << "normal";
                dout <<" "<< object[j].Nx[i] <<" "<< object[j].Ny[i] <<" "<< object[j].Nz[i] <<"\n";
                dout << "point";
                dout <<" "<< object[j].Px[i] <<" "<< object[j].Py[i] <<" "<< object[j].Pz[i] <<"\n";
            }
        }
        //close output file
        dout.close();
    }
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
    inFile("input.txt");
    splitObject();
    calcNormals();
    float lx = userPrompt("Light Direction X:");
    float ly = userPrompt("Light Direction Y:");
    float lz = userPrompt("Light Direction Z:");
    float ka = userPrompt("Ka:");
    float kd = userPrompt("Kd:");
    float ks = userPrompt("Ks:");
    float kp = userPrompt("Kp:");
    calcShading(lx,ly,lz,ka,kd,ks,kp);
    outFile();
    
    return 0;
}