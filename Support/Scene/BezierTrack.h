#ifndef _BEZIERTRACK_H_
#define _BEZIERTRACK_H_

#include "TransformGroup.h"
#include "Bezier.h"

#define STEP_SIZE .4

class BezierTrack : public Group {
  public:
    BezierTrack() {
      bezier = new Bezier(3);
      t = 0;
    }
    
    ~BezierTrack() {
      delete bezier;
    }
    
    void addPoint(double x, double y, double z) {
      bezier->addPoint(x, y, z);
    }
    
    void step(double elapsed) {
      t += elapsed * STEP_SIZE;
      
      double usedT = fmin(t, bezier->getNumSegments() - 1e-9);
      
      Vector3 position = bezier->getPoint(usedT);
      Vector3 velocity = bezier->getTangent(usedT);
      Vector3 acceleration = bezier->getAcceleration(usedT);
      
      for (int i = 0; i < numChildren; i++) {
        children[i]->orient(position, velocity, acceleration);
      }
    }
    
    void setT(double newT) {
      t = newT;
    }
    
    double getT() {
      return t;
    }
    
  private:
    Bezier *bezier;
    double t;
};

#endif