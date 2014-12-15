#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "InputManager.h"
#include "MatrixTransform.h"
#include "Window.h"
#include "Bezier.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

#include "BezierMovement.h"

BezierMovement::BezierMovement(std::vector<Location> l) 
: Node("BezierMovement")
{
  for (std::vector<Location>::iterator it = l.begin(); it != l.end(); ++it)
    locations.push_back(it->position);

  unsigned int missing_points = 3 - (l.size() % 3 - 1);
  for (unsigned int i = 0; i < missing_points; i++)
    locations.push_back(l[l.size() - 1].position);
}

void BezierMovement::setup()
{
  active = true;
  activation_time = Window::time_since_start;
}

void BezierMovement::update(const int delta_time)
{
  if (!active)
    return;

  unsigned int runtime = 60000;
  float t = ((Window::time_since_start - activation_time) % runtime) / float(runtime);

  glm::dvec2 position = Bezier::curve(locations, t);
  glm::dvec2 tangent = Bezier::tangent(locations, t);

  glm::vec2 position_xy = Window::map->getPosition(position);
  float angle = 0;
  //float angle = acos(glm::dot(glm::vec2(0, -1), glm::normalize(glm::vec2(tangent))));
  //angle += float(M_PI) / 2.0f;

  std::cout << t << " " << glm::to_string(position_xy) << std::endl;

  MatrixTransform* transform = (MatrixTransform*)parent;
  glm::mat4 parent_matrix = transform->getMatrix();

  glm::mat4 matrix = glm::translate(glm::mat4(), glm::vec3(position_xy.x, 0, position_xy.y))
                   * glm::rotate(glm::mat4(), angle, glm::vec3(0,1,0));

  transform->setMatrix(matrix);
}

void BezierMovement::destroy()
{
  active = false;
}
