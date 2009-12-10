#ifndef _GAMECONTROLLER_H_
#define _GAMECONTROLLER_H_

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

class GameController : public Controller {
public:
  virtual void initialize() {
    speed = 1;
    timeout = 2;
    killWithLaser = false;
    
    display = new TransformGroup(Matrix4::TranslationMatrix(0, 0, 0), 2);
    airplane = new Airplane(.1, .1, .1);
    laser = new Laser();

    Vector3 p0 = canyon->getFirstPosition();
    
    display->addChild(*airplane);
    display->addChild(*laser);
  }
  
  virtual void step(double elapsed) {
    airplane->step(elapsed * speed);
    laser->step(elapsed);
    
    Vector3 position = airplane->getPosition();
    if (position[Z] > (canyon->getYMin() + 10) * 4) {
      canyon->addSegment();
    }
    
    if (timeout > 0) {
      timeout--;
    }
    
    double distAboveCanyon = canyon->aboveCanyon(airplane->getNose());
    
    if (laser->isDone() && killWithLaser) {
      cerr << "Shot dead by lasers. Don't fly so high!" << endl;
      togglePaused();
      timeout = 25;
    }
    if (timeout <= 0 && canyon->collisionWithPoint(airplane->getWingTip(true))) {
      cerr << "Collision with right wing!" << endl;
      togglePaused();
      timeout = 25;
    }
    else if (timeout <= 0 && canyon->collisionWithPoint(airplane->getWingTip(false))) {
      cerr << "Collision with left wing!" << endl;
      togglePaused();
      timeout = 25;
    }
    else if (timeout <= 0 && canyon->collisionWithPoint(airplane->getNose())) {
      cerr << "Collision with nose!" << endl;
      togglePaused();
      timeout = 25;
    }
    else if (timeout <= 0 && distAboveCanyon > 0 && laser->isDone()) {
      Vector3 planePosition = airplane->getPosition();
      Vector3 laserDirection = Matrix4::RotationYMatrix(180).multiply(airplane->getDirection().cross(Vector3::MakeVector(0, 1, 0)).scale(50));
      laserDirection.print();
      laser->reset(planePosition + laserDirection, planePosition);
      
      int probability = distAboveCanyon;
      if (rand() % 100 < probability) {
        killWithLaser = true;
      }
      else {
        laser->miss();
      }
      cerr << "Above canyon!" << endl;
    }
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
    
    glLoadIdentity();
    skybox->draw(airplane->getDirection());
  }
  
  virtual void keyDownHandler(int key) {
    if (-key == 'a' || key == GLUT_KEY_LEFT) {
      airplane->turnLeft();
    }

    if (-key == 'e' || key == GLUT_KEY_RIGHT) {
      airplane->turnRight();
    }

    if (-key == ',' || key == GLUT_KEY_UP) {
      airplane->turnUp();
    }

    if (-key == 'o' || key == GLUT_KEY_DOWN) {
      airplane->turnDown();
    }

    if (-key == ' ')
      speed = 2;
  }
  
  virtual void keyUpHandler(int key) {
    if (-key == 'a' || -key == 'e' || key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT) {
      airplane->lrStopTurn();
    }
    if (-key == ',' || -key == 'o' || key == GLUT_KEY_UP || key == GLUT_KEY_DOWN) {
      airplane->udStopTurn();
    }

    if (-key == ' ')
      speed = 1;
  }
  
private:
  Airplane* airplane;
  TransformGroup* display;
  Laser* laser;
  int speed, timeout;
  bool killWithLaser;
};

#endif