#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(GLuint light) : Node("PointLight")
{
  light_number = light;
}

void DirectionalLight::setupLight(const glm::mat4& matrix)
{
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

void DirectionalLight::render(const glm::mat4& matrix, const RenderType type)
{
  if (type == RenderType::LIGHT)
    setupLight(matrix);
}