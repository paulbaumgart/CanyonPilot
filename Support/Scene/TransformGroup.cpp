#include "TransformGroup.h"
#include <stdlib.h>

TransformGroup::TransformGroup() {
  allocate(1);
}

TransformGroup::TransformGroup(Matrix4 mat) {
  this->transform = mat;
  allocate(2);
}

TransformGroup::TransformGroup(Matrix4 mat, int childCountHint) {
  this->transform = mat;
  allocate(childCountHint);
}

Matrix4& TransformGroup::getMatrix() {
  return transform;
}

void TransformGroup::drawObject(Matrix4& mat) {
  Matrix4 newMatrix = mat.multiply(this->transform);

  for (int i = 0; i < numChildren; i++) {
    children[i]->draw(newMatrix);
  }
}

void TransformGroup::setCamera(Matrix4& mat) {
  Matrix4 newMatrix = mat.multiply(this->transform);

  for (int i = 0; i < numChildren; i++) {
    children[i]->setCamera(newMatrix);
  }
}