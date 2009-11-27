#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include "Vector4.h"

class Vector3 : public Vector4
{
  public:
    Vector3();        // constructor
    Vector3(double, double, double);
    Vector3 cross(Vector3);
    double length();
    void normalize();
    
    Vector3 add(Vector3);
    Vector3 operator+(Vector3);
    Vector3 subtract(Vector3);
    Vector3 operator-(Vector3);
    Vector3 scale(double);
    bool operator==(Vector3);
    
    static Vector3 MakeVector(double, double, double);
    static Vector3 MakeVector(double[]);
    static Vector3 Vector3::BezierVector(double);
    static Vector4 Vector3::BezierDerivativeVector(double);
};

#endif