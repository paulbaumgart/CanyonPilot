#ifndef _BEZIERSURFACE_H_
#define _BEZIERSURFACE_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Shape3d.h"
#include "../Matrix4.h"

#define DEPTH_SEGMENTS 35
#define CIRCULAR_SEGMENTS 35

class BezierSurface : public Shape3d {
  public:
    BezierSurface(int w, int h) {
      numXVectors = w * 3 + 1;
      numYVectors = h * 3 + 1;
      controlPoints = (Vector3 *) malloc(sizeof(Matrix4) * numXVectors * numYVectors);
      pointCounter = 0;
    }
    
    ~BezierSurface() {
      free(controlPoints);
    }
    
    Matrix4 getXMatrix(int xSeg, int yOffset) {
      int xOffset = xSeg * 3;
      /*printf("Matrix at xSeg:%d and yOffset:%d\n", xSeg, yOffset);
      printf("Vector positions: %d, %d, %d, %d\n", numXVectors * yOffset + xOffset,
        numXVectors * yOffset + xOffset + 1,
        numXVectors * yOffset + xOffset + 2,
        numXVectors * yOffset + xOffset + 3);*/
      return Matrix4::MakeMatrix(
        controlPoints[numXVectors * yOffset + xOffset],
        controlPoints[numXVectors * yOffset + xOffset + 1],
        controlPoints[numXVectors * yOffset + xOffset + 2],
        controlPoints[numXVectors * yOffset + xOffset + 3]
      );
    }
    
    Matrix4 getYMatrix(int xOffset, int ySeg) {
      int yOffset = ySeg * 3;
      return Matrix4::MakeMatrix(
        controlPoints[numXVectors * yOffset + xOffset],
        controlPoints[numXVectors * (yOffset + 1) + xOffset],
        controlPoints[numXVectors * (yOffset + 2) + xOffset],
        controlPoints[numXVectors * (yOffset + 3) + xOffset]
      );
    }
    
    Vector4 getPosition(double tx, double ty) {
      int xSeg = tx;
      int yOffset = ((int) ty) * 3;
      Matrix4 controlMatrix;
      
      for (int i = 0; i < 4; i++) {
        Vector3 subPosition = getXMatrix(xSeg, yOffset + i).
            multiply(Matrix4::BezierMatrix()).
            multiply(Vector3::BezierVector(fmod(tx, 1)));
        controlMatrix.setElement(i, 0, subPosition[X]);  
        controlMatrix.setElement(i, 1, subPosition[Y]);  
        controlMatrix.setElement(i, 2, subPosition[Z]);
      }
      
      return controlMatrix.
          multiply(Matrix4::BezierMatrix()).
          multiply(Vector3::BezierVector(fmod(ty, 1)));
    }

    Vector4 getPositionTest(double tx, double ty) {
      int xOffset = ((int) tx) * 3;
      int ySeg = ty;
      Matrix4 controlMatrix;

      for (int i = 0; i < 4; i++) {
        Vector3 subPosition = getYMatrix(xOffset + i, ySeg).
            multiply(Matrix4::BezierMatrix()).
            multiply(Vector3::BezierVector(fmod(ty, 1)));
        controlMatrix.setElement(i, 0, subPosition[X]);  
        controlMatrix.setElement(i, 1, subPosition[Y]);  
        controlMatrix.setElement(i, 2, subPosition[Z]);
      }

      return controlMatrix.
          multiply(Matrix4::BezierMatrix()).
          multiply(Vector3::BezierVector(fmod(tx, 1)));
    }
    
    void sanityCheck() {
      for (double i = 0; i < numXVectors / 3; i += .1) {
        for (double j = 0; j < numYVectors / 3; j += .1) {
          printf("method1: %f == %f\n", getPosition(i, j)[Y], getPositionTest(i, j)[Y]);
          printf("derivs: %f != %f\n", getXPartial(i, j)[Y], getYPartial(i, j)[Y]);
        }
      }
    }
    
