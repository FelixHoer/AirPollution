#ifndef _DIRECTIONAL_LIGHT_H_
#define _DIRECTIONAL_LIGHT_H_

#include "Node.h"
#include <GL/glut.h>
#include <glm/glm.hpp>

class DirectionalLight : public Node
{
protected:
  GLuint light_number;

  virtual glm::mat4 calculateMatrixUp();

public:
  DirectionalLight(GLuint light);

  virtual void configureLight();
};

#endif

