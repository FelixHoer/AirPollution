#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BoundingSphere.h"

BoundingSphere::BoundingSphere() : center(glm::vec3(0,0,0)), radius(0.0f)
{}

BoundingSphere::BoundingSphere(glm::vec3 c, float r) :
  center(c), radius(r)
{}

glm::vec3 BoundingSphere::getCenter() const
{
  return center;
}

float BoundingSphere::getRadius() const 
{
  return radius;
}

void BoundingSphere::render(const glm::mat4& matrix)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(glm::value_ptr(matrix));

  glColor3f(1.0, 1.0, 0.0);

  glTranslatef(center.x, center.y, center.z);
  glutWireSphere(radius, 8, 8);
}

BoundingSphere BoundingSphere::transform(const glm::mat4& matrix)
{
  // extract the center
  glm::vec4 center4 = glm::vec4(center, 1);

  // get 3 corners of a fully enclosed box
  glm::vec4 axes[3];
  axes[0] = glm::vec4(radius, 0, 0, 1);
  axes[1] = glm::vec4(0, radius, 0, 1);
  axes[2] = glm::vec4(0, 0, radius, 1);

  // transform center and the 3 corner points
  center4 = matrix * center4;
  for (int i = 0; i < 3; i++)
    axes[i] = matrix * axes[i];

  // find max distance from center to a corner point
  float max_distance = 0.0f;
  for (int i = 0; i < 3; i++)
  {
    float distance = glm::length(center4 - axes[i]);
    if (distance > max_distance)
      max_distance = distance;;
  }

  // build sphere from center and that max distance as radius
  return BoundingSphere(glm::vec3(center4), max_distance);
}