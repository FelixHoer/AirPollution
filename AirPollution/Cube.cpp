#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Window.h"
#include "ObjectCounter.h"

#include "Cube.h"

Cube::Cube() : Cube("Cube")
{}

Cube::Cube(char* n) : Geode(n)
{
  ObjectCounter::registerObject();
}

void Cube::render(const glm::mat4& matrix)
{
  if (Window::enable_culling &&
    !Window::camera->getFrustum()->isInside(bounding_sphere.transform(matrix)))
    return; // object was culled 
  ObjectCounter::count();

  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(glm::value_ptr(matrix));

  // Draw sides of cube in object coordinate system:
  glBegin(GL_QUADS);

  glColor3f(1.0, 1.0, 1.0);

  // Draw front face:
  glNormal3f(0.0, 0.0, 1.0);
  glVertex3f(-0.5, 0.5, 0.5);
  glVertex3f(0.5, 0.5, 0.5);
  glVertex3f(0.5, -0.5, 0.5);
  glVertex3f(-0.5, -0.5, 0.5);

  // Draw left side:
  glNormal3f(-1.0, 0.0, 0.0);
  glVertex3f(-0.5, 0.5, 0.5);
  glVertex3f(-0.5, 0.5, -0.5);
  glVertex3f(-0.5, -0.5, -0.5);
  glVertex3f(-0.5, -0.5, 0.5);

  // Draw right side:
  glNormal3f(1.0, 0.0, 0.0);
  glVertex3f(0.5, 0.5, 0.5);
  glVertex3f(0.5, 0.5, -0.5);
  glVertex3f(0.5, -0.5, -0.5);
  glVertex3f(0.5, -0.5, 0.5);

  // Draw back face:
  glNormal3f(0.0, 0.0, -1.0);
  glVertex3f(-0.5, 0.5, -0.5);
  glVertex3f(0.5, 0.5, -0.5);
  glVertex3f(0.5, -0.5, -0.5);
  glVertex3f(-0.5, -0.5, -0.5);

  // Draw top side:
  glNormal3f(0.0, 1.0, 0.0);
  glVertex3f(-0.5, 0.5, 0.5);
  glVertex3f(0.5, 0.5, 0.5);
  glVertex3f(0.5, 0.5, -0.5);
  glVertex3f(-0.5, 0.5, -0.5);

  // Draw bottom side:
  glNormal3f(0.0, -1.0, 0.0);
  glVertex3f(-0.5, -0.5, -0.5);
  glVertex3f(0.5, -0.5, -0.5);
  glVertex3f(0.5, -0.5, 0.5);
  glVertex3f(-0.5, -0.5, 0.5);

  glEnd();
}

void Cube::initializeBoundingSphere()
{
  glm::vec3 center(0, 0, 0);
  float radius = glm::length(center - glm::vec3(0.77, 0.77, 0.77));
  bounding_sphere = BoundingSphere(center, radius);
}

BoundingSphere* Cube::getBoundingSphere()
{
  return &bounding_sphere;
}