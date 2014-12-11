#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>

#include "Frustum.h"

Frustum::Frustum()
{}

void Frustum::initialize(float left,
                          float right,
                          float bottom,
                          float top,
                          float near,
                          float far)
{
  // build planes

  // basic vectors
  glm::vec3 p(0, 0, 0);
  glm::vec3 forward(0, 0, -1);
  glm::vec3 up(0, 1, 0);
  glm::vec3 side = glm::cross(up, forward);

  // helper variables
  glm::vec3 n;
  float d;
  int i = 0;

  // front
  n = forward;
  d = -near;
  planes[i++] = glm::vec4(n, d);

  // back
  n = -forward;
  d = far;
  planes[i++] = glm::vec4(n, d);

  // right
  n = glm::normalize(glm::cross(up, glm::vec3(right, 0, -near)));
  d = 0;
  planes[i++] = glm::vec4(n, d);

  // left
  n = glm::normalize(glm::cross(-up, glm::vec3(left, 0, -near)));
  d = 0;
  planes[i++] = glm::vec4(n, d);

  // up
  n = glm::normalize(glm::cross(side, glm::vec3(0, top, -near)));
  d = 0;
  planes[i++] = glm::vec4(n, d);

  // down
  n = glm::normalize(glm::cross(-side, glm::vec3(0, bottom, -near)));
  d = 0;
  planes[i++] = glm::vec4(n, d);
}

bool Frustum::isInside(const BoundingSphere& sphere)
{
  for (int i = 0; i < 6; i++)
  {
    float dist = distanceToPlane(planes[i], sphere.getCenter());
    if (dist < -sphere.getRadius())
      return false; // has to be completely outside
    if (dist < sphere.getRadius())
    {
      // sphere is partially outside - check children
    }
  }
  return true;
}

float Frustum::distanceToPlane(const glm::vec4& plane, const glm::vec3& point)
{
  // TODO center dot normal + d
  // center = point (plus or minus)
  // normal = plane.xyz
  // d = plane[3]
  float a = glm::dot(point, glm::vec3(plane));
  float b = plane[3];
  return a + b;
}