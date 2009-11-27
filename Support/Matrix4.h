#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#import "Vector4.h"
#import "Vector3.h"

class Matrix4
{
  protected:
    double m[4][4];   // matrix elements
    
  public:
    Matrix4();        // constructor
    Matrix4(double, double, double, double, 
            double, double, double, double, 
            double, double, double, double, 
            double, double, double, double);
    Matrix4(double* [4][4]);
    double* getPointer();  // return pointer to matrix elements
    void setElement(int, double);
    void setElement(int, int, double);
    double getElement(int);
    double getElement(int, int);
    double operator[](int);
    Vector4 multiply(Vector4);
    Vector3 multiply(Vector3);
    Matrix4 multiply(Matrix4);
    void identity();  // create identity matrix
    void rotateX(double); // rotation about x axis
    void rotateY(double); // rotation about y axis
    void rotateZ(double); // rotation about z axis
    void rotateArbitrary(Vector4, double);
    void scale(double, double, double);
    void translate(Vector4);
    void print();
    
    static Matrix4 MakeMatrix(double, double, double, double, 
              double, double, double, double, 
              double, double, double, double, 
              double, double, double, double);
    static Matrix4 MakeMatrix();
    static Matrix4 MakeMatrix(Vector4, Vector4, Vector4, Vector4);
    static Matrix4 RotationMatrix(Vector4, double);        
    static Matrix4 RotationXMatrix(double);  
    static Matrix4 RotationYMatrix(double);  
    static Matrix4 RotationZMatrix(double);
    static Matrix4 ScaleMatrix(double, double, double);
    static Matrix4 TranslationMatrix(Vector4);
    static Matrix4 TranslationMatrix(double, double, double);
    static Matrix4 BezierMatrix();
};

#endif