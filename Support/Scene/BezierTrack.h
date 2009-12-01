#ifndef _BEZIERTRACK_H_
#define _BEZIERTRACK_H_

#include "TransformGroup.h"
#include "Bezier.h"

class BezierTrack : public Group {
  public:
    BezierTrack() {
      bezier = new Bezier();
      t = 0;
    }
    
    ~BezierTrack() {
      delete bezier;
    }
    
    void addPoint(double x, double y, double z) {
      bezier->addPoint(x, y, z);
    }
    
    void step(double tStep) {
      t += tStep;
      printf("%f\n", t);
      
      for (int i = 0; i < numChildren; i++) {
        Vector3 v = Vector3::MakeVector(0, 0, 0);
        children[i]->orient(bezier->getPoint(t), bezier->getTangent(t), v);
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