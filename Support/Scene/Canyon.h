#ifndef _CANYON_H_
#define _CANYON_H_
#include <pthread.h>
#include <iostream>
#include "../Matrix4.h"

using namespace std;

#define currentSegment (segmentCounter % 2)
#define otherSegment (1 - currentSegment)

extern pthread_mutex_t drawMutex;


class Canyon {
  public:

  static Canyon* getCanyon() {
    static Canyon* theCanyon = new Canyon();
    return theCanyon;
  }

  int getYMin() {
    return children[otherSegment]->getYMin();
  }

  Vector3 getFirstPosition() {
    Vector3 cp0 = children[currentSegment]->getControlPoint(0);
    return cp0.scale(4);
  }

  void draw() {
    for (int i = 0; i < 2; i++) {
      children[i]->draw();
    }
  }

  void addSegment() {
    if (!creatingSegment) {
      creatingSegment = true;
      pthread_create(&segmentThread, NULL, threadCreateNewSegment, NULL);
    }
  }

  private:
  CanyonSegment* children[2];

  pthread_t segmentThread;
  int segmentCounter;
  bool creatingSegment;

  double* points;
  int mapHeight;
  int mapWidth;
 
  Canyon() {
    int xStart, yStart, xNext;

    mapWidth = mapHeight = 128;
    segmentCounter = 2;
    creatingSegment = false;

    srand(time(NULL));

    xStart = xNext = rand() % mapWidth;
    yStart = 0;
    CanyonSegment* firstSeg = new CanyonSegment(xStart, yStart, xNext, mapWidth, mapHeight);

    xStart = firstSeg->getControlPoint(3)[X];
    yStart = mapHeight;
    // Calculate x coordinate of 2nd control point so as to maintain C1 continuity
    xNext = firstSeg->getControlPoint(3)[X] * 2 - firstSeg->getControlPoint(2)[X];
    CanyonSegment* secondSeg = new CanyonSegment(xStart, yStart, xNext, mapWidth, mapHeight);

    children[0] = firstSeg;
    children[1] = secondSeg;
  }

  double getNoise() {
    return (rand() % 2000 - 1000) / 10000.0;
  }

  void createNewSegment() {
    cerr << "Creating new segment." << endl;
    int xStart, yStart, xNext;

    CanyonSegment* oldSegment = children[currentSegment];
    CanyonSegment* holdoverSegment = children[otherSegment];

    xStart = holdoverSegment->getControlPoint(3)[X];
    yStart = mapHeight * segmentCounter;
    xNext = holdoverSegment->getControlPoint(3)[X] * 2 - holdoverSegment->getControlPoint(2)[X];

    CanyonSegment* newSegment = new CanyonSegment(xStart, yStart, xNext, mapWidth, mapHeight);
    cerr << "Allocated and initialized new segment." << endl;

    // Make sure the canyon isn't currently being drawn when switching out the segments
    pthread_mutex_lock(&drawMutex);
    children[currentSegment] = newSegment;
    segmentCounter++;
    delete oldSegment;
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
