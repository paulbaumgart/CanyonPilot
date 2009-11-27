#ifndef _APPLE_H_
#define _APPLE_H_

#include "Bezier.h"

class Apple : public Bezier {
  public:
    Apple() {
      this->addPoint(0, 0);
      this->addPoint(4, 0);
      this->addPoint(3, 6);
      this->addPoint(0, 4);
    }
    
    void drawObject(Matrix4& mat) {
      GLfloat specularA[4] = {0, 0, 0, 1};
      GLfloat diffuseA[4] = {1, 1, 1, 1.0};
      glMaterialfv(GL_FRONT, GL_SPECULAR, specularA);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseA);
      
      Bezier::drawObject(mat);
    }
};


#endif