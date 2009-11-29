#ifndef _AIRPLANE_H_
#define _AIRPLANE_H_

#include "TransformGroup.h"
#include "Rect.h"
#include "Camera.h"
#include <math.h>

#define LEFT -1
#define RIGHT 1
#define DOWN -1
#define UP 1
#define LR_MAX_TURN 70
#define UD_MAX_TURN 2
#define LR_TURN_MAG 40
#define UD_TURN_MAG 1
#define ROTATIONAL_MAG 2
#define SPEED 160

class Airplane : public TransformGroup {
  public:
    Airplane() {
      allocate(2);
      
      airplane = new TransformGroup(Matrix4::MakeMatrix(), 3);
      leftWingTrans = new TransformGroup(Matrix4::TranslationMatrix(-5, 0, 0), 1);
      rightWingTrans = new TransformGroup(Matrix4::TranslationMatrix(5, 0, 0), 1);
      wing = new Rect(6, 1, 3);
      body = new Rect(4, 3, 5);
      camera = new Camera(Vector3::MakeVector(0, 10, -12), Vector3::MakeVector(0, 5.0, 1), Vector3::MakeVector(0, 1, 0));
      
      leftWingTrans->addChild(*wing);
      rightWingTrans->addChild(*wing);
      
      airplane->addChild(*leftWingTrans);
      airplane->addChild(*rightWingTrans);
      airplane->addChild(*body);
      
      addChild(*camera);
      addChild(*airplane);
      
      lrTurnAccel = 0;
      lrTurnVel = 0;
      direction = Vector3::MakeVector(0, 0, 1);
      position = Vector3::MakeVector(0, 50, 0);
    }
    
    ~Airplane() {
      delete leftWingTrans;
      delete rightWingTrans;
      delete wing;
      delete body;
    }
    
    void step(double elapsed) {
      /**** LEFT/RIGHT TURNING ****/
      double lrTurnMagnitude = LR_TURN_MAG * elapsed;
      
      if (lrTurnAccel != 0) {
        lrTurnVel += lrTurnAccel * lrTurnMagnitude;
      }
      else {
        if (fabs(lrTurnVel) < lrTurnMagnitude) {
          lrTurnVel = 0;
        }
      }
      
      if (lrTurnVel > 0 && lrTurnAccel != RIGHT) {
        lrTurnVel -= lrTurnMagnitude;
      }
      if (lrTurnVel < 0 && lrTurnAccel != LEFT) {
        lrTurnVel += lrTurnMagnitude;
      }
      
      if (lrTurnVel > LR_MAX_TURN) {
        lrTurnVel = LR_MAX_TURN;
      }
      else if (lrTurnVel < -LR_MAX_TURN) {
        lrTurnVel = -LR_MAX_TURN;
      }
      
      /**** UP/DOWN TURNING ****/
      double udTurnMagnitude = UD_TURN_MAG * elapsed;
      
      if (udTurnAccel != 0) {
        udTurnVel += udTurnAccel * udTurnMagnitude;
      }
      else {
        if (fabs(udTurnVel) < udTurnMagnitude) {
          udTurnVel = 0;
        }
      }
      
      if (udTurnVel > 0 && udTurnAccel != UP) {
        udTurnVel -= udTurnMagnitude;
      }
      if (udTurnVel < 0 && udTurnAccel != DOWN) {
        udTurnVel += udTurnMagnitude;
      }
      
      if (udTurnVel > UD_MAX_TURN) {
        udTurnVel = UD_MAX_TURN;
      }
      else if (udTurnVel < -UD_MAX_TURN) {
        udTurnVel = -UD_MAX_TURN;
      }
      
      direction = Matrix4::RotationYMatrix(-lrTurnVel * ROTATIONAL_MAG * elapsed).multiply(direction);
      Vector3 movement = direction + Vector3::MakeVector(0, udTurnVel, 0);
      position = position + movement.scale(SPEED * elapsed);
      double angle = atan2(-direction[Z], direction[X]);
      
      airplane->getMatrix() = Matrix4::RotationZMatrix(lrTurnVel).multiply(Matrix4::RotationXMatrix(-udTurnVel * 20));
      
      getMatrix() = Matrix4::TranslationMatrix(position).multiply(
                    Matrix4::RotationYMatrix(angle * 180 / M_PI + 90));
    }
    
    void turnLeft() {
      lrTurnAccel = LEFT;
    }
    
    void turnRight() {
      lrTurnAccel = RIGHT;
    }

    void turnUp() {
      udTurnAccel = UP;
    }

    void turnDown() {
      udTurnAccel = DOWN;
    }
    
    void udStopTurn() {
      udTurnAccel = 0;
    }
    
    void lrStopTurn() {
      lrTurnAccel = 0;
    }
    
    void setPosition(Vector3 p) {
      position = p;
    }
    
    Vector3 getPosition() {
      return position;
    }

    Vector3 getWingTip(bool right=true) {
      return Matrix4::TranslationMatrix(0, 0, -20).multiply(getMatrix().multiply(airplane->getMatrix()).multiply(Vector3::MakeVector(right ? -8 : 8, 0, -1.5)));
    }
    
    virtual void drawObject(Matrix4& mat) {
      glColor3f(.8, .8, .8);
      TransformGroup::drawObject(mat);
    }
  private:
    TransformGroup *leftWingTrans, *rightWingTrans, *airplane;
    Rect *wing, *body;
    Camera *camera;
    double lrTurnAccel, lrTurnVel, udTurnAccel, udTurnVel;
    Vector3 direction;
    Vector3 position;
};


#endif
