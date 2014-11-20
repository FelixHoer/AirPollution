#ifndef _CUBE_H_
#define _CUBE_H_

#include "Geode.h"

class Cube : public Geode
{
protected:
  BoundingSphere bounding_sphere;

  virtual void renderCube(const glm::mat4& matrix);

public:
  Cube();
  Cube(char* name);

  virtual void render(const glm::mat4& matrix, const RenderType type);

  virtual void initializeBoundingSphere();
  virtual BoundingSphere* getBoundingSphere();
};

#endif

