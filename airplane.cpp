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
#include "Support/Scene/CanyonSegment.h"
#include <sys/time.h>
#include <pthread.h>

using namespace std;

int width  = 512;   // set window width in pixels here
int height = 512;   // set window height in pixels here

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
int segmentCounter = 2;
bool creatingSegment = false;
pthread_t segmentThread;
pthread_mutex_t drawMutex = PTHREAD_MUTEX_INITIALIZER;

#define currentSegment (segmentCounter % 2)
#define otherSegment (1 - currentSegment)

CanyonSegment* map[2];

Airplane plane;

TransformGroup display(Matrix4::TranslationMatrix(0, 0, -20), 1);

//----------------------------------------------------------------------------
// Callback method called when window is resized.
void reshapeCallback(int w, int h)
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
  displayCallback();
}

void xloadData() {
  //points = loadPGM("Heightmap.pgm", mapWidth, mapHeight);
  display.addChild(plane);
}

double getNoise() {
  return (rand() % 2000 - 1000) / 10000.0;
}

void generateMap() {
  srand ( time(NULL) );
  
  mapWidth = 128;
  mapHeight = 128;
  map[0] = new CanyonSegment(rand() % mapWidth, 0, rand() % mapWidth, mapWidth, mapHeight);
  map[1] = new CanyonSegment(map[0]->getControlPoint(3)[X], mapHeight, 
        map[0]->getControlPoint(3)[X] * 2 - map[0]->getControlPoint(2)[X], mapWidth, mapHeight);
}

void * threadStartPoint(void *data) {
  generateMap();
  pthread_exit(NULL);
}

void loadData() {
  generateMap();
  //pthread_t thread;
  //pthread_create(&thread, NULL, threadStartPoint, NULL);
  lastTime = getMicroTime();
  display.addChild(plane);
}

void createNewSegment() {
  printf("Creating new segment.\n");
  CanyonSegment *oldSegment = map[currentSegment];
  CanyonSegment *newSegment = new CanyonSegment(map[otherSegment]->getControlPoint(3)[X], mapHeight * segmentCounter, 
        map[otherSegment]->getControlPoint(3)[X] * 2 - map[otherSegment]->getControlPoint(2)[X], mapWidth, mapHeight);
  printf("Allocated and initialized new segment.\n");
  
  pthread_mutex_lock(&drawMutex);
  map[currentSegment] = newSegment;
  segmentCounter++;
  delete oldSegment;
  pthread_mutex_unlock(&drawMutex);
  
  creatingSegment = false;
}

void * threadCreateNewSegment(void *data) {
  createNewSegment();
  pthread_exit(NULL);
}

void createSegmentThread() {
  pthread_create(&segmentThread, NULL, threadCreateNewSegment, NULL);
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

void step() {
  double t = getMicroTime();
  plane.step(t - lastTime);
  
  Vector3 position = plane.getPosition();
  if (position[Z] > (map[otherSegment]->getYMin() + 5) * 4 && !creatingSegment) {
    printf("Generating new segment.\n");
    creatingSegment = true;
    createSegmentThread();
  }
  
  lastTime = t;
}

//----------------------------------------------------------------------------
// Callback method called when window readraw is necessary or
// when glutPostRedisplay() was called.
void displayCallback(void)
{  
  pthread_mutex_lock(&drawMutex);
  Matrix4 identity;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
  glMatrixMode(GL_MODELVIEW);
  
  glLoadIdentity();
  
  float specular[] = {0.0, 0.0, 0.0, 1.0};
  float diffuse[] = {1.0, 1.0, 1.0, 1.0};
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT, GL_SPECULAR, diffuse);
  
  for (int i = 0; i < 2; i++) {
    map[i]->draw();
  }
  
  step();
  
  display.setCamera(identity);
  display.draw(identity);
  
  glutSwapBuffers();
  
  pthread_mutex_unlock(&drawMutex);
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
  
  displayCallback();
}

void keyUpHandler(unsigned char key, int, int)
{
  if (key == 'a' || key == 'e') {
    plane.lrStopTurn();
  }
  if (key == ',' || key == 'o') {
    plane.udStopTurn();
  }
  
  displayCallback();
}

int main(int argc, char *argv[])
{
  GLfloat mat_specular[]   = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[]  = { 50.0 };
  GLfloat light0_position[] = { 10.0, 10.0, 10.0, 0.0 };
  
  glutInit(&argc, argv);      	      	      // initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
  glutInitWindowSize(width, height);      // set initial window size
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
  glutDisplayFunc(displayCallback);
  glutReshapeFunc(reshapeCallback);
  
  glutIdleFunc(displayCallback);
  glutKeyboardFunc(keyDownHandler);
  glutKeyboardUpFunc(keyUpHandler);
    
  glutMainLoop();
  return 0;
}

