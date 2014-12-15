#ifndef _BEZIER_MOVEMENT_H_
#define _BEZIER_MOVEMENT_H_

#include <glm/glm.hpp>
#include <vector>

#include "Node.h"
#include "DataReader.h"

class BezierMovement : public Node
{
protected:
  bool active;
  int activation_time;
  std::vector<glm::dvec2> locations;

public:
  BezierMovement(std::vector<Location> locations);

  virtual void setup();
  virtual void update(const int delta_time);
  virtual void destroy();
};

#endif

