#ifndef _UTIL_H_
#define _UTIL_H_

#include <sys/time.h>
#include "Support/Vector3.h"

extern int height;
extern int width;

double getMicroTime() {
  struct timeval t;
  gettimeofday(&t, NULL);
  
  return t.tv_sec + (t.tv_usec / 1.0e6);
}

static inline bool outsidePlane (Vector3 point, Vector3 planePoint, Vector3 planeOutwardNormal) {
    return (point - planePoint).dot(planeOutwardNormal) > 0;
}

static bool relativeCCW_3d (Vector3 point, Vector3 start, Vector3 end, Vector3 quadNormal) {
    Vector3 sideNormal = quadNormal.cross(end - start).normalize(); // Points inward
    return outsidePlane(point, start, sideNormal);
}

static bool collisionWithTriangle (Vector3 point, Vector3 triangle[], Vector3 normal) {
    for (int i = 0; i < 3; i++) {
        int endIndex = (i + 1) % 3;
        bool inSide = relativeCCW_3d(point, triangle[i], triangle[endIndex], normal);

        if (!inSide) {
            return false;
        }
    }

    return !outsidePlane(point, triangle[0], normal);
}

bool collisionWithQuad (Vector3 point, Vector3 quad[], Vector3 normal) {
    Vector3 triangles[2][3] = {
        {quad[0], quad[1], quad[2]},
        {quad[0], quad[3], quad[4]}
    };

    return collisionWithTriangle(point, triangles[0], normal) || collisionWithTriangle(point, triangles[1], normal);
}

void setOrthographicProjection() {

  // switch to projection mode
  glMatrixMode(GL_PROJECTION);
  // save previous matrix which contains the 
  //settings for the perspective projection
  glPushMatrix();
  // reset matrix
  glLoadIdentity();
  // set a 2D orthographic projection
  gluOrtho2D(0, width, 0, height);
  // invert the y axis, down is positive
  glScalef(1, -1, 1);
  // mover the origin from the bottom left corner
  // to the upper left corner
  glTranslatef(0, -height, 0);
  glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection() {
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

void renderBitmapString(float x, float y, char *string)
{
  char *c;
  glDisable(GL_LIGHTING);
  glRasterPos2f(x, y);
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
  }
  glEnable(GL_LIGHTING);
}

enum windowCorner {
  kBottomLeft = 0,
  kBottomRight,
  kTopLeft,
  kTopRight
};

void drawText(int x, int y, char* s, enum windowCorner relativeTo) {
  glPushMatrix();
  glColor3f(0.0f,1.0f,1.0f);
  setOrthographicProjection();
  glLoadIdentity();
  switch (relativeTo) {
    case kBottomLeft:
      y = height - y;
      break;
    case kBottomRight:
      y = height - y;
      x = width - x;
      break;
    case kTopRight:
      x = width - x;
      break;
  }

  renderBitmapString(x, y, s);
  resetPerspectiveProjection();
  glPopMatrix();
}

#endif
