#ifndef _CANYONSEGMENT_H_
#define _CANYONSEGMENT_H_

#include <iostream>
#include <limits>
using namespace std;

#define BEZIER_POINTS 50
#define DIFFICULTY_COEFFICIENT(d) (sqrt((d) + 2.0) / 40)

class CanyonSegment : public Node {

private:
  double* points;
  int width, height, xMin, yMin, startDifficulty, endDifficulty;
  Vector3 controlPoints[4];
  CanyonSegment* previous;
  
public:  
  CanyonSegment(int xStart, int yStart, int xNext, int w, int h, CanyonSegment* prev, int startDiff, int endDiff) {
    this->width = w;
    this->height = h;
    this->yMin = yStart;
    this->previous = prev;
    this->startDifficulty = startDiff;
    this->endDifficulty = endDiff;
    
    int midpointX = (xStart + xNext) / 2;
    int newControlX1 = (rand() % width) - (width / 2) + midpointX;
    int newControlX2 = (rand() % width) - (width / 2) + midpointX;
    controlPoints[0] = Vector3::MakeVector(xStart, yStart, 0);
    controlPoints[1] = Vector3::MakeVector(xNext, yStart + height / 3.0, 0);
    controlPoints[2] = Vector3::MakeVector(newControlX1, yStart + height * 2 / 3.0, 0);
    controlPoints[3] = Vector3::MakeVector(newControlX2, yStart + height, 0);
    
    int fudgeFactor = 2*(1.0 / DIFFICULTY_COEFFICIENT(min(startDiff, endDiff))) + 5;
    printf("Fudge: %d\n", fudgeFactor);
    this->xMin = fmin(controlPoints[0][X], fmin(controlPoints[1][X], fmin(controlPoints[2][X], controlPoints[3][X]))) - fudgeFactor;
    int xMax = fmax(controlPoints[0][X], fmax(controlPoints[1][X], fmax(controlPoints[2][X], controlPoints[3][X]))) + fudgeFactor;
    this->width = xMax - xMin;
    
    Bezier b(1);

    b.addPoint(controlPoints[0][X] - xMin, controlPoints[0][Y] - yMin);
    b.addPoint(controlPoints[1][X] - xMin, controlPoints[1][Y] - yMin);
    b.addPoint(controlPoints[2][X] - xMin, controlPoints[2][Y] - yMin);
    b.addPoint(controlPoints[3][X] - xMin, controlPoints[3][Y] - yMin);

    points = new double[width * height];

    Matrix4 multiplierMatrix = b.getMatrix(0).multiply(Matrix4::BezierMatrix());
    double stepAmount = 1.0 / BEZIER_POINTS;

    for (int j = 0; j < height; j++) {
      for (int i = 0; i < width; i++) {
        double dist = height + width;
        double bestT = 0;
        for (double t = -5 * stepAmount; t < 1 + 5 * stepAmount; t += stepAmount) {
          Vector3 coords = multiplierMatrix.multiply(Vector3::BezierVector(t));
          double distToCoords = sqrt(pow(i - coords[X], 2) + pow(j - coords[Y], 2));
          if (distToCoords < dist) {
            dist = distToCoords;
            bestT = t;
          }
        }
        double difficulty = fmin(fmax(startDiff + (endDiff - startDiff) * bestT, startDiff), endDiff);
        double pointHeight = fmax(fmin(pow(dist * DIFFICULTY_COEFFICIENT(difficulty), 2) + getNoise(), 1), 0);
        points[j * width + i] = pointHeight * 100;
      }
    }
  }
  
  ~CanyonSegment() {
    delete[] points;
  }

  double getNoise() {
    return (rand() % 2000 - 1000) / 10000.0;
  }
  
  Vector3 getPoint(int x, int y) {
    if (x < width && x >= 0 && y < height && y >= 0) {
      return Vector3::MakeVector((x + xMin) * 4, points[width * y + x], (y + yMin) * 4);
    }
    else {
      return Vector3::MakeVector(numeric_limits<double>::infinity(),numeric_limits<double>::infinity(),numeric_limits<double>::infinity());
    }
  }

  void setColor(double height) {
    if (height < 10) {
      glColor3d(0, 0, .7);
    }
    else if (height < 20) {
      glColor3d(1, 1, .3);
    }
    else if (height < 45) {
      glColor3d(0, .6, 0);
    }
    else if (height < 70) {
      glColor3d(.3, .3, 0);
    }
    else {
      glColor3d(1, 1, 1);
    }
  }
  
  void draw() {
    glBegin(GL_QUADS);
    glColor3f(.8, .8, .8);

    // Connect to previous segment
    if (previous) {
      int dXMin = xMin - previous->getXMin();
      for (int i = 0; i < width - 1; i++) {
        Vector3 v1 = previous->getPoint(i + dXMin, previous->getHeight()-1);
        Vector3 v2 = getPoint(i, 0);
        Vector3 v3 = getPoint(i + 1, 0);
        Vector3 v4 = previous->getPoint(i + dXMin + 1, previous->getHeight()-1);

        if (isinf(v1[X]) || isinf(v4[X])) {
          continue;
        }
        
        Vector3 normal = (v3 - v2).cross(v1 - v2);
        normal.normalize();
        glNormal3dv(normal.getPointer());
        setColor(v1[Y]);
        glVertex3dv(v1.getPointer());
        setColor(v2[Y]);
        glVertex3dv(v2.getPointer());
        setColor(v3[Y]);
        glVertex3dv(v3.getPointer());
        setColor(v4[Y]);
        glVertex3dv(v4.getPointer());
      }
    }

    // Draw this segment
    for (int j = 0; j < height - 1; j++) {
      for (int i = 0; i < width - 1; i++) {
        Vector3 v1 = getPoint(i, j);
        Vector3 v2 = getPoint(i, j + 1);
        Vector3 v3 = getPoint(i + 1, j + 1);
        Vector3 v4 = getPoint(i + 1, j);
        Vector3 normal = (v3 - v2).cross(v1 - v2);
        normal.normalize();
        glNormal3dv(normal.getPointer());
        setColor(v1[Y]);
        glVertex3dv(v1.getPointer());
        setColor(v2[Y]);
        glVertex3dv(v2.getPointer());
        setColor(v3[Y]);
        glVertex3dv(v3.getPointer());
        setColor(v4[Y]);
        glVertex3dv(v4.getPointer());
      }
    }
    glEnd();
  }
  
  int getYMin() {
    return yMin;
  }

  int getXMin() {
    return xMin;
  }

  int getHeight() {
    return height;
  }

  void setPrevious(CanyonSegment* prev) {
    previous = prev;
  }
  
  Vector3 getControlPoint(int n) {
    return controlPoints[n];
  }
};
#endif
