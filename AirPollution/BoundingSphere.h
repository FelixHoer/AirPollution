#ifndef _BOUNDING_SPHERE_H_
#define _BOUNDING_SPHERE_H_

#include <glm/glm.hpp>

class BoundingSphere
{
protected:
  glm::vec3 center;
  float radius;

public:
  BoundingSphere();
  BoundingSphere(glm::vec3 center, float radius);

  virtual glm::vec3 getCenter() const;
  virtual float getRadius() const;

  virtual void render(const glm::mat4& matrix);

  virtual BoundingSphere BoundingSphere::transform(const glm::mat4& matrix);
};

#endif

