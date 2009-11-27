#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Shape3d.h"
#include <GLUT/glut.h>

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
      glMatrixMode(GL_PROJECTION);
      
      Vector3 tLookAt = mat.multiply(lookAt);
      Vector3 tPosition = mat.multiply(position);
      Vector3 tUp = up;
      
      glLoadIdentity();
      glFrustum(-10.0, 10.0, -10.0, 10.0, 10, 1000.0);
      gluLookAt(tPosition[X], tPosition[Y], tPosition[Z], 
                tLookAt[X], tLookAt[Y], tLookAt[Z], 
                tUp[X], tUp[Y], tUp[Z]);
      
      glMatrixMode(GL_MODELVIEW);
    }
  private:
    Vector3 lookAt, position, up;
};


#endif