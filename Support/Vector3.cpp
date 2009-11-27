#include <math.h>
#include "Vector3.h"

Vector3::Vector3() {
  v[0] = 0;
  v[1] = 0;
  v[2] = 0;
  v[3] = 1;
}

Vector3::Vector3(double x, double y, double z) {
  v[0] = x;
  v[1] = y;
  v[2] = z;
  v[3] = 1;
}

Vector3 Vector3::cross(Vector3 rhs) {
  return Vector3::MakeVector(v[Y] * rhs[Z] - v[Z] * rhs[Y], 
                             v[Z] * rhs[X] - v[X] * rhs[Z], 
                             v[X] * rhs[Y] - v[Y] * rhs[X]);
}

Vector3 Vector3::scale(double scalar) {
  return Vector3::MakeVector(v[0] * scalar, v[1] * scalar, v[2] * scalar);
}

double Vector3::length() {
  return sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
}

void Vector3::normalize() {
  scale(1 / length());
}


Vector3 Vector3::add(Vector3 rhs) {
  return MakeVector(v[0] + rhs[0], v[1] + rhs[1], v[2] + rhs[2]);
}

Vector3 Vector3::operator+(Vector3 rhs) {
  return add(rhs);
}

Vector3 Vector3::subtract(Vector3 rhs) {
  return MakeVector(v[0] - rhs[0], v[1] - rhs[1], v[2] - rhs[2]);
}

Vector3 Vector3::operator-(Vector3 rhs) {
  return subtract(rhs);
}

bool Vector3::operator==(Vector3 rhs) {
  return v[X] == rhs[X] && v[Y] == rhs[Y] && v[Z] == rhs[Z];
}


Vector3 Vector3::MakeVector(double x, double y, double z) {
  Vector3 newVector(x, y, z);
  return newVector;
}

Vector3 Vector3::MakeVector(double vertices[]) {
  Vector3 newVector(vertices[0], vertices[1], vertices[2]);
  return newVector;
}

Vector3 Vector3::BezierVector(double t) {
  return MakeVector(t * t * t, t * t, t);
}

Vector4 Vector3::BezierDerivativeVector(double t) {
  return Vector4::MakeVector(3 * t * t, 2 * t, 1, 0);
}