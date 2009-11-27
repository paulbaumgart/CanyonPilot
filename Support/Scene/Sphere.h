#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Shape3d.h"
#include <GLUT/glut.h>

class Sphere : public Shape3d {
  public:
    Sphere(double radius) {
      this->radius = radius;
      this->boundingSphere.radius = radius;
    }
    
    virtual void drawObject(Matrix4& mat) {
      glLoadMatrixd(mat.getPointer());
      glutSolidSphere(radius, 20, 20);
    }
  private:
    double radius;
};


#endif