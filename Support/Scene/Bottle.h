#ifndef _BOTTLE_H_
#define _BOTTLE_H_

#include "Bezier.h"

class Bottle : public Bezier {
  public:
    Bottle() {
      this->addPoint(0, 0);
      this->addPoint(12, -3);
      this->addPoint(10, 9);
      this->addPoint(10, 18);
      this->addPoint(10, 27);
      this->addPoint(5, 27);
      this->addPoint(5, 37);
      this->addPoint(5, 47);
      this->addPoint(6, 46);
      this->addPoint(0, 46);
    }
    
    void drawObject(Matrix4& mat) {
      GLfloat specularA[4] = {1., 1., 1., 1.};
      GLfloat diffuseA[4] = {1, 1, 1, 1.0};
      glMaterialfv(GL_FRONT, GL_SPECULAR, specularA);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseA);
      
      Bezier::drawObject(mat);
    }
};


#endif