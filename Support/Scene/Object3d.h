#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "Shape3d.h"
#include "../objreader.h"
#include <float.h>

class Object3d : public Shape3d {
  public:
    Object3d(char* file) {
      float *rawVertices;
      ObjReader::readObj(file, nVerts, &rawVertices, &normals, &texcoords, nIndices, &indices);
      
      vertices = new Vector3[nVerts];
      
      for (int i = 0; i < nVerts; i++) {
        vertices[i] = Vector3::MakeVector(rawVertices[i*3], rawVertices[i*3 + 1], rawVertices[i*3 + 2]);
      }
      
      normalize();
    }
    
    ~Object3d() {
      delete[] vertices;
    }
    
    virtual void drawObject(Matrix4& mat) {
      glMultMatrixd(mat.getPointer());
      
      glBegin(GL_TRIANGLES);
      glColor3f(.8, .8, .8);
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
    
    void scale(double val) {
      Matrix4 s = Matrix4::ScaleMatrix(val, val, val);
      boundingSphere.radius *= val;
      
      for (int i = 0; i < nVerts; i++) {
        vertices[i] = s.multiply(vertices[i]);
      }
    }
    
    void normalize() {
      float maxX = FLT_MIN, maxY = FLT_MIN, maxZ = FLT_MIN, minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;

      for (int i = 0; i < nVerts; i++) {
        if (vertices[i][X] > maxX) {
          maxX = vertices[i][X];
        }
        else if (vertices[i][X] < minX) {
          minX = vertices[i][X];
        }

        if (vertices[i][Y] > maxY) {
          maxY = vertices[i][Y];
        }
        else if (vertices[i][Y] < minY) {
          minY = vertices[i][Y];
        }

        if (vertices[i][Z] > maxZ) {
          maxZ = vertices[i][Z];
        }
        else if (vertices[i][Z] < minZ) {
          minZ = vertices[i][Z];
        }
      }

      Vector3 transVector = 
          Vector3::MakeVector(-(maxX + minX) / 2, -(maxY + minY) / 2, -(maxZ + minZ) / 2);
      Matrix4 transMatrix = Matrix4::TranslationMatrix(transVector);

      float scaleFactor = fmax(maxX - minX, fmax(maxY - minY, maxZ - minZ));

      Matrix4 scaleMatrix;
      scaleMatrix.scale(1/scaleFactor, 1/scaleFactor, 1/scaleFactor);

      Matrix4 normMatrix = scaleMatrix.multiply(transMatrix);
      
      boundingSphere.radius = 0;
      for (int i = 0; i < nVerts; i++) {
        vertices[i] = normMatrix.multiply(vertices[i]);
        
        if (vertices[i].length() > boundingSphere.radius) {
          boundingSphere.radius = vertices[i].length();
        }
      }
    }
  private:
    int nVerts;
    Vector3 *vertices;
    float *normals;
    float *texcoords;
    int nIndices;
    int *indices;
};


#endif