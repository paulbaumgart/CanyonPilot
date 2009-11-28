#include <iostream>
#include <math.h>
#include <GLUT/glut.h>
#include "Support/Matrix4.h"
#include "Support/Vector3.h"
#include "Support/loadppm.h"
#include "Support/loadpgm.h"
#include "Support/Scene/TransformGroup.h"
#include "Support/Scene/Frustum.h"
#include "Support/Scene/Bezier.h"
#include "Support/Scene/Airplane.h"
#include "Support/Scene/CanyonSegment.h"
#include "Support/Scene/Canyon.h"
#include <sys/time.h>

using namespace std;

pthread_mutex_t drawMutex = PTHREAD_MUTEX_INITIALIZER;

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

Airplane plane;

bool paused = false;

int speed = 1;

int timeout = 0;

Canyon* canyon;

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
  glFrustum(-10.0, 10.0, -10.0, 10.0, 10, 100000.0); // set perspective projection viewing frustum
  frustum.set(-10.0, 10.0, -10.0, 10.0, 10, 100000.0);
}

void loadData() {
  lastTime = getMicroTime();
  canyon = Canyon::getCanyon();

  Vector3 p0 = canyon->getFirstPosition();
  plane.setPosition(Vector3::MakeVector(p0[X], 50, p0[Z]));
  
  display.addChild(plane);
}

void step() {
  double t = getMicroTime();
  
  if (!paused)
    plane.step(speed*(t - lastTime));
  
  lastTime = t;

  if (paused)
    return;
  
  Vector3 position = plane.getPosition();
  if (position[Z] > (canyon->getYMin() + 10) * 4) {
    canyon->addSegment();
  }

  if (!paused && timeout > 0) {
    timeout--;
    cerr << timeout << endl;
  }

  if (timeout <= 0 && canyon->collisionWithPoint(plane.getWingTip(true))) {
    cerr << "Collision with right wing!" << endl;
    paused = true;
    timeout = 25;
  }
  else if (timeout <= 0 && canyon->collisionWithPoint(plane.getWingTip(false))) {
    cerr << "Collision with left wing!" << endl;
    paused = true;
    timeout = 25;
  }
  
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
  
  step();
  
  display.setCamera(identity);
  display.draw(identity);
  
  glLoadIdentity();
  canyon->draw();

  glFlush();
  glutSwapBuffers();
  
  pthread_mutex_unlock(&drawMutex);
}


// Why the negated key for character keys?
// Because GLUT_KEY_UP == 101 == 'd', so this
// is a simple, hackish way of partitioning them.
void keyDownHandler(int key, int, int)
{
  if (-key == 'a' || key == GLUT_KEY_LEFT) {
    plane.turnLeft();
  }
  
  if (-key == 'e' || key == GLUT_KEY_RIGHT) {
    plane.turnRight();
  }
  
  if (-key == ',' || key == GLUT_KEY_UP) {
    plane.turnUp();
  }
  
  if (-key == 'o' || key == GLUT_KEY_DOWN) {
    plane.turnDown();
  }

  if (-key == 'p')
    paused = !paused;

  if (-key == ' ')
    speed = 2;
  
  displayCallback();
}

void charKeyDownHandler(unsigned char key, int a, int b) {
  keyDownHandler(-key, a, b);
}

void keyUpHandler(int key, int, int)
{
  if (-key == 'a' || -key == 'e' || key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT) {
    plane.lrStopTurn();
  }
  if (-key == ',' || -key == 'o' || key == GLUT_KEY_UP || key == GLUT_KEY_DOWN) {
    plane.udStopTurn();
  }

  if (-key == ' ')
    speed = 1;
  
  displayCallback();
}

void charKeyUpHandler(unsigned char key, int a, int b) {
  keyUpHandler(-key, a, b);
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
  glutSpecialFunc(keyDownHandler);
  glutSpecialUpFunc(keyUpHandler);
  glutKeyboardFunc(charKeyDownHandler);
  glutKeyboardUpFunc(charKeyUpHandler);
    
  glutMainLoop();
  return 0;
}

