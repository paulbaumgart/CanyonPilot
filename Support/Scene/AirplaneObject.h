#ifndef _AIRPLANE_OBJECT_H_
#define _AIRPLANE_OBJECT_H_

#include "Object3d.h"

#define EXPLODE_COEFFICIENT 3
#define GRAVITY_COEFFICIENT 5
#define SPIN_COEFFICIENT 500

class AirplaneObject : public Object3d {
  public:
    AirplaneObject(char* file) : Object3d(file) {
      exploding = false;
      t = 0;
    }
    
    virtual void drawObject(Matrix4& mat) {
      glMultMatrixd(mat.getPointer());
      
      
      if (exploding) {
        glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_CULL_FACE);
        
        for (int i = 0; i < nIndices; i += 3) {
          Vector3 sum = (vertices[indices[i]] + vertices[indices[i + 1]] + vertices[indices[i + 2]]); 
          Vector3 velocity = sum - Vector3::MakeVector(0, -3, -6).scale(3);
          velocity = velocity + Vector3::MakeVector(0, -1, 0).scale(t * t * GRAVITY_COEFFICIENT);
          
          Vector3 k = velocity.scale(EXPLODE_COEFFICIENT * t);
          glPushMatrix();
          glTranslated(k[X], k[Y], k[Z]);
          glRotated(SPIN_COEFFICIENT, sin(sum[X]), cos(sum[Y]), sin(sum[Z]));
          glBegin(GL_TRIANGLES);
          
          if (normals)
            glNormal3fv(&normals[indices[i] * 3]);
          glVertex3dv((vertices[indices[i]]).getPointer());

          if (normals)
            glNormal3fv(&normals[indices[i + 1] * 3]);
          glVertex3dv((vertices[indices[i + 1]]).getPointer());

          if (normals)
            glNormal3fv(&normals[indices[i + 2] * 3]);
          glVertex3dv((vertices[indices[i + 2]]).getPointer());
          
          glEnd();
          glPopMatrix();
        }
        glPopAttrib();
      }
      else {
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < nIndices; i += 3) {
          if (normals)
            glNormal3fv(&normals[indices[i] * 3]);
          glVertex3dv(vertices[indices[i]].getPointer());

          if (normals)
            glNormal3fv(&normals[indices[i + 1] * 3]);
          glVertex3dv(vertices[indices[i + 1]].getPointer());

          if (normals)
            glNormal3fv(&normals[indices[i + 2] * 3]);
          glVertex3dv(vertices[indices[i + 2]].getPointer());
        }
        glEnd();
      }
    }
    
    void step(double elapsed) {
      t += elapsed;
    }
    
    void explode() {
      exploding = true;
    }
  protected:
    bool exploding;
    double t;
};


#endif