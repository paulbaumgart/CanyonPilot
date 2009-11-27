#ifndef _SPOTLIGHT_H_
#define _SPOTLIGHT_H_

#include "Shape3d.h"
#include <GLUT/glut.h>

class Spotlight : public Leaf {
  public:
    Spotlight(Vector3 d) {
      direction = d;
    }
    
    virtual void drawObject(Matrix4& mat) {
      glLoadMatrixd(mat.getPointer());
      
      GLfloat light1_position[] = { 0.0, 0.0, 0.0, 1.0 };
      GLfloat light1_direction[] = { 0.0, -1.0, 1.0, 1.0 };
      
      glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
      glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);
    }
  private:
    Vector3 direction;
};


#endif