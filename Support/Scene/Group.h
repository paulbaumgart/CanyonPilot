#ifndef _GROUP_H_
#define _GROUP_H_

#include "Node.h"

class Group : public Node {
  public:
    Group();
    Group(int);
    ~Group();
    void addChild(Node&);
    virtual void drawObject(Matrix4&);
    virtual void setCamera(Matrix4&);
  protected:
    void allocate(int);
    void reallocate(int);
    Node ** children;
    int maxChildren;
    int numChildren;
};

#endif