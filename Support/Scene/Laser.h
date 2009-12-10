#ifndef _LASER_H_
#define _LASER_H_

#include "Leaf.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define LASER_STEP 2

class Laser : public Leaf {
  public:
    Laser() {
      t = 1;
    }
    
    void reset(Vector3 start, Vector3 end) {
      direction = end - start;
      startPosition = start;
      t = 0;
    }
    
    void step(double elapsed) {
      t += elapsed * LASER_STEP;
    }
    
    virtual void drawObject(Matrix4& mat) {
      if (isDone()) {
        return;
      }
    
      glColor3f(1, 0, 0);
      
      glBegin(GL_LINES);
      Vector3 s = startPosition + direction.scale(t);
      Vector3 e = startPosition + direction.scale(t + .1);
      glVertex3dv(s.getPointer());
      glVertex3dv(e.getPointer());
      glEnd();
      return;
    }
    
    bool isDone() {
      return t > .9;
    }
  private:
    Vector3 startPosition, direction;
    double t;
};


#endif