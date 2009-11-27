#ifndef _RECT_H_
#define _RECT_H_

#include "Shape3d.h"
#include <math.h>
#include <GLUT/glut.h>

class Rect : public Shape3d {
  public:
    Rect(double x, double y, double z) {
      this->x = x;
      this->y = y;
      this->z = z;
      
      this->boundingSphere.radius = sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
    }
    
    virtual void drawObject(Matrix4& mat) {
      glLoadMatrixd(mat.getPointer());
      
      glBegin(GL_QUADS);
      
      glNormal3d(0, -1, 0);
      glVertex3d( x/2, -y/2, -z/2);
      glVertex3d( x/2, -y/2,  z/2);
      glVertex3d(-x/2, -y/2,  z/2);
      glVertex3d(-x/2, -y/2, -z/2);
      
      glNormal3d(0, 1, 0);
      glVertex3d( x/2,  y/2,  z/2);
      glVertex3d( x/2,  y/2, -z/2);
      glVertex3d(-x/2,  y/2, -z/2);
      glVertex3d(-x/2,  y/2,  z/2);
      
      glNormal3d(-1, 0, 0);
      glVertex3d(-x/2, -y/2,  z/2);
      glVertex3d(-x/2,  y/2,  z/2);
      glVertex3d(-x/2,  y/2, -z/2);
      glVertex3d(-x/2, -y/2, -z/2);
      
      glNormal3d(1, 0, 0);
      glVertex3d( x/2, -y/2, -z/2);
      glVertex3d( x/2,  y/2, -z/2);
      glVertex3d( x/2,  y/2,  z/2);
      glVertex3d( x/2, -y/2,  z/2);
      
      glNormal3d(0, 0, -1);
      glVertex3d( x/2,  y/2, -z/2);
      glVertex3d( x/2, -y/2, -z/2);
      glVertex3d(-x/2, -y/2, -z/2);
      glVertex3d(-x/2,  y/2, -z/2);
      
      glNormal3d(0, 0, 1);
      glVertex3d( x/2, -y/2,  z/2);
      glVertex3d( x/2,  y/2,  z/2);
      glVertex3d(-x/2,  y/2,  z/2);
      glVertex3d(-x/2, -y/2,  z/2);
      
      glEnd();
    }
  private:
    double x, y, z;
};


#endif