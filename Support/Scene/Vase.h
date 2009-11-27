#ifndef _VASE_H_
#define _VASE_H_

#include "Bezier.h"

class Vase : public Bezier {
  public:
    Vase() {
      this->addPoint(0, 0);
      this->addPoint(7, 0);
      this->addPoint(14, 7);
      this->addPoint(18, 11);
      this->addPoint(22, 15);
      this->addPoint(25, 22.5);
      this->addPoint(22, 28);
      this->addPoint(19, 33.5);
      this->addPoint(13.5, 36);
      this->addPoint(17, 40);
      this->addPoint(20.5, 44);
      this->addPoint(19, 49);
      this->addPoint(14.5, 44.5);
      this->addPoint(10, 40);
      this->addPoint(10, 37);
      this->addPoint(11.5, 33);
      this->addPoint(13, 29);
      this->addPoint(17, 28.5);
      this->addPoint(18.5, 22);
      this->addPoint(20, 15.5);
      this->addPoint(7, 6);
      this->addPoint(0, 6);
    }
    
    void drawObject(Matrix4& mat) {
      GLfloat specularA[4] = {1., 1., 1., 1.};
      GLfloat diffuseA[4] = {.8, .8, .8, 1.0};
      glMaterialfv(GL_FRONT, GL_SPECULAR, specularA);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseA);
      
      Bezier::drawObject(mat);
    }
};


#endif