#ifndef _CUTSCENECONTROLLER_H_
#define _CUTSCENECONTROLLER_H_

#include "Support/Scene/Canyon.h"
#include "Support/Vector3.h"
#include "Support/Scene/TransformGroup.h"
#include "Support/Scene/Airplane.h"
#include "Support/Scene/Canyon.h"
#include "Support/Scene/Skybox.h"
#include "Support/Scene/Laser.h"
#include "Controller.h"
#include "Util.h"

extern void togglePaused();
extern Canyon *canyon;
extern Skybox *skybox;

class CutsceneController : public Controller {
public:
  virtual void initialize() {
    track = new BezierTrack();
    badTrack = new BezierTrack();
    airplane = new Airplane(.1, .1, .1);
    badPlane = new Airplane(.9, .1, .1);
    laser = new Laser();
    display = new TransformGroup(Matrix4::TranslationMatrix(0, 0, 0), 3);
    
    track->addPoint(600, 190, -190);
    track->addPoint(500, 200, -210);
    track->addPoint(420, 210, -190);
    track->addPoint(350, 200, -200); //
    track->addPoint(280, 190, -210);
    track->addPoint(210, 300, -340);
    track->addPoint(150, 300, -380); //
    track->addPoint(90, 300, -420);
    track->addPoint(0, 50, -260);
    track->addPoint(0, 50, 0);
    
    badTrack->addPoint(650, 190, -190);
    badTrack->addPoint(550, 200, -210);
    badTrack->addPoint(470, 210, -190);
    badTrack->addPoint(400, 200, -200); //
    badTrack->addPoint(330, 190, -210);
    badTrack->addPoint(260, 300, -340);
    badTrack->addPoint(130, 320, -350); //
    badTrack->addPoint(0, 340, -360);
    badTrack->addPoint(-100, 350, -370);
    badTrack->addPoint(-150, 360, -380);
    
    track->addChild(*airplane);
    badTrack->addChild(*badPlane);
    
    display->addChild(*track);
    display->addChild(*badTrack);
    display->addChild(*laser);
  }
  
  virtual void step(double elapsed) {
    track->step(elapsed);
    badTrack->step(elapsed);
    laser->step(elapsed);
    
    if (laser->isDone() && track->getT() < 1.8) {
      laser->reset(badPlane->getNose(), airplane->getNose());
      laser->miss();
    }
  }
  
  virtual void draw() {
    Matrix4 identity;
    
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_FOG);
    
    int view = (track->getT() < 1 && false) ? 1 : 2;
    Vector3 position = airplane->getPosition();
    Vector3 camera = Vector3::MakeVector(250, 250, -320);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (view == 1) {
      display->setCamera(identity);
    }
    else if (view == 2) {
      gluLookAt(camera[X], camera[Y], camera[Z], position[X], position[Y], position[Z], 0, 1, 0);
    }
    
    glPushMatrix();
    display->draw(identity);
    glPopMatrix();
    
    canyon->draw();
    
    glLoadIdentity();
    
    if (view == 1) {
      skybox->draw(airplane->getDirection());
    }
    else if (view == 2) {
      skybox->draw(position - camera);
    }
    
    glPopAttrib();
  }
  
  virtual void keyDownHandler(int key) {}
  virtual void keyUpHandler(int key) {}
  
  bool isDone() {
    return track->getT() >= 3 - 1e-9;
  }
private:
  Airplane *airplane, *badPlane;
  BezierTrack *track, *badTrack;
  Laser *laser;
  TransformGroup* display;
};

#endif