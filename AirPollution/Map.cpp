#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MapTile.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "Map.h"

Map::Map(unsigned int x_min, unsigned int y_min, unsigned int size, unsigned int zoom) 
: Map("Terrain", x_min, y_min, size, zoom)
{}

Map::Map(char* n, unsigned int x, unsigned int y, unsigned int s, unsigned int z)
: MatrixTransform(n), x_min(x), y_min(y), size(s), zoom(z)
{}

void Map::generateLevel(MatrixTransform* parent,
  unsigned int x, 
  unsigned int y, 
  unsigned int size)
{
  if (size == 1)
  {
    std::stringstream file;
    file << "..\\Texture\\Map\\tile-" << x << "-" << y << ".png.ppm";
    parent->addChild(new MapTile(file.str()));
  }
  else
  {
    int new_size = size / 2;

    MatrixTransform* m11 = new MatrixTransform();
    m11->setMatrix(glm::translate(glm::mat4(), glm::vec3(-new_size / 2.0f, 0.0f, -new_size / 2.0f)));
    generateLevel(m11, x, y, new_size);
    parent->addChild(m11);

    MatrixTransform* m12 = new MatrixTransform();
    m12->setMatrix(glm::translate(glm::mat4(), glm::vec3(new_size / 2.0f, 0.0f, -new_size / 2.0f)));
    generateLevel(m12, x + new_size, y, new_size);
    parent->addChild(m12);

    MatrixTransform* m21 = new MatrixTransform();
    m21->setMatrix(glm::translate(glm::mat4(), glm::vec3(-new_size / 2.0f, 0.0f, new_size / 2.0f)));
    generateLevel(m21, x, y + new_size, new_size);
    parent->addChild(m21);

    MatrixTransform* m22 = new MatrixTransform();
    m22->setMatrix(glm::translate(glm::mat4(), glm::vec3(new_size / 2.0f, 0.0f, new_size / 2.0f)));
    generateLevel(m22, x + new_size, y + new_size, new_size);
    parent->addChild(m22);
  }
}

void Map::setup()
{
  generateLevel(this, x_min, y_min, size);
  MatrixTransform::setup();
}

glm::vec2 Map::getPosition(glm::dvec2 latlon)
{
  double lat = latlon[0];
  double lon = latlon[1];

  // conversion code taken from: http://wiki.openstreetmap.org/wiki/Slippy_map_tilenames
  float x = (lon + 180.0) / 360.0 * pow(2.0, zoom);
  float y = (1.0 - log(tan(lat * M_PI / 180.0) + 1.0 / cos(lat * M_PI / 180.0)) / M_PI) / 2.0 * pow(2.0, zoom);
  
  return glm::vec2(x - x_min, y - y_min);
}