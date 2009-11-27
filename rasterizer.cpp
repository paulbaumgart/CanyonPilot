#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include "Support/Matrix4.h"
#include "Support/Vector3.h"
#include <sys/time.h>
#include "Support/Scene/Bezier.h"

typedef Vector3 Bary;
#define MakeBary(x, y, z) Vector3::MakeVector(x, y, z)
#define ALPHA 0
#define BETA 1
#define GAMMA 2
#define LINEAR true
#define HYPERBOLIC false
#define SEGMENTATION_LOWER_BOUND 20

using namespace std;

static int window_width = 512, window_height = 512;
static float* pixels = new float[window_width * window_height * 3];

// Clear frame buffer
void clearBuffer()
{
  float clearColor[] = {0.0, 0.0, 0.0};   // clear color: black
  for (int i=0; i<window_width*window_height; ++i)
  {
    pixels[i*3]   = clearColor[0];
    pixels[i*3+1] = clearColor[1];
    pixels[i*3+2] = clearColor[2];
  }  
}

// Draw a point into the frame buffer
void drawPoint(int x, int y, double z, float r, float g, float b)
{
  
  if (x >= window_width || x < 0 || y >= window_height || y < 0)
    return;
  int offset = y*window_width*3 + x*3;
    pixels[offset]   = r;
    pixels[offset+1] = g;
    pixels[offset+2] = b;
}

double getNoise() {
  return (rand() % 2000 - 1000) / 10000.0;
}

void rasterize()
{
  
  Bezier b(3);
  
  srand ( time(NULL) );

  b.addPoint(rand() % 128, 0);
  b.addPoint(rand() % 128, 40);
  b.addPoint(rand() % 128, 80);
  b.addPoint(rand() % 128, 128);
    
  Matrix4 multiplierMatrix = b.getMatrix(0).multiply(Matrix4::BezierMatrix());
  
  for (int j = 0; j < 128; j++) {
    for (int i = 0; i < 128; i++) {
      double dist = 1024;
      double bestT = 0;
      for (double t = 0; t < 1; t += .03) {
        Vector3 coords = multiplierMatrix.multiply(Vector3::BezierVector(t));
        double distToCoords = sqrt(pow(i - coords[X], 2) + pow(j - coords[Y], 2));
        if (distToCoords < dist) {
          dist = distToCoords;
          bestT = t;
        }
      }
      double height = fmax(fmin(pow(dist / (5 + bestT * 3), 2) + getNoise(), 1), 0);
      drawPoint(i, j, 0, height, height, height);
    }
  }
}

void display()
{
  clearBuffer();
  rasterize();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDrawPixels(window_width, window_height, GL_RGB, GL_FLOAT, pixels);

  glutSwapBuffers();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(window_width, window_height);
  glutCreateWindow("CSE 167 Project 3");

  glutDisplayFunc(display);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0, 0.0, 0.0, 1.0); 
  glutMainLoop();
}
