#ifndef _CUTSCENECONTROLLER_H_
#define _CUTSCENECONTROLLER_H_

#include "Support/Scene/Canyon.h"
#include "Support/Vector3.h"
#include "Support/Scene/TransformGroup.h"
#include "Support/Scene/Airplane.h"
#include "Support/Scene/Canyon.h"
#include "Controller.h"
#include "Util.h"

#define STEP_SIZE .4

extern void togglePaused();
extern Canyon *canyon;

class CutsceneController : public Controller {
public:
  virtual void initialize() {
    track = new BezierTrack();
    airplane = new Airplane();
    display = new TransformGroup(Matrix4::TranslationMatrix(0, 0, 0), 1);
    
    track->addPoint(450, 200, -200);
    track->addPoint(280, 200, -200);
    track->addPoint(210, 250, -340);
    track->addPoint(120, 300, -380);
    track->addPoint(30, 350, -420);
    track->addPoint(0, 50, -260);
    track->addPoint(0, 50, 0);
    
    track->addChild(*airplane);
    
    display->addChild(*track);
  }
  
  virtual void step(double elapsed) {
    track->step(elapsed * STEP_SIZE);
    //track->step(.01);
  }
  
  virtual void draw() {
    Matrix4 identity;
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    display->setCamera(identity);
    
    glPushMatrix();
    display->draw(identity);
    glPopMatrix();
    
    canyon->draw();
  }
  
  virtual void keyDownHandler(int key) {}
  virtual void keyUpHandler(int key) {}
  
  bool isDone() {
    return track->getT() >= 1.999;
  }
private:
  Airplane *airplane;
  BezierTrack *track;
  TransformGroup* display;
};

#endif