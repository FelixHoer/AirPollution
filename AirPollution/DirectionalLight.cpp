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

#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(GLuint light) : Node("PointLight")
{
  light_number = light;
}

glm::mat4 DirectionalLight::calculateMatrixUp()
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

void DirectionalLight::configureLight()
{
  glm::mat4 matrix = calculateMatrixUp();

  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(glm::value_ptr(matrix));

  // position defined by matrix, w=1 defines a positional light
  float origin_positional[] = { 0.0, 1.0, 0.0, 0.0 };
  glLightfv(light_number, GL_POSITION, origin_positional);

  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  glEnable(GL_LIGHTING);
  glEnable(light_number);
}