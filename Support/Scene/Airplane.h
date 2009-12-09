#ifndef _AIRPLANE_H_
#define _AIRPLANE_H_

#include "TransformGroup.h"
#include "Rect.h"
#include "Camera.h"
#include "BezierTrack.h"
#include "Object3d.h"
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
      
      airplane = new TransformGroup(Matrix4::MakeMatrix(), 1);
      camera = new Camera(Vector3::MakeVector(0, 10, -18), Vector3::MakeVector(0, 5.0, 1), Vector3::MakeVector(0, 1, 0));
      planeObject = new Object3d("Objects/f22.obj");
      planeObject->scale(20);
      
      airplane->addChild(*planeObject);
      
      addChild(*camera);
      addChild(*airplane);
      
      lrTurnAccel = 0;
      lrTurnVel = 0;
      udTurnAccel = 0;
      udTurnVel = 0;
      direction = Vector3::MakeVector(0, 0, 1);
      position = Vector3::MakeVector(0, 50, 0);
    }
    
    ~Airplane() {
      delete planeObject;
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
    
    virtual void orient(Vector3& position, Vector3& velocity, Vector3& acceleration) {
      direction = velocity;
      
      double angleXZ = atan2(-velocity[Z], velocity[X]) * 180 / M_PI + 90;
      double angleY = asin(velocity.normalize()[Y]) * 180 / M_PI;
      Vector3 orthoVel = Vector3::MakeVector(-velocity[Z], 0, velocity[X]);
      double turnAngle = acos(orthoVel.dot(acceleration) / (orthoVel.length() * acceleration.length()));
      
      airplane->getMatrix() = Matrix4::RotationYMatrix(0).multiply(Matrix4::RotationXMatrix(-angleY));
      
      getMatrix() = Matrix4::TranslationMatrix(position).multiply(Matrix4::RotationYMatrix(angleXZ));
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
    
    Vector3 getDirection() {
      return direction;
    }
    
    double getAngle() {
      return atan2(direction[Z], direction[X]);
    }

    Vector3 getWingTip(bool right=true) {
      return getMatrix().multiply(airplane->getMatrix()).multiply(Vector3::MakeVector(right ? -8 : 8, 0, -1.5));
    }
    
    virtual void drawObject(Matrix4& mat) {
      Matrix4 identity;
      glColor3f(.8, .8, .8);
      TransformGroup::drawObject(mat);
    }
  private:
    TransformGroup *airplane;
    Object3d *planeObject;
    Camera *camera;
    double lrTurnAccel, lrTurnVel, udTurnAccel, udTurnVel;
    Vector3 direction;
    Vector3 position;
};


#endif
