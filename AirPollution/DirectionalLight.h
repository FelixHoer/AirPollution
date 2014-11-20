#ifndef _DIRECTIONAL_LIGHT_H_
#define _POINT_LIG_DIRECTIONAL_LIGHT_H_HT_H_

#include "Node.h"

class DirectionalLight : public Node
{
protected:
  GLuint light_number;

  virtual void setupLight(const glm::mat4& matrix);

public:
  DirectionalLight(GLuint light);

  virtual void render(const glm::mat4& matrix, const RenderType type);
};

#endif

