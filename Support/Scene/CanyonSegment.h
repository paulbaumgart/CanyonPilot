#ifndef _CANYONSEGMENT_H_
#define _CANYONSEGMENT_H_

#include <iostream>
#include <limits>
#include "Bezier.h"
#include "../../Util.h"
using namespace std;


#define BEZIER_POINTS 50
#define DIFFICULTY_COEFFICIENT(d) (sqrt((d) + 2.0) / 40)

class CanyonSegment : public Node {

private:
  double *points, *terrain;
  int width, height, xMin, yMin, startDifficulty, endDifficulty, xStartOffset, xEndOffset;
  Vector3 controlPoints[4];
  CanyonSegment* previous;
  int collidedx, collidedy;
  int terrainSize;
  
public:
  CanyonSegment(int xStart, int yStart, int w, int h, int difficulty) {
    this->width = w;
    this->height = h;
    this->yMin = yStart;
    this->previous = NULL;
    this->startDifficulty = difficulty;
    this->endDifficulty = difficulty;
    this->collidedx = this->collidedy = -1;
    
    controlPoints[0] = Vector3::MakeVector(xStart, yStart, 0);
    controlPoints[1] = Vector3::MakeVector(xStart, yStart + height / 3.0, 0);
    controlPoints[2] = Vector3::MakeVector(xStart, yStart + height * 2 / 3.0, 0);
    controlPoints[3] = Vector3::MakeVector(xStart, yStart + height, 0);
    
    initialize();
  }
  
  CanyonSegment(int xStart, int yStart, int xNext, int w, int h, CanyonSegment* prev, int startDiff, int endDiff) {
    this->width = w;
    this->height = h;
    this->yMin = yStart;
    this->previous = prev;
    this->startDifficulty = startDiff;
    this->endDifficulty = endDiff;
    this->collidedx = this->collidedy = -1;
    
    int midpointX = (xStart + xNext) / 2;
    int newControlX1 = (rand() % width) - (width / 2) + midpointX;
    int newControlX2 = (rand() % width) - (width / 2) + midpointX;
    controlPoints[0] = Vector3::MakeVector(xStart, yStart, 0);
    controlPoints[1] = Vector3::MakeVector(xNext, yStart + height / 3.0, 0);
    controlPoints[2] = Vector3::MakeVector(newControlX1, yStart + height * 2 / 3.0, 0);
    controlPoints[3] = Vector3::MakeVector(newControlX2, yStart + height, 0);
    
    initialize();
  }
  
  void initialize() {  
    int fudgeFactor = 2*(1.0 / DIFFICULTY_COEFFICIENT(min(startDifficulty, endDifficulty))) + 5;
    printf("Fudge: %d\n", fudgeFactor);
    this->xMin = fmin(controlPoints[0][X], fmin(controlPoints[1][X], fmin(controlPoints[2][X], controlPoints[3][X]))) - fudgeFactor;
    int xMax = fmax(controlPoints[0][X], fmax(controlPoints[1][X], fmax(controlPoints[2][X], controlPoints[3][X]))) + fudgeFactor;
    this->width = xMax - xMin;
    this->xStartOffset = 0;
    this->xEndOffset = width;
    
    Bezier b(1);

    b.addPoint(controlPoints[0][X] - xMin, controlPoints[0][Y] - yMin);
    b.addPoint(controlPoints[1][X] - xMin, controlPoints[1][Y] - yMin);
    b.addPoint(controlPoints[2][X] - xMin, controlPoints[2][Y] - yMin);
    b.addPoint(controlPoints[3][X] - xMin, controlPoints[3][Y] - yMin);

    points = new double[width * height];

    Matrix4 multiplierMatrix = b.getMatrix(0).multiply(Matrix4::BezierMatrix());
    double stepAmount = 1.0 / BEZIER_POINTS;
    
    terrain = createTerrain(height, .9);

    bool foundFirst = false;
    
    printf("height: %d, width: %d\n", height, width);
    
    for (int i = 0; i < width; i++) {
      bool foundNonOne = false;
      for (int j = 0; j < height; j++) {
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
        double difficulty = fmin(fmax(startDifficulty + (endDifficulty - startDifficulty) * bestT, startDifficulty), endDifficulty);
        double pointHeight = fmax(fmin(pow(dist * DIFFICULTY_COEFFICIENT(difficulty), 2), 1), 0);
        points[j * width + i] = (pointHeight * 120) + getTerrain(j, i) * (pointHeight) + getNoise() * (1 - pointHeight);
        if (pointHeight != 1.0) {
          foundNonOne = true;
          foundFirst = true;
        }
      }
      if (!foundNonOne) {
        if (foundFirst) {
          if (abs(xStartOffset - i) > 5) {
            xEndOffset = min(xEndOffset, i);
          }
        }
        else {
          xStartOffset = max(xStartOffset, i);
        }
      }
    }
    
    xStartOffset = max(xStartOffset - 5, 0);
    xEndOffset = min(xEndOffset + 5, width);
  }
  
