#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include "../Vector3.h"
#include <stdio.h>

class Frustum {
  double k[6];
  Vector3 normal[6];
  
  public:
    void set(double left, double right, double bottom, double top, double near, double far) {
      near *= -1;
      far *= -1;
      
      Vector3 nearBottomRight = Vector3::MakeVector(right, bottom, near);
      Vector3 nearTopRight = Vector3::MakeVector(right, top, near);
      Vector3 nearBottomLeft = Vector3::MakeVector(left, bottom, near);
      Vector3 nearTopLeft = Vector3::MakeVector(left, top, near);
      
      double farRight = right * far / near;
      double farLeft = left * far / near;
      double farTop = top * far / near;
      double farBottom = bottom * far / near;
      
      Vector3 farBottomRight = Vector3::MakeVector(farRight, farBottom, far);
      Vector3 farTopRight = Vector3::MakeVector(farRight, farTop, far);
      Vector3 farBottomLeft = Vector3::MakeVector(farLeft, farBottom, far);
      Vector3 farTopLeft = Vector3::MakeVector(farLeft, farTop, far);
      
      normal[0] = (nearBottomLeft - nearBottomRight).cross(nearTopRight - nearBottomRight);
      normal[0].normalize();
      k[0] = normal[0].dot(nearTopRight);
      
      normal[1] = (farBottomRight - farBottomLeft).cross(farTopLeft - farBottomLeft);
      normal[1].normalize();
      k[1] = normal[1].dot(farTopLeft);
      
      normal[2] = (nearBottomRight - farBottomRight).cross(farTopRight - farBottomRight);
      normal[2].normalize();
      k[2] = normal[2].dot(farTopRight);
      
      normal[3] = (farBottomLeft - nearBottomLeft).cross(nearTopLeft - nearBottomLeft);
      normal[3].normalize();
      k[3] = normal[3].dot(nearTopLeft);
      
      normal[4] = (nearTopLeft - nearTopRight).cross(farTopRight - nearTopRight);
      normal[4].normalize();
      k[4] = normal[4].dot(farTopRight);
      
      normal[5] = (nearBottomRight - nearBottomLeft).cross(farBottomLeft - nearBottomLeft);
      normal[5].normalize();
      k[5] = normal[5].dot(farBottomLeft);
    }
    
    bool intersectsSphere(Vector3 center, double radius) {
      for (int i = 0; i < 6; i++) {
        if (normal[i].dot(center) - k[i] < -radius) {
          return false;
        }
      }
      
      return true;
    }
};

#endif
