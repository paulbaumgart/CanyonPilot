#ifndef _CANYON_H_
#define _CANYON_H_
#include <pthread.h>
#include <iostream>
#include "../Matrix4.h"

using namespace std;

#define currentSegment (segmentCounter % 3)
#define middleSegment ((segmentCounter + 1) % 3)
#define newestSegment (currentSegment ? currentSegment - 1 : 2)

extern pthread_mutex_t drawMutex;


class Canyon {
public:

  static Canyon* getCanyon() {
    static Canyon* theCanyon = new Canyon();
    return theCanyon;
  }

  int getYMin() {
    int ymin = children[middleSegment]->getYMin();
    return ymin;
  }

  Vector3 getFirstPosition() {
    Vector3 cp0 = children[currentSegment]->getControlPoint(0);
    return cp0.scale(4);
  }

  void draw() {
    for (int i = 0; i < 3; i++) {
      int seg = (currentSegment + i) % 3;
      children[seg]->draw();
    }
  }

  void addSegment() {
    if (!creatingSegment) {
      creatingSegment = true;
      pthread_create(&segmentThread, NULL, threadCreateNewSegment, NULL);
    }
  }

private:
  CanyonSegment* children[3];

  pthread_t segmentThread;
  int segmentCounter;
  bool creatingSegment;

  double* points;
  int mapHeight;
  int mapWidth;
 
  Canyon() {
    int xStart, yStart, xNext;

    mapWidth = mapHeight = 128;
    segmentCounter = 3;
    creatingSegment = false;

    srand(time(NULL));

    xStart = xNext = rand() % mapWidth;
    yStart = 0;
    CanyonSegment* firstSeg = new CanyonSegment(xStart, yStart, xNext, mapWidth, mapHeight, NULL, 0, 1);

    xStart = firstSeg->getControlPoint(3)[X];
    yStart = mapHeight;
    // Calculate x coordinate of 2nd control point so as to maintain C1 continuity
    xNext = firstSeg->getControlPoint(3)[X] * 2 - firstSeg->getControlPoint(2)[X];
    CanyonSegment* secondSeg = new CanyonSegment(xStart, yStart, xNext, mapWidth, mapHeight, firstSeg, 1, 2);

    xStart = secondSeg->getControlPoint(3)[X];
    yStart = 2*mapHeight;
    // Calculate x coordinate of 2nd control point so as to maintain C1 continuity
    xNext = secondSeg->getControlPoint(3)[X] * 2 - secondSeg->getControlPoint(2)[X];
    CanyonSegment* thirdSeg = new CanyonSegment(xStart, yStart, xNext, mapWidth, mapHeight, secondSeg, 2, 3);

    children[0] = firstSeg;
    children[1] = secondSeg;
    children[2] = thirdSeg;
  }

  void createNewSegment() {
    cerr << "Creating new segment." << endl;
    int xStart, yStart, xNext;

    CanyonSegment* oldSegment = children[currentSegment];
    CanyonSegment* lastSegment = children[newestSegment];

    xStart = lastSegment->getControlPoint(3)[X];
    yStart = mapHeight * segmentCounter;
    xNext = lastSegment->getControlPoint(3)[X] * 2 - lastSegment->getControlPoint(2)[X];

    CanyonSegment* newSegment = new CanyonSegment(xStart, yStart, xNext, mapWidth, mapHeight, lastSegment, segmentCounter, segmentCounter + 1);

    // Make sure the canyon isn't currently being drawn when switching out the segments
    pthread_mutex_lock(&drawMutex);
    
    children[currentSegment] = newSegment;
    children[middleSegment]->setPrevious(NULL);
    delete oldSegment;
    segmentCounter++;
    
    pthread_mutex_unlock(&drawMutex);

    creatingSegment = false;
  }

  static void* threadCreateNewSegment(void* data) {
    Canyon* theCanyon = getCanyon();
    theCanyon->createNewSegment();
    pthread_exit(NULL);
  }
};
#endif
