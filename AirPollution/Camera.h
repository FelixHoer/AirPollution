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

public:
  Camera();

  virtual void render(const glm::mat4& matrix, const RenderType type);

  virtual glm::mat4 getMatrix();
  virtual Frustum* getFrustum();
};

#endif

