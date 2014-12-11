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

void generateLevel(MatrixTransform* parent, 
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
  unsigned int x_min = 11434;
  unsigned int y_min = 26458;

  generateLevel(this, x_min, y_min, 32);
  MatrixTransform::setup();
}

/*
void Map::setup()
{
  unsigned int x_min = 11434;
  unsigned int x_max = x_min + 32;
  unsigned int y_min = 26458;
  unsigned int y_max = y_min + 32;

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
*/