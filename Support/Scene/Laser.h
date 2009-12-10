#ifndef _LASER_H_
#define _LASER_H_

#include "Leaf.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define LASER_STEP 5

class Laser : public Leaf {
  public:
    Laser() {
      t = 1;
    }
    
    void miss() {
      direction = (direction + Vector3::MakeVector(rand() % 15 - 7, rand() % 15 - 7, rand() % 15 - 7)).scale(5);
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
      if (t > .9) {
        return;
      }
      
      glMultMatrixd(mat.getPointer());
    
      glPushAttrib(GL_ENABLE_BIT);
      glDisable(GL_LIGHTING);
      glDisable(GL_BLEND);
      glDisable(GL_FOG);
      
      glColor3f(1, 0, 0);
      
      glLineWidth(3);
      glBegin(GL_LINES);
      Vector3 s = startPosition + direction.scale(t);
      Vector3 e = startPosition + direction.scale(t + .2);
      glVertex3dv(s.getPointer());
      glVertex3dv(e.getPointer());
      glEnd();
      
      glPopAttrib();
    }
    
    bool isDone() {
      return t > 2.2;
    }
  private:
    Vector3 startPosition, direction;
    double t;
};


#endif