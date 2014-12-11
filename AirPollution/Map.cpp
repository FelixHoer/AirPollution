#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MapTile.h"

#include "Map.h"

Map::Map() : Map("Terrain")
{}

Map::Map(char* n) : MatrixTransform(n)
{}

void Map::setup()
{
  unsigned int x_min = 11434;
  unsigned int x_max = x_min+32;
  unsigned int y_min = 26458;
  unsigned int y_max = y_min+32;

  // add map tiles from ranges
  for (unsigned int x = x_min; x < x_max; x++)
  {
    for (unsigned int y = y_min; y < y_max; y++)
    {
      std::stringstream file;
      file << "..\\Texture\\Map\\tile-" << x << "-" << y << ".png.ppm";

      MatrixTransform* position = new MatrixTransform();
      position->setMatrix(glm::translate(glm::mat4(), glm::vec3(x - x_min, 0.0f, y - y_min)));
      position->addChild(new MapTile(file.str()));

      addChild(position);
    }
  }

  MatrixTransform::setup();
}