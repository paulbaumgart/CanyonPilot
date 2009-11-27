#include "Matrix4.h"
#include <math.h>

#define DEGREES_TO_RADIANS(degs) ((degs) * M_PI / 180.0)

Matrix4 BEZIER_MATRIX = Matrix4::MakeMatrix(
  -1, 3, -3, 1, 
  3, -6, 3, 0, 
  -3, 3, 0, 0, 
  1, 0, 0, 0);

Matrix4::Matrix4()
{
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      m[i][j] = i == j ? 1 : 0;
    }
  }
} 
Matrix4::Matrix4(
  double m00, double m01, double m02, double m03,
  double m10, double m11, double m12, double m13,
  double m20, double m21, double m22, double m23,
  double m30, double m31, double m32, double m33 )
{
  m[0][0] = m00;
  m[1][0] = m01;
  m[2][0] = m02;
  m[3][0] = m03;
  m[0][1] = m10;
  m[1][1] = m11;
  m[2][1] = m12;
  m[3][1] = m13;
  m[0][2] = m20;
  m[1][2] = m21;
  m[2][2] = m22;
  m[3][2] = m23;
  m[0][3] = m30;
  m[1][3] = m31;
  m[2][3] = m32;
  m[3][3] = m33;
}

Matrix4::Matrix4(double * values[4][4]) {
  for (int i=0; i<4; ++i) {
    for (int j=0; j<4; ++j) {
      m[i][j] = *(values[i][j]);
    }
  }
}

double* Matrix4::getPointer()
{
  return &m[0][0];
}

void Matrix4::setElement(int index, double value) {
  m[index % 4][index / 4] = value; // column major
}

double Matrix4::getElement(int index) {
  return m[index % 4][index / 4]; // column major
}

double Matrix4::getElement(int col, int row) {
  return m[col][row];
}

void Matrix4::setElement(int col, int row, double value) {
  m[col][row] = value;
}

double Matrix4::operator[](int index) {
  return getElement(index);
}

Matrix4 Matrix4::multiply(Matrix4 rhs) {
  Matrix4 newMatrix;
  
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      double val = 0;
      for (int n = 0; n < 4; n++) {
        val += getElement(n, i) * rhs.getElement(j, n);
      }
      newMatrix.setElement(j, i, val);
    }
  }
  
  return newMatrix;
}

Vector4 Matrix4::multiply(Vector4 rhs) {
  Vector4 newVector;
  
  for (int i = 0; i < 4; i++) {
    double val = 0;
    for (int n = 0; n < 4; n++) {
      val += getElement(n, i) * rhs[n];
    }
    newVector.setElement(i, val);
  }
  
  return newVector;
}

Vector3 Matrix4::multiply(Vector3 rhs) {
  Vector3 newVector;
  
  for (int i = 0; i < 4; i++) {
    double val = 0;
    for (int n = 0; n < 4; n++) {
      val += getElement(n, i) * rhs[n];
    }
    newVector.setElement(i, val);
  }
  
  return newVector;
}

void Matrix4::identity()
{
  double ident[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      m[i][j] = ident[i][j];
    }
  }
}

// angle in degrees
void Matrix4::rotateX(double angle)
{
  double th = DEGREES_TO_RADIANS(angle);
	m[0][0] = 1;
	m[1][0] = 0;
	m[2][0] = 0;
	m[3][0] = 0;
	m[0][1] = 0;
	m[1][1] = cos(th);
	m[2][1] = -sin(th);
	m[3][1] = 0;
	m[0][2] = 0;
	m[1][2] = sin(th);
	m[2][2] = cos(th);
	m[3][2] = 0;
	m[0][3] = 0;
	m[1][3] = 0;
	m[2][3] = 0;
	m[3][3] = 1;
}

// angle in degrees
void Matrix4::rotateY(double angle)
{
  double th = DEGREES_TO_RADIANS(angle);
	m[0][0] = cos(th);
	m[1][0] = 0;
	m[2][0] = sin(th);
	m[3][0] = 0;
	m[0][1] = 0;
	m[1][1] = 1;
	m[2][1] = 0;
	m[3][1] = 0;
	m[0][2] = -sin(th);
	m[1][2] = 0;
	m[2][2] = cos(th);
	m[3][2] = 0;
	m[0][3] = 0;
	m[1][3] = 0;
	m[2][3] = 0;
	m[3][3] = 1;
}

// angle in degrees
void Matrix4::rotateZ(double angle)
{
  double th = DEGREES_TO_RADIANS(angle);
	m[0][0] = cos(th);
	m[1][0] = -sin(th);
	m[2][0] = 0;
	m[3][0] = 0;
	m[0][1] = sin(th);
	m[1][1] = cos(th);
	m[2][1] = 0;
	m[3][1] = 0;
	m[0][2] = 0;
	m[1][2] = 0;
	m[2][2] = 1;
	m[3][2] = 0;
	m[0][3] = 0;
	m[1][3] = 0;
	m[2][3] = 0;
	m[3][3] = 1;
}

