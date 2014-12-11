#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>
#include "Frustum.h"

#include "Node.h"

class Camera : public Node
{
protected:
  glm::mat4 matrix;
  Frustum frustum;

  virtual glm::mat4 calculateMatrixUp();

public:
  Camera();

  virtual void configureCamera();

  virtual glm::mat4 getMatrix();
  virtual Frustum* getFrustum();
};

#endif

