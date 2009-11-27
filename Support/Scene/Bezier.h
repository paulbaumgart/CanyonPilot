#ifndef _BEZIER_H_
#define _BEZIER_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Shape3d.h"
#include "../Matrix4.h"

#define DEPTH_SEGMENTS 35
#define CIRCULAR_SEGMENTS 35

class Bezier : public Shape3d {
  public:
    Bezier() {
      allocate(4);
    }
    
    Bezier(int amount) {
      allocate(amount);
    }
    
    ~Bezier() {
      free(segments);
    }
    
    Matrix4 getMatrix(int segNumber) {
      return segments[segNumber];
    }
    
    void addPoint(double x, double y) {
      int numSegments = pointCounter / 3;
      int pointOffset = pointCounter % 3;
      
      if (pointOffset == 1 && numSegments == maxSegments) {
        reallocate(maxSegments + 1);
        segments[numSegments].setElement(0, 0, segments[numSegments - 1].getElement(3, 0));
        segments[numSegments].setElement(0, 1, segments[numSegments - 1].getElement(3, 1));
      }
      
      if (pointCounter == 0) {
        segments[numSegments].setElement(0, 0, x);
        segments[numSegments].setElement(0, 1, y);
      }
      else if (pointOffset == 0 && numSegments == maxSegments) {
        segments[numSegments - 1].setElement(3, 0, x);
        segments[numSegments - 1].setElement(3, 1, y);
      }
      else if (pointOffset == 0) {
        segments[numSegments - 1].setElement(3, 0, x);
        segments[numSegments - 1].setElement(3, 1, y);
        segments[numSegments].setElement(0, 0, x);
        segments[numSegments].setElement(0, 1, y);
      }
      else {
        segments[numSegments].setElement(pointOffset, 0, x);
        segments[numSegments].setElement(pointOffset, 1, y);
      }
      
      pointCounter++;
    }

    Vector3 getVertex(Vector3& coord, double rot) {
      return Vector3::MakeVector(sin(rot) * coord[X], coord[Y], cos(rot) * coord[X]);
    }

    Vector4 getNormal(Vector4& coord, double rot) {
      Vector3 n = Vector3::MakeVector(sin(rot) * coord[Y], -coord[X], cos(rot) * coord[Y]);
      n.normalize();
      return n;
    }
    
    void drawObject(Matrix4& mat) {
      glLoadMatrixd(mat.getPointer());
      
      int numSegments = pointCounter / 3; // assume that we've completed the last segment
      double step = 1.0 / DEPTH_SEGMENTS;
      double cStep = 2.0 * M_PI / CIRCULAR_SEGMENTS;
      
      if (textureId != -1)
        glBindTexture(GL_TEXTURE_2D, textureId);
      
      glBegin(GL_QUADS);
      for (int i = 0; i < numSegments; i++) {
        Matrix4 multiplierMatrix = getMatrix(i).multiply(Matrix4::BezierMatrix());
        Vector3 previousCoords = multiplierMatrix.multiply(Vector3::BezierVector(0));
        Vector4 previousNormal = multiplierMatrix.multiply(Vector3::BezierDerivativeVector(0));
        
        for (double t = step; t <= 1.000001; t += step) {
          Vector3 coords = multiplierMatrix.multiply(Vector3::BezierVector(t));
          Vector4 normal = multiplierMatrix.multiply(Vector3::BezierDerivativeVector(t));
          
          for (double c = 0; c < 2 * M_PI; c += cStep) {
            if (textureId != -1)
              glTexCoord2f((c) / (2 * M_PI), 1.0 - ((t - step) + i) / numSegments);
            glNormal3dv(getNormal(previousNormal, c).getPointer());
            glVertex3dv(getVertex(previousCoords, c).getPointer());
            
            if (textureId != -1)
              glTexCoord2f((c + cStep) / (2 * M_PI), 1.0 - ((t - step) + i) / numSegments);
            glNormal3dv(getNormal(previousNormal, c + cStep).getPointer());
            glVertex3dv(getVertex(previousCoords, c + cStep).getPointer());
            
            if (textureId != -1)
              glTexCoord2f((c + cStep) / (2 * M_PI), 1.0 - (t + i) / numSegments);
            glNormal3dv(getNormal(normal, c + cStep).getPointer());
            glVertex3dv(getVertex(coords, c + cStep).getPointer());
            
            if (textureId != -1)
              glTexCoord2f((c) / (2 * M_PI), 1.0 - (t + i) / numSegments);
            glNormal3dv(getNormal(normal, c).getPointer());
            glVertex3dv(getVertex(coords, c).getPointer());
          }
          
          previousCoords = coords;
          previousNormal = normal;
        }
      }
      glEnd();
    }
    
    void allocate(int amount) {
      segments = (Matrix4 *) malloc(sizeof(Matrix4) * amount);
      maxSegments = amount;
      pointCounter = 0;
      for (int i = 0; i < maxSegments; i++) {
        segments[i] = Matrix4::MakeMatrix();
      }
    }
    
    void reallocate(int amount) {
      segments = (Matrix4 *) realloc(segments, sizeof(Matrix4) * amount);
      for (int i = maxSegments; i < amount; i++) {
        segments[i] = Matrix4::MakeMatrix();
      }
      maxSegments = amount;
    }
  private:
    Matrix4 * segments;
    int maxSegments;
    int pointCounter;
};

#endif