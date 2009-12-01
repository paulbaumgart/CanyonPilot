#ifndef _NODE_H_
#define _NODE_H_

#include "../Matrix4.h"
#include "../Vector3.h"
#include "Frustum.h"

//extern Frustum frustum;
//extern bool culling;

struct BoundingSphere {
  BoundingSphere() {
    radius = 10000;
    center = Vector3::MakeVector(0, 0, 0);
  }
  double radius;
  Vector3 center;
};

class Node {
  protected:
    virtual void drawObject(Matrix4& mat) {};
    BoundingSphere boundingSphere;
  public:
    virtual void orient(Vector3 position, Vector3 velocity, Vector3 acceleration) {}
    virtual void setCamera(Matrix4& mat) {}
    virtual void draw(Matrix4& mat) {
      drawObject(mat);
    }
};

#endif