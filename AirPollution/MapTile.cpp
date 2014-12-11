#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Window.h"
#include "ObjectCounter.h"
#include "TextureReader.h"

#include "MapTile.h"

MapTile::MapTile(std::string n)
{
  name = n;
	ObjectCounter::registerObject();
}

void MapTile::setup()
{
  Geode::setup();
  texture = TextureReader::loadTexture(name);
}

void MapTile::render(const glm::mat4& matrix)
{
  if (!visible)
    return; // object was culled

  if (Window::debug)
    bounding_sphere.render(matrix);

	ObjectCounter::count();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(matrix));

  TextureReader::configureTexture(GL_TEXTURE0, texture);

	// Draw sides of cube in object coordinate system:
	glBegin(GL_QUADS);

	// drawing color white to see original texture colors
  glColor3f(1.0f, 1.0f, 1.0f);

  glNormal3f(0.0f, 1.0f, 0.0f);

	glTexCoord2f(0, 0); 
  glVertex3f(-0.5f, 0.0f, -0.5f);

  glTexCoord2f(1, 0); 
  glVertex3f( 0.5f, 0.0f, -0.5f);

  glTexCoord2f(1, 1); 
  glVertex3f( 0.5f, 0.0f,  0.5f);

  glTexCoord2f(0, 1); 
  glVertex3f(-0.5f, 0.0f,  0.5f);

	glEnd();
}

void MapTile::initializeBoundingSphere()
{
	glm::vec3 center(0, 0, 0);
  float radius = 0.77f;
	bounding_sphere = BoundingSphere(center, radius);
}

BoundingSphere* MapTile::getBoundingSphere()
{
	return &bounding_sphere;
}
