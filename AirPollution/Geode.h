#ifndef _GEODE_H_
#define _GEODE_H_

#include <glm/glm.hpp>
#include <string>

#include "Node.h"

class Geode : public Node
{
public:
  Geode();
  Geode(char* name);
};

#endif

