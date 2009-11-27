#ifndef _TGROUP_H_
#define _TGROUP_H_

#include "Group.h"
#include "../Matrix4.h"

class TransformGroup : public Group {
  public:
    TransformGroup();
    TransformGroup(Matrix4);
    TransformGroup(Matrix4, int);
    virtual void drawObject(Matrix4&);
    virtual void setCamera(Matrix4&);
    Matrix4& getMatrix();
  private:
    Matrix4 transform;
};

#endif