#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <GLUT/glut.h>
#include "airplane.h"
#include "Support/Matrix4.h"
#include "Support/Vector3.h"
#include "Support/loadppm.h"
#include "Support/loadpgm.h"
#include "Support/Scene/TransformGroup.h"
#include "Support/Scene/Frustum.h"
#include "Support/Scene/Bezier.h"
#include "Support/Scene/Airplane.h"
#include <sys/time.h>
#include <pthread.h>

using namespace std;

/*typedef struct CanyonSegment {
  double * points;
  int width, height, xOffset, yOffset;
} CanyonSegment;*/

int Window::width  = 512;   // set window width in pixels here
int Window::height = 512;   // set window height in pixels here

double getMicroTime() {
  struct timeval t;
  gettimeofday(&t, NULL);
  
  return t.tv_sec + (t.tv_usec / 1.0e6);
}

Frustum frustum;
Matrix4 baseMatrix;
int turn = 0;
double counter = 0;
double lastTime;

double* points;
int mapHeight;
int mapWidth;

Airplane plane;

TransformGroup display(Matrix4::TranslationMatrix(0, 0, -20), 1);

//----------------------------------------------------------------------------
// Callback method called when window is resized.
void Window::reshapeCallback(int w, int h)
{
  width = w;
  height = h;
  glViewport(0, 0, w, h);  // set new viewport size
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-10.0, 10.0, -10.0, 10.0, 10, 1000.0); // set perspective projection viewing frustum
  frustum.set(-10.0, 10.0, -10.0, 10.0, 10, 1000.0);
}

void idleFunc(void) {
  counter++;
  Window::displayCallback();
}

void xloadData() {
  //points = loadPGM("Heightmap.pgm", mapWidth, mapHeight);
  display.addChild(plane);
}

double getNoise() {
  return (rand() % 2000 - 1000) / 10000.0;
}

void generateMap() {
  mapWidth = mapHeight = 128;
  
  Bezier b(3);
  
  srand ( time(NULL) );
  
  double startPosition = rand() % mapWidth;
  plane.setPosition(Vector3::MakeVector(startPosition * 4, 30, 0));

  b.addPoint(startPosition, 0);
  b.addPoint(rand() % mapWidth, mapHeight / 3);
  b.addPoint(rand() % mapWidth, mapHeight * 2 / 3);
  b.addPoint(rand() % mapWidth, mapHeight);
  
  points = (double *) malloc(mapWidth * mapHeight * sizeof(double));
    
  Matrix4 multiplierMatrix = b.getMatrix(0).multiply(Matrix4::BezierMatrix());
  
  for (int j = 0; j < mapHeight; j++) {
    for (int i = 0; i < mapWidth; i++) {
      double dist = mapHeight + mapWidth;
      for (double t = 0; t < 1; t += .05) {
        Vector3 coords = multiplierMatrix.multiply(Vector3::BezierVector(t));
        double distToCoords = sqrt(pow(i - coords[X], 2) + pow(j - coords[Y], 2));
        dist = (distToCoords < dist) ? distToCoords : dist;
      }
      double height = fmax(fmin(pow(dist / 20, 2) + getNoise(), 1), 0);
      points[j * mapWidth + i] = height;
    }
  }
}

void * threadStartPoint(void *data) {
  generateMap();
  pthread_exit(NULL);
}

void loadData() {
  pthread_t thread;
  pthread_create(&thread, NULL, threadStartPoint, NULL);
  lastTime = getMicroTime();
  display.addChild(plane);
}

Vector3 getPoint(int x, int y) {
  return Vector3::MakeVector(x << 2, ((double)points[mapWidth * y + x]) * 100, y << 2);
}

void setColor(double height) {
  if (height < 10) {
    glColor3d(0, 0, .7);
  }
  else if (height < 20) {
    glColor3d(1, 1, .3);
  }
  else if (height < 45) {
    glColor3d(0, .6, 0);
  }
  else if (height < 70) {
    glColor3d(.3, .3, 0);
  }
  else {
    glColor3d(1, 1, 1);
  }
}

//----------------------------------------------------------------------------
// Callback method called when window readraw is necessary or
// when glutPostRedisplay() was called.
void Window::displayCallback(void)
{
  Matrix4 identity;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
  glMatrixMode(GL_MODELVIEW);
  
  glLoadIdentity();
  
  float specular[] = {0.0, 0.0, 0.0, 1.0};
  float diffuse[] = {1.0, 1.0, 1.0, 1.0};
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT, GL_SPECULAR, diffuse);
  
  glBegin(GL_QUADS);
  glColor3f(.8, .8, .8);
  for (int j = 0; j < mapHeight; j++) {
    for (int i = 0; i < mapWidth; i++) {
      Vector3 v1 = getPoint(i, j);
      Vector3 v2 = getPoint(i, j + 1);
      Vector3 v3 = getPoint(i + 1, j + 1);
      Vector3 v4 = getPoint(i + 1, j);
      Vector3 normal = (v3 - v2).cross(v1 - v2);
      normal.normalize();
      glNormal3dv(normal.getPointer());
      setColor(v1[Y]);
      glVertex3dv(v1.getPointer());
      setColor(v2[Y]);
      glVertex3dv(v2.getPointer());
      setColor(v3[Y]);
      glVertex3dv(v3.getPointer());
      setColor(v4[Y]);
      glVertex3dv(v4.getPointer());
    }
  }
  glEnd();
  
  double t = getMicroTime();
  plane.step(t - lastTime);
  lastTime = t;
  
  display.setCamera(identity);
  display.draw(identity);
  
  glutSwapBuffers();
}

void keyDownHandler(unsigned char key, int, int)
{
  if (key == 'a') {
    plane.turnLeft();
  }
  
  if (key == 'e') {
    plane.turnRight();
  }
  
  if (key == ',') {
    plane.turnUp();
  }
  
  if (key == 'o') {
    plane.turnDown();
  }
  
  Window::displayCallback();
}

void keyUpHandler(unsigned char key, int, int)
{
  if (key == 'a' || key == 'e') {
    plane.lrStopTurn();
  }
  if (key == ',' || key == 'o') {
    plane.udStopTurn();
  }
  
  Window::displayCallback();
}

int main(int argc, char *argv[])
{
  GLfloat mat_specular[]   = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[]  = { 50.0 };
  GLfloat light0_position[] = { 10.0, 10.0, 10.0, 0.0 };
  
  glutInit(&argc, argv);      	      	      // initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
  glutInitWindowSize(Window::width, Window::height);      // set initial window size
  glutCreateWindow("Looking for something?");    	                // open window and set window title

  glEnable(GL_NORMALIZE);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glEnable(GL_COLOR_MATERIAL);
  
  glEnable(GL_DEPTH_TEST);            	      // enable depth buffering
  glClearColor(0.0, 0.0, 0.0, 0.0);   	      // set clear color to black
  glMatrixMode(GL_PROJECTION); 
  
  loadData();
  
  // Generate material properties:
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  
  // Generate light source:
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  
  // Install callback functions:
  glutDisplayFunc(Window::displayCallback);
  glutReshapeFunc(Window::reshapeCallback);
  
  glutIdleFunc(Window::displayCallback);
  glutKeyboardFunc(keyDownHandler);
  glutKeyboardUpFunc(keyUpHandler);
    
  glutMainLoop();
  return 0;
}

