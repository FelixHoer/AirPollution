#ifndef _MATRIX_TRANSFORM_H_
#define _MATRIX_TRANSFORM_H_

#include "Group.h"

class MatrixTransform : public Group
{
protected:
  glm::mat4 matrix;
  BoundingSphere transformed_sphere;

public:
  MatrixTransform();
  MatrixTransform(char* name);

  virtual void setMatrix(const glm::mat4& matrix);
  virtual glm::mat4 getMatrix();

  virtual void render(const glm::mat4& parent_matrix);

  virtual void initializeBoundingSphere();
  virtual BoundingSphere* getBoundingSphere();
};

#endif

