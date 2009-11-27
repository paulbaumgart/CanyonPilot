#ifndef _VECTOR4_H_
#define _VECTOR4_H_

#define X 0
#define Y 1
#define Z 2
#define W 3

class Vector4
{
  protected:
    double v[4];   // vector elements
    
  public:
    Vector4();        // ructor
    Vector4(double, double, double, double);
    double* getPointer();  // return pointer to vector elements
    void setElement(int, double);
    double getElement(int);
    double operator[](int);
    Vector4 add(Vector4 &);
    Vector4 operator+(Vector4 &);
    Vector4 subtract(Vector4 &);
    Vector4 operator-(Vector4 &);
    Vector4 scale(double);
    double dot(Vector4 &);
    Vector4 cross(Vector4);
    void dehomogenize();
    
    void print();
    
    static Vector4 MakeVector(double, double, double, double);
};

#endif