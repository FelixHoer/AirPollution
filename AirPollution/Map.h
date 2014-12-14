#ifndef _MAP_H_
#define _MAP_H_

#include <glm/glm.hpp>
#include "MatrixTransform.h"

class Map : public MatrixTransform
{
protected:
  unsigned int x_min;
  unsigned int y_min;
  unsigned int size;
  unsigned int zoom;

  void generateLevel(MatrixTransform* parent,
    unsigned int x,
    unsigned int y,
    unsigned int size);

public:
  Map(unsigned int x_min, unsigned int y_min, unsigned int size, unsigned int zoom);
  Map(char* name, unsigned int x_min, unsigned int y_min, unsigned int size, unsigned int zoom);

  virtual void setup();

  glm::vec2 getPosition(glm::dvec2 latlon);
};

#endif
