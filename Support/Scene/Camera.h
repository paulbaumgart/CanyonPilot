#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Shape3d.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class Camera : public Leaf {
  public:
    Camera(Vector3 p, Vector3 l, Vector3 u) {
      lookAt = l;
      position = p;
      up = u;
    }
    
    virtual void drawObject(Matrix4& mat) {
      return;
    }
    
    virtual void setCamera(Matrix4& mat) {
      Vector3 tLookAt = mat.multiply(lookAt);
      Vector3 tPosition = mat.multiply(position);
      Vector3 tUp = up;

      gluLookAt(tPosition[X], tPosition[Y], tPosition[Z], 
                tLookAt[X], tLookAt[Y], tLookAt[Z], 
                tUp[X], tUp[Y], tUp[Z]);
    }
  private:
    Vector3 lookAt, position, up;
};


#endif