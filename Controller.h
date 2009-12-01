#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

class Controller {
public:
  virtual void initialize() {}
  virtual void step(double elapsed) {}
  virtual void draw() {}
  virtual void keyDownHandler(int key) {}
  virtual void keyUpHandler(int key) {}
};

#endif