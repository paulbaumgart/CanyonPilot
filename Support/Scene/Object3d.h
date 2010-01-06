#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "Shape3d.h"
#include "../objreader.h"
#include <float.h>

class Object3d : public Shape3d {
  public:
    Object3d(char* file) {
      ObjReader::readObj(file, nVerts, &rawVertices, &rawNormals, &texcoords, nIndices, &indices);
      
      // 9 because there are 3 vertices for each index and 3 floats per vertex
      vertices = new float[nIndices * 9];
      normals = new float[nIndices * 9];
      
      normalize();
    }
    
    ~Object3d() {
      delete[] vertices;
      delete[] normals;
      
      free(rawVertices);
      
      if (rawNormals)
        free(rawNormals);
      
      if (texcoords)
        free(texcoords);
      
      if (indices)
        free(indices);
    }
    
    virtual void drawObject(Matrix4& mat) {
      glMultMatrixd(mat.getPointer());
      
      glEnableClientState(GL_NORMAL_ARRAY);
      glEnableClientState(GL_VERTEX_ARRAY);
      glNormalPointer(GL_FLOAT, 0, normals);
      glVertexPointer(3, GL_FLOAT, 0, vertices);

      glDrawArrays(GL_TRIANGLES, 0, nIndices * 3);

      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_NORMAL_ARRAY);
    }
    
    void scale(double val) {
      
      for (int i = 0; i < nIndices * 3; i++) {
        vertices[i] *= val;
      }
    }
    
    void normalize() {
      float maxX = FLT_MIN, maxY = FLT_MIN, maxZ = FLT_MIN, minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;

      for (int i = 0; i < nVerts * 3; i++) {
        float & maxRef = (i % 3 == 0) ? maxX : (i % 3 == 1) ? maxY : maxZ;
        float & minRef = (i % 3 == 0) ? minX : (i % 3 == 1) ? minY : minZ;
        
        if (rawVertices[i] > maxRef) {
          maxRef = rawVertices[i];
        }
        else if (rawVertices[i] < minRef) {
          minRef = rawVertices[i];
        }
      }
      
      float transX = -(maxX + minX) / 2;
      float transY = -(maxY + minY) / 2;
      float transZ = -(maxZ + minZ) / 2;

      float scaleFactor = fmax(maxX - minX, fmax(maxY - minY, maxZ - minZ));
      
      for (int i = 0; i < nVerts * 3; i++) {
        rawVertices[i] += transX;
        rawVertices[i] /= scaleFactor;
      }
      
      for (int i = 0; i < nIndices; i++) {
        memcpy(vertices + (i * 3), rawVertices + (indices[i] * 3), 3 * sizeof(float));
        memcpy(normals + (i * 3), rawNormals + (indices[i] * 3), 3 * sizeof(float));
      }
    }
  protected:
    int nVerts;
    float *rawNormals;
    float *texcoords;
    float *vertices;
    float *normals;
    float *rawVertices;
    int nIndices;
    int *indices;
};


#endif