    Vector4 getXPartial(double tx, double ty) {
      int xOffset = ((int) tx) * 3;
      int ySeg = ty;
      Matrix4 controlMatrix;
      
      for (int i = 0; i < 4; i++) {
        Vector3 subPosition = getYMatrix(xOffset + i, ySeg).
            multiply(Matrix4::BezierMatrix()).
            multiply(Vector3::BezierVector(fmod(ty, 1)));
        controlMatrix.setElement(i, 0, subPosition[X]);  
        controlMatrix.setElement(i, 1, subPosition[Y]);  
        controlMatrix.setElement(i, 2, subPosition[Z]);
      }
      
      return controlMatrix.
          multiply(Matrix4::BezierMatrix()).
          multiply(Vector3::BezierDerivativeVector(fmod(tx, 1)));
    }
    
    Vector4 getYPartial(double tx, double ty) {
      int xSeg = tx;
      int yOffset = ((int) ty) * 3;
      Matrix4 controlMatrix;
      
      for (int i = 0; i < 4; i++) {
        Vector3 subPosition = getXMatrix(xSeg, yOffset + i).
            multiply(Matrix4::BezierMatrix()).
            multiply(Vector3::BezierVector(fmod(tx, 1)));
        controlMatrix.setElement(i, 0, subPosition[X]);  
        controlMatrix.setElement(i, 1, subPosition[Y]);  
        controlMatrix.setElement(i, 2, subPosition[Z]);
      }
      
      return controlMatrix.
          multiply(Matrix4::BezierMatrix()).
          multiply(Vector3::BezierDerivativeVector(fmod(ty, 1)));
    }
    
    void addPoint(double x, double y, double z) {
      controlPoints[pointCounter++] = Vector3::MakeVector(x, y, z);
    }
    
    void drawObject(Matrix4& mat) {
      glLoadMatrixd(mat.getPointer());
      
      double xSegs = numXVectors / 3;
      double ySegs = numYVectors / 3;
      double step = .1;
      
      /*for (double tx = 0; tx < xSegs; tx += step) {
        glBegin(GL_TRIANGLE_STRIP);
        for (double ty = 0; ty <= ySegs; ty += step) {
          glNormal3dv(getYPartial(tx, ty).cross(getXPartial(tx, ty)).getPointer());
          glVertex3dv(getPosition(tx, ty).getPointer());
          
          glNormal3dv(getYPartial(tx + step, ty).cross(getXPartial(tx + step, ty)).getPointer());
          glVertex3dv(getPosition(tx + step, ty).getPointer());
        }
        glEnd();
      }*/
      
      glBegin(GL_QUADS);
      for (double tx = 0; tx < xSegs; tx += step) {
        for (double ty = 0; ty < ySegs; ty += step) {
          printf("1. ");
          getPosition(tx, ty).print();
          getYPartial(tx, ty).cross(getXPartial(tx, ty)).print();
          glNormal3dv(getYPartial(tx, ty).cross(getXPartial(tx, ty)).getPointer());
          glVertex3dv(getPosition(tx, ty).getPointer());
          
          glNormal3dv(getYPartial(tx, ty + step).cross(getXPartial(tx, ty + step)).getPointer());
          glVertex3dv(getPosition(tx, ty + step).getPointer());
          
          printf("2. ");
          getPosition(tx + step, ty + step).print();
          glNormal3dv(getYPartial(tx + step, ty + step).cross(getXPartial(tx + step, ty + step)).getPointer());
          glVertex3dv(getPosition(tx + step, ty + step).getPointer());
          
          glNormal3dv(getYPartial(tx + step, ty).cross(getXPartial(tx + step, ty)).getPointer());
          glVertex3dv(getPosition(tx + step, ty).getPointer());
        }
      }
      glEnd();
      
      /*int numSegments = pointCounter / 3; // assume that we've completed the last segment
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
      glEnd();*/
    }
    
  private:
    Vector3 * controlPoints;
    int numXVectors;
    int numYVectors;
    int pointCounter;
};

#endif