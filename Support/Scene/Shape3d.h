#ifndef _SHAPE3D_H_
#define _SHAPE3D_H_

#include "Leaf.h"

int textureCount = 1;

class Shape3d : public Leaf {
protected:
  int textureId;
public:
  Shape3d() {
    textureId = -1;
  }
  
  void setTextureId(int textureId) {
    this->textureId = textureId;
  }
};

#endif