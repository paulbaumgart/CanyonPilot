#ifndef _LEAF_H_
#define _LEAF_H_

#include "Node.h"

class Leaf : public Node {
  public:
    virtual void drawObject(Matrix4&) {};
};


#endif