// angle in degrees
void Matrix4::rotateArbitrary(Vector4 a, double angle)
{
  double th = DEGREES_TO_RADIANS(angle);
	m[0][0] = 1 + (1 - cos(th)) * (pow(a[X], 2) - 1);
	m[1][0] = -a[Z] * sin(th) + (1 - cos(th)) * a[X] * a[Y];
	m[2][0] = a[Y] * sin(th) + (1 - cos(th)) * a[X] * a[Z];
	m[3][0] = 0;
	m[0][1] = a[Z] * sin(th) + (1 - cos(th)) * a[Y] * a[X];
	m[1][1] = 1 + (1 - cos(th)) * (pow(a[Y], 2) - 1);
	m[2][1] = -a[X] * sin(th) + (1 - cos(th)) * a[Y] * a[Z];
	m[3][1] = 0;
	m[0][2] = -a[Y] * sin(th) + (1 - cos(th)) * a[X] * a[Z];
	m[1][2] = a[X] * sin(th) + (1 - cos(th)) * a[Z] * a[Y];;
	m[2][2] = 1 + (1 - cos(th)) * (pow(a[Z], 2) - 1);
	m[3][2] = 0;
	m[0][3] = 0;
	m[1][3] = 0;
	m[2][3] = 0;
	m[3][3] = 1;
}


void Matrix4::scale(double x, double y, double z) {
  m[0][0] = x;
	m[1][0] = 0;
	m[2][0] = 0;
	m[3][0] = 0;
	m[0][1] = 0;
	m[1][1] = y;
	m[2][1] = 0;
	m[3][1] = 0;
	m[0][2] = 0;
	m[1][2] = 0;
	m[2][2] = z;
	m[3][2] = 0;
	m[0][3] = 0;
	m[1][3] = 0;
	m[2][3] = 0;
	m[3][3] = 1;
}

void Matrix4::translate(Vector4 t) {
  m[0][0] = 1;
	m[1][0] = 0;
	m[2][0] = 0;
	m[3][0] = t[X];
	m[0][1] = 0;
	m[1][1] = 1;
	m[2][1] = 0;
	m[3][1] = t[Y];
	m[0][2] = 0;
	m[1][2] = 0;
	m[2][2] = 1;
	m[3][2] = t[Z];
	m[0][3] = 0;
	m[1][3] = 0;
	m[2][3] = 0;
	m[3][3] = 1;
}

Matrix4 Matrix4::RotationMatrix(Vector4 a, double angle) {
  Matrix4 newMatrix;
  newMatrix.rotateArbitrary(a, angle);
  return newMatrix;
}

Matrix4 Matrix4::RotationXMatrix(double angle) {
  Matrix4 newMatrix;
  newMatrix.rotateX(angle);
  return newMatrix;
}

Matrix4 Matrix4::RotationYMatrix(double angle) {
  Matrix4 newMatrix;
  newMatrix.rotateY(angle);
  return newMatrix;
}

Matrix4 Matrix4::RotationZMatrix(double angle) {
  Matrix4 newMatrix;
  newMatrix.rotateZ(angle);
  return newMatrix;
}

Matrix4 Matrix4::ScaleMatrix(double x, double y, double z) {
  Matrix4 newMatrix;
  newMatrix.scale(x, y, z);
  return newMatrix;
}

Matrix4 Matrix4::TranslationMatrix(Vector4 t) {
  Matrix4 newMatrix;
  newMatrix.translate(t);
  return newMatrix;
}

Matrix4 Matrix4::TranslationMatrix(double x, double y, double z) {
  return TranslationMatrix(Vector4::MakeVector(x, y, z, 1));
}

Matrix4 Matrix4::MakeMatrix(
    double m00, double m01, double m02, double m03,
    double m10, double m11, double m12, double m13,
    double m20, double m21, double m22, double m23,
    double m30, double m31, double m32, double m33) {
  Matrix4 newMatrix(
    m00, m01, m02, m03, 
    m10, m11, m12, m13,
    m20, m21, m22, m23,
    m30, m31, m32, m33);
  return newMatrix;
}

Matrix4 Matrix4::MakeMatrix(Vector4 a, Vector4 b, Vector4 c, Vector4 d) {
  Matrix4 newMatrix(
    a[X], b[X], c[X], d[X], 
    a[Y], b[Y], c[Y], d[Y],
    a[Z], b[Z], c[Z], d[Z],
    0, 0, 0, 0);
  return newMatrix;
}

Matrix4 Matrix4::MakeMatrix() {
  Matrix4 newMatrix;
  return newMatrix;
}

Matrix4 Matrix4::BezierMatrix() {
  return BEZIER_MATRIX;
}

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

void Matrix4::print() {
  std::cout << "Matrix:\n";
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      printf("%.2f   ", m[i][j]);
    }
    std::cout << std::endl;
  }
}
