#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "MatrixTransform.h"

#include "Camera.h"

Camera::Camera() : Node("Camera")
{}

glm::mat4 Camera::calculateMatrixUp()
{
  glm::mat4 matrix;

  Node* node = this;
  while (node != NULL)
  {
    if (typeid(*node) == typeid(MatrixTransform))
    {
      MatrixTransform* transform = (MatrixTransform*)node;
      matrix = transform->getMatrix() * matrix;
    }
    node = node->getParent();
  }

  return matrix;
}

void Camera::configureCamera()
{
  glm::mat4 matrix = calculateMatrixUp();

  float left = -1.0, right = 1.0, bottom = -1.0, top = 1.0, near = 1.0, far = 1000;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(left, right, bottom, top, near, far); // set perspective projection viewing frustum

  frustum.initialize(left, right, bottom, top, near, far);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glm::vec4 eye = matrix * glm::vec4(0, 0, 0, 1);
  glm::vec4 target = matrix * glm::vec4(0, 0, -1, 1);
  glm::vec4 up = matrix * glm::vec4(0, 1, 0, 1) - eye;
  gluLookAt(eye.x, eye.y, eye.z, target.x, target.y, target.z, up.x, up.y, up.z);

  glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(Camera::matrix));
}

glm::mat4 Camera::getMatrix()
{
  return matrix;
}

Frustum* Camera::getFrustum()
{
  return &frustum;
}