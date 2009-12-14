#include <iostream>
#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "Support/Matrix4.h"
#include "Support/Vector3.h"
#include "Support/loadppm.h"
#include "Support/loadpgm.h"
#include "Support/Scene/TransformGroup.h"
#include "Support/Scene/Airplane.h"
#include "Support/Scene/CanyonSegment.h"
#include "Support/Scene/Canyon.h"
#include "Support/Scene/Skybox.h"
#include "GameController.h"
#include "CutsceneController.h"
#include "Controller.h"
#include "Util.h"

using namespace std;

pthread_mutex_t drawMutex = PTHREAD_MUTEX_INITIALIZER;

int width  = 0;
int height = 0;

void togglePaused();

int textureCount = 1;
double lastTime, fps;
Canyon *canyon;
Skybox *skybox;
GLuint canyonTexture = textureCount++;

bool paused = false;
bool redCollisions = true;
bool skipCutscene = false;

GameController gameController;
CutsceneController cutsceneController;
Controller *activeController;

//----------------------------------------------------------------------------
// Callback method called when window is resized.
void reshapeCallback(int w, int h)
{
  width = w;
  height = h;
  glViewport(0, 0, w, h);  // set new viewport size
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90, (double)w/h, 1, 10000);
  //glFrustum(-10.0, 10.0, -10.0, 10.0, 1, 10000.0); // set perspective projection viewing frustum
}

void loadData() {
  canyon = Canyon::getCanyon();
  skybox = new Skybox(10000);
  gameController.initialize();
  cutsceneController.initialize();

  int tex_w, tex_h;
  unsigned char* image = loadPPM("Textures/desert.pbm", tex_w, tex_h);
  glBindTexture(GL_TEXTURE_2D, canyonTexture);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex_w, tex_h, GL_RGB, GL_UNSIGNED_BYTE, image);
  
  free(image);


  activeController = &cutsceneController;

  lastTime = getMicroTime();
  fps = 0;
}

void step() {
  double t = getMicroTime();

  if (paused) {
    return;
  }

  fps = 1 / (t - lastTime);

  if (activeController == &cutsceneController && cutsceneController.isDone() || skipCutscene) {
    activeController = &gameController;
  } 

  activeController->step(t - lastTime);

  lastTime = t;
}

//----------------------------------------------------------------------------
// Callback method called when window readraw is necessary or
// when glutPostRedisplay() was called.
void displayCallback(void)
{  
  double t = getMicroTime();

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

  activeController->draw();

  char s[10];
  snprintf(s, 10, "FPS: %0.2f", fps);
  drawText(s, kBottomRight);

  glFlush();
  glutSwapBuffers();

  pthread_mutex_unlock(&drawMutex);
}


// Why the negated key for character keys?
// Because GLUT_KEY_UP == 101 == 'd', so this
// is a simple, hackish way of partitioning them.
void keyDownHandler(int key, int, int)
{
  if (-key == 'p')
    togglePaused();
  else if(-key == 27) {
    gameController.saveHighScore();
    exit(0);
  }
  else if (-key == 't') {
    redCollisions = !redCollisions;
  }
  else if (-key == 's') {
    skipCutscene = true;
  }

  activeController->keyDownHandler(key);

  displayCallback();
}

void charKeyDownHandler(unsigned char key, int a, int b) {
  keyDownHandler(-key, a, b);
}

void keyUpHandler(int key, int, int)
{
  activeController->keyUpHandler(key);

  displayCallback();
}

void charKeyUpHandler(unsigned char key, int a, int b) {
  keyUpHandler(-key, a, b);
}

void togglePaused() {
  if (paused) {
    glutIdleFunc(displayCallback);
    lastTime = getMicroTime();
  }
  else {
    glutIdleFunc(NULL);
  }

  paused = !paused;
}

void disableKeyEvents() {
  glutSpecialFunc(0);
  glutSpecialUpFunc(0);
  glutKeyboardFunc(0);
  glutKeyboardUpFunc(0);
}

void enableKeyEvents() {
  glutSpecialFunc(keyDownHandler);
  glutSpecialUpFunc(keyUpHandler);
  glutKeyboardFunc(charKeyDownHandler);
  glutKeyboardUpFunc(charKeyUpHandler);
}

int main(int argc, char *argv[])
{
  GLfloat mat_specular[]   = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[]  = { 50.0 };
  GLfloat light0_position[] = { 10.0, 10.0, 10.0, 0.0 };

  glutInit(&argc, argv);      	      	      // initialize GLUT
  
  width = glutGet(GLUT_SCREEN_WIDTH);   // set window height in pixels here
  height = glutGet(GLUT_SCREEN_HEIGHT);   // set window height in pixels here
 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
  glutInitWindowSize(width, height);      // set initial window size
  glutCreateWindow("Looking for something?");    	                // open window and set window title

  float fogColor[] = {0.5, 0.5, 0.5, 1.0};

  glFogi(GL_FOG_MODE, GL_LINEAR);
  glFogfv(GL_FOG_COLOR, fogColor);
  glFogf(GL_FOG_DENSITY, 0.35f);
  glHint(GL_FOG_HINT, GL_DONT_CARE);
  glFogf(GL_FOG_START, 100.0f);
  glFogf(GL_FOG_END, 500.0f);
  glEnable(GL_FOG);

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
  enableKeyEvents();
 
  glutDisplayFunc(displayCallback);
  glutReshapeFunc(reshapeCallback);

  glutIdleFunc(displayCallback);
  glutFullScreen();

  glutMainLoop();
  return 0;
}

