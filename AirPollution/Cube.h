#ifndef _CUBE_H_
#define _CUBE_H_

#include "Geode.h"

class Cube : public Geode
{
protected:
  BoundingSphere bounding_sphere;

public:
  Cube();
  Cube(char* name);

  virtual void render(const glm::mat4& matrix);

  virtual void initializeBoundingSphere();
  virtual BoundingSphere* getBoundingSphere();
};

#endif

