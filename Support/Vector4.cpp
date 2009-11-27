#include "Vector4.h"

Vector4 Vector4::MakeVector(double x, double y, double z, double w) {
  Vector4 newVector(x, y, z, w);
  return newVector;
}

Vector4::Vector4() {
  v[0] = 0;
  v[1] = 0;
  v[2] = 0;
  v[3] = 0;
}

Vector4::Vector4(double x, double y, double z, double w) {
  v[0] = x;
  v[1] = y;
  v[2] = z;
  v[3] = w;
}

double* Vector4::getPointer() {
  return &v[0];
}

void Vector4::setElement(int index, double value) {
  v[index] = value;
}

double Vector4::getElement(int index) {
  return v[index];
}

double& Vector4::operator[](int index) {
  return v[index];
}

Vector4 Vector4::add(Vector4 & rhs) {
  return MakeVector(v[0] + rhs[0], v[1] + rhs[1], v[2] + rhs[2], v[3] + rhs[3]);
}

Vector4 Vector4::operator+(Vector4 & rhs) {
  return add(rhs);
}

Vector4 Vector4::subtract(Vector4 & rhs) {
  return MakeVector(v[0] - rhs[0], v[1] - rhs[1], v[2] - rhs[2], v[3] - rhs[3]);
}

Vector4 Vector4::operator-( Vector4 & rhs) {
  return subtract(rhs);
}

Vector4 Vector4::scale(double scalar) {
  return Vector4::MakeVector(v[0] * scalar, v[1] * scalar, v[2] * scalar, v[3] * scalar);
}

double Vector4::dot( Vector4 & rhs) {
  return v[0] * rhs[0] + v[1] * rhs[1] + v[2] * rhs[2];
}

Vector4 Vector4::cross(Vector4 rhs) {
  return Vector4::MakeVector(v[Y] * rhs[Z] - v[Z] * rhs[Y], 
                             v[Z] * rhs[X] - v[X] * rhs[Z], 
                             v[X] * rhs[Y] - v[Y] * rhs[X],
                             1);
}

void Vector4::dehomogenize() {
  scale(1 / v[3]);
}

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

void Vector4::print() {
  std::cout << "Vector: " << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << std::endl;
}