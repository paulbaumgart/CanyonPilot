#ifndef _CANYONSEGMENT_H_
#define _CANYONSEGMENT_H_

#include <iostream>
#include <limits>
#include "Bezier.h"
#include "../../Util.h"
using namespace std;


#define BEZIER_POINTS 50
#define DIFFICULTY_COEFFICIENT(d) (sqrt((d) + 2.0) / 40)
#define TOP_OF_CANYON 100

extern GLuint canyonTexture;

class CanyonSegment : public Node {

private:
  double *points, *terrain;
  double *heights;
  int width, height, xMin, yMin, startDifficulty, endDifficulty, xStartOffset, xEndOffset;
  Vector3 controlPoints[4];
  CanyonSegment* previous;
  int collidedx, collidedy;
  int terrainSize;
  double *tangentAngles;
  
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
    int fudgeFactor = 2*(1.0 / DIFFICULTY_COEFFICIENT(min(startDifficulty, endDifficulty))) + 10;
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
    heights = new double[height];
    //tangentAngles = new double[width * height];

    for (int i = 0; i < height; i++) {
      heights[i] = 0;
    }

    Matrix4 multiplierMatrix = b.getMatrix(0).multiply(Matrix4::BezierMatrix());
    double stepAmount = 1.0 / BEZIER_POINTS;
    
    terrain = createTerrain(height, .9);

    bool foundFirst = false;


    
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

        //Vector3 tangent = b.getTangent(bestT); 
        //tangentAngles[j * width + i] = atan2(tangent[X], tangent[Y]) * 180 / M_PI;

        double difficulty = fmin(fmax(startDifficulty + (endDifficulty - startDifficulty) * bestT, startDifficulty), endDifficulty);
        double pointHeight = fmax(fmin(pow(dist * DIFFICULTY_COEFFICIENT(difficulty), 2), 1), 0);
        points[j * width + i] = (pointHeight * TOP_OF_CANYON) + getTerrain(j, i) * (pointHeight) + getNoise() * (1 - pointHeight);
        heights[j] = max(heights[j], points[j * width + i]);
        if (pointHeight != 1.0) {
          foundNonOne = true;
          foundFirst = true;
        }
      }
      if (!foundNonOne) {
        if (foundFirst) {
          if (abs(xStartOffset - i) > 10) {
            xEndOffset = min(xEndOffset, i);
          }
        }
        else {
          xStartOffset = max(xStartOffset, i);
        }
      }
    }
    
    xStartOffset = max(xStartOffset - 10, 0);
    xEndOffset = min(xEndOffset + 10, width);
  }
  
  ~CanyonSegment() {
    delete[] points;
    delete[] terrain;
    delete[] heights;
  }

  double getNoise() {
    double gaussian_rand = 0;
    const int iterations = 8;
    const int range = 30;
    for (int i = 0; i < iterations; i++) {
      gaussian_rand += rand() % range;
    }

    gaussian_rand /= iterations;
    gaussian_rand -= range/2;

    return gaussian_rand;
  }
  
  double getTerrain(int y, int x) {
    // Reflects the x point such that f(-5) == f(5) and f(105) == f(95) if terrainSize == 100
    int absX = abs(x);
    int reflectedX = (absX / terrainSize) % 2 == 0 ? absX % terrainSize : terrainSize - (absX % terrainSize) - 1;
    
    return terrain[terrainSize * y + reflectedX] * 40;
  }
  
  Vector3 getPoint(int x, int y) {
    if (x < width && x >= 0 && y < height && y >= 0) {
      return Vector3::MakeVector((x + xMin) * 4, points[width * y + x], (y + yMin) * 4);
    }
    else {
      return Vector3::MakeVector(numeric_limits<double>::infinity(),numeric_limits<double>::infinity(),numeric_limits<double>::infinity());
    }
  }

  //double getTangentAngle(int x, int y) {
  //  return tangentAngles[width * y + x];
  //}
  
  Vector3 getTerrainPoint(int x, int y) {
    y = min(max(y, 0), height - 1);
    return Vector3::MakeVector((x + xMin) * 4, TOP_OF_CANYON + getTerrain(y, x), (y + yMin) * 4);
  }

  Vector3 getHeightMapCoords(Vector3 point) {
    return Vector3::MakeVector(floor(point[X] / 4.0 - xMin), floor(point[Z] / 4.0 - yMin), 0);
  }

  bool pointOnBackSide(Vector3 point) {
    Vector3 coords = getHeightMapCoords(point);
    int i = coords[X], j = coords[Y];

    Vector3 v1 = getPoint(i, j);
    Vector3 v2 = getPoint(i, j + 1);
    Vector3 v3 = getPoint(i + 1, j + 1);
    Vector3 normal = (v3 - v2).cross(v1 - v2).normalize();

    if ((point-v1).dot(normal) < 0) {
      collidedx = i, collidedy = j;
      return true;
    }
    else {
      return false;
    }
  }
  
  bool aboveCanyon(Vector3 point) {
    Vector3 coords = getHeightMapCoords(point);
    int index = coords[Y];
    
    return point[Y] > heights[index];
  }

  bool fancyPantsPointOnBackSide(Vector3 point) {
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
    if (height < 0) {
      glColor3d(0.2, 0.2, 1);
    }
    else if (height > 10 && height < 15) {
      glColor3d(1, 0.95, 1);
    }
    else if (height > 45 && height < 50) {
      glColor3d(1, 1, 0.9);
    }
    else if (height > 75 && height < 80) {
      glColor3d(0.95, 1, 1);
    }
    else if (height > 98) {
      glColor3d(1, 1, 1);
    }
    else {
      glColor3d(0.5, 0.5, 0.5);
    }

    //float zeros[] = {0,0,0,1};

   // glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zeros);
  }

  void setTexCoords(Vector3& point) {
    const int texture_fudge = 150;
    double max_i = xEndOffset - xStartOffset;
    double max_j = height;

    //Matrix4 rot = Matrix4::RotationZMatrix(getTangentAngle(i,j) + 90);
    //Vector3 texCoords(i / max_i * texture_fudge, j / max_j * texture_fudge, 0 );
    //texCoords = rot.multiply(texCoords);
    //glTexCoord2f(texCoords[X], texCoords[Y]);
    setColor(point[Y]);
    glTexCoord2f(point[X]/texture_fudge, point[Z]/texture_fudge);
  }

  void draw() {
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, canyonTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
            GL_LINEAR_MIPMAP_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    glBegin(GL_QUAD_STRIP);

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

        setTexCoords(v1); 
        glVertex3dv(v1.getPointer());
        
        setTexCoords(v2); 
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
        
        setTexCoords(v1);
        glVertex3dv(v1.getPointer());

        setTexCoords(v2);
        glVertex3dv(v2.getPointer());
      }
      glEnd();
    }

    glPopAttrib();

    glColor3f(1,1,1);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
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

    return terrain;
  }
};
#endif
