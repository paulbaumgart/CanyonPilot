#ifndef _UTIL_H_
#define _UTIL_H_

#include <sys/time.h>
#include "Support/Vector3.h"

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

#endif