  ~CanyonSegment() {
    delete[] points;
    delete[] terrain;
  }

  double getNoise() {
    return (rand() % 2000 - 1000) / 100.0;
  }
  
  double getTerrain(int y, int x) {
    if (x >= terrainSize) {
      x = terrainSize * 2 - x - 1;
    }
    return terrain[terrainSize * y + x] * 40 - 20;
  }
  
  Vector3 getPoint(int x, int y) {
    if (x < width && x >= 0 && y < height && y >= 0) {
      return Vector3::MakeVector((x + xMin) * 4, points[width * y + x], (y + yMin) * 4);
    }
    else {
      return Vector3::MakeVector(numeric_limits<double>::infinity(),numeric_limits<double>::infinity(),numeric_limits<double>::infinity());
    }
  }

  Vector3 getHeightMapCoords(Vector3 point) {
    return Vector3::MakeVector(round(point[X] / 4.0 - xMin), round(point[Z] / 4.0 - yMin), 0);
  }

  bool pointOnBackSide(Vector3 point) {
    Vector3 coords = getHeightMapCoords(point);
    int x = coords[X];
    int y = coords[Y];

    for (int i = x-1; i <= x+1; i++) {
        for (int j = y-1; j <= y+1; j++) {
            Vector3 v1 = getPoint(i, j);
            Vector3 v2 = getPoint(i, j + 1);
            Vector3 v3 = getPoint(i + 1, j + 1);
            Vector3 v4 = getPoint(i + 1, j);
            Vector3 normal = (v3 - v2).cross(v1 - v2).normalize();
            Vector3 quad[] = {v1, v2, v3, v4};

            if (collisionWithQuad(point, quad, normal)) {
                collidedx = i;
                collidedy = j;
                return true;
            }
        }
    }

    return false;
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
    glBegin(GL_QUAD_STRIP);
    glColor3f(.8, .8, .8);

    // Connect to previous segment
    if (previous) {
      int dXMin = xMin - previous->getXMin();
      for (int i = xStartOffset; i <= xEndOffset; i++) {
        Vector3 v1 = previous->getPoint(i + dXMin, previous->getHeight()-1);
        Vector3 v2 = getPoint(i, 0);
        Vector3 v3 = getPoint(i + 1, 0);

        if (isinf(v1[X]) || isinf(v3[X])) {
          continue;
        }
        
        Vector3 normal = (v3 - v2).cross(v1 - v2).normalize();
        glNormal3dv(normal.getPointer());
        setColor(v1[Y]);
        glVertex3dv(v1.getPointer());
        setColor(v2[Y]);
        glVertex3dv(v2.getPointer());
      }
    }
    
    glEnd();

    // Draw this segment
    for (int j = 0; j < height; j++) {
      glBegin(GL_QUAD_STRIP);
      for (int i = xStartOffset; i <= xEndOffset; i++) {
        bool makeRed = (i == collidedx && j == collidedy);
        Vector3 v1 = getPoint(i, j);
        Vector3 v2 = getPoint(i, j + 1);
        Vector3 v3 = getPoint(i + 1, j + 1);
        Vector3 normal = (v3 - v2).cross(v1 - v2).normalize();
        glNormal3dv(normal.getPointer());
        makeRed ? glColor3d(1,0,0) : setColor(v1[Y]);
        glVertex3dv(v1.getPointer());
        makeRed ? glColor3d(1,0,0) : setColor(v2[Y]);
        glVertex3dv(v2.getPointer());
      }
      glEnd();
    }
    
    int stepSize = 2;
    
    for (int j = 0; j < height; j += stepSize) {
      glBegin(GL_QUAD_STRIP);
      for (int i = xStartOffset; i >= stepSize; i -= stepSize) {
        bool makeRed = (i == collidedx && j == collidedy);
        Vector3 v1 = getPoint(i, j);
        Vector3 v2 = getPoint(i, j - stepSize);
        Vector3 v3 = getPoint(i - stepSize, j - stepSize);
        Vector3 normal = (v3 - v2).cross(v1 - v2);
        normal.normalize();
        glNormal3dv(normal.getPointer());
        makeRed ? glColor3d(1,0,0) : setColor(v1[Y]);
        glVertex3dv(v1.getPointer());
        makeRed ? glColor3d(1,0,0) : setColor(v2[Y]);
        glVertex3dv(v2.getPointer());
      }
      
      glEnd();
      
      glBegin(GL_QUAD_STRIP);
      for (int i = xEndOffset; i < width; i += stepSize) {
        bool makeRed = (i == collidedx && j == collidedy);
        Vector3 v1 = getPoint(i, j);
        Vector3 v2 = getPoint(i, j + stepSize);
        Vector3 v3 = getPoint(i + stepSize, j + stepSize);
        Vector3 normal = (v3 - v2).cross(v1 - v2);
        normal.normalize();
        glNormal3dv(normal.getPointer());
        makeRed ? glColor3d(1,0,0) : setColor(v1[Y]);
        glVertex3dv(v1.getPointer());
        makeRed ? glColor3d(1,0,0) : setColor(v2[Y]);
        glVertex3dv(v2.getPointer());
      }
      glEnd();
    }
    
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
  
  double terrainNoise(double h, int iteration) {
    return (rand() % 2000 - 1000) / 1000.0 * pow(pow(2, -h), iteration);
  }
  
  double * createTerrain(int size, double h) {
    terrainSize = 1;
    while (terrainSize + 1 < size) {
      terrainSize <<= 1;
    }
    terrainSize += 1;
    printf("terrainsize: %d\n", terrainSize);
    
    double *terrain = new double[terrainSize * terrainSize];

    int step = terrainSize / 2;
    int iteration = 0;

    for (int i = 0; i < terrainSize; i++) {
      for (int j = 0; j < terrainSize; j++) {
        terrain[(i) * terrainSize + (j)] = 0;
      }
    }

    while (step != 0) {
      // diamond step
      for (int j = step; j < terrainSize; j += step * 2) {
        for (int i = step; i < terrainSize; i += step * 2) {
          terrain[(j) * terrainSize + (i)] = (terrain[(j-step) * terrainSize + (i-step)] + 
                                      terrain[(j-step) * terrainSize + (i+step)] + 
                                      terrain[(j+step) * terrainSize + (i-step)] + 
                                      terrain[(j+step) * terrainSize + (i+step)]) / 4 + terrainNoise(h, iteration);
        }
      }

      // square step a
      for (int j = step * 2; j < terrainSize - step * 2; j += step * 2) {
        for (int i = step; i < terrainSize; i += step * 2) {
          if (j == 0) {
            terrain[(j) * terrainSize + (i)] = (terrain[(j+step) * terrainSize + (i)] + 
                                          terrain[(j) * terrainSize + (i-step)] + 
                                          terrain[(j) * terrainSize + (i+step)]) / 3 + terrainNoise(h, iteration);
          }
          else if (j == terrainSize - 1) {
            terrain[(j) * terrainSize + (i)] = (terrain[(j-step) * terrainSize + (i)] + 
                                          terrain[(j) * terrainSize + (i-step)] + 
                                          terrain[(j) * terrainSize + (i+step)]) / 3 + terrainNoise(h, iteration);
          }
          else {
            terrain[(j) * terrainSize + (i)] = (terrain[(j-step) * terrainSize + (i)] + 
                                          terrain[(j+step) * terrainSize + (i)] + 
                                          terrain[(j) * terrainSize + (i-step)] + 
                                          terrain[(j) * terrainSize + (i+step)]) / 4 + terrainNoise(h, iteration);
          }
        }
      }
      for (int j = step; j < terrainSize; j += step * 2) {
        for (int i = 0; i < terrainSize; i += step * 2) {
          if (i == 0) {
            terrain[(j) * terrainSize + (i)] = (terrain[(j-step) * terrainSize + (i)] + 
                                          terrain[(j+step) * terrainSize + (i)] + 
                                          terrain[(j) * terrainSize + (i+step)]) / 3 + terrainNoise(h, iteration);
          }
          else if (i == terrainSize - 1) {
            terrain[(j) * terrainSize + (i)] = (terrain[(j-step) * terrainSize + (i)] + 
                                          terrain[(j+step) * terrainSize + (i)] + 
                                          terrain[(j) * terrainSize + (i-step)]) / 3 + terrainNoise(h, iteration);
          }
          else {
            terrain[(j) * terrainSize + (i)] = (terrain[(j-step) * terrainSize + (i)] + 
                                          terrain[(j+step) * terrainSize + (i)] + 
                                          terrain[(j) * terrainSize + (i-step)] + 
                                          terrain[(j) * terrainSize + (i+step)]) / 4 + terrainNoise(h, iteration);
          }
        }
      }


      step /= 2;
      iteration++;
    }

    /*double min = 100000, max = -100000;

    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        min = fmin(min, terrain[(i) * size + (j)]);
        max = fmax(max, terrain[(i) * size + (j)]);
      }
    }
    
    printf("max: %f, min: %f\n", max, min);

    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        terrain[(i) * size + (j)] = (terrain[(i) * size + (j)] - min) / (max - min);
        //printf("terrain: %f\n", terrain[i * size + j]);
      }
    }*/
    
    return terrain;
  }
};
#endif
