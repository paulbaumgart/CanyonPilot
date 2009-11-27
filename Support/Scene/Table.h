#ifndef _TABLE_H_
#define _TABLE_H_

#include "Bezier.h"

class Table : public Bezier {
  public:
    Table() {
      this->addPoint(0, 0);
      this->addPoint(2, 0);
      this->addPoint(2.5, 2);
      this->addPoint(3, 6);//
      this->addPoint(3.5, 10);
      this->addPoint(8.5, 32);
      this->addPoint(18.5, 35);//
      this->addPoint(28.5, 38);
      this->addPoint(61, 41);
      this->addPoint(40, 41);//
      this->addPoint(19, 41);
      this->addPoint(9.5, 41);
      this->addPoint(0, 41);
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