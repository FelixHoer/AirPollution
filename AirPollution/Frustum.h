#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include <glm/glm.hpp>
#include "BoundingSphere.h"

enum Status {INSIDE, PARTIAL, OUTSIDE};

class Frustum
{
protected:
  glm::vec4 planes[6];

public:
  Frustum();

  virtual void initialize(float left,
                           float right,
                           float bottom,
                           float top,
                           float near,
                           float far);

  virtual Status check(const BoundingSphere& sphere);
  virtual float distanceToPlane(const glm::vec4& plane, const glm::vec3& point);

};

#endif

