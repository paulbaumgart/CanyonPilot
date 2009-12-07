#include "Group.h"
#include <stdlib.h>
#include <stdio.h>

Group::Group() {
  allocate(2);
}

Group::Group(int childCountHint) {
  allocate(childCountHint);
}

Group::~Group() {
  free(this->children);
}

void Group::allocate(int count) {
  this->maxChildren = count;
  this->numChildren = 0;
  this->children = (Node **) malloc(count * sizeof(Node*));
}

void Group::reallocate(int count) {
  this->maxChildren = count;
  this->children = (Node **) realloc(this->children, count * sizeof(Node*));
}

void Group::addChild(Node& child) {
  if (numChildren == maxChildren) {
    reallocate(++maxChildren);
  }
  
  this->children[numChildren++] = &child;
}

void Group::drawObject(Matrix4& mat) {
  glPushMatrix();
  for (int i = 0; i < numChildren; i++) {
    children[i]->draw(mat);
  }
  glPopMatrix();
}

void Group::setCamera(Matrix4& mat) {
  for (int i = 0; i < numChildren; i++) {
    children[i]->setCamera(mat);
  }
}