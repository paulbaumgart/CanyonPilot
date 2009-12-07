#ifndef _CUBE_H_
#define _CUBE_H_

#include "Shape3d.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class Cube : public Shape3d {
  public:
    Cube(double size) { this->size = size; }
    
    virtual void drawObject(Matrix4& mat) {
      glLoadMatrixd(mat.getPointer());
      glutSolidCube(size);
    }
  private:
    double size;
};


#endif