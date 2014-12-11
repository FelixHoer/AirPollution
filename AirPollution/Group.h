#ifndef _GROUP_H_
#define _GROUP_H_

#include <glm/glm.hpp>
#include <string>
#include <list>

#include "Node.h"

class Group : public Node
{
protected:
  std::list<Node*> children;
  BoundingSphere bounding_sphere;

public:
  Group();
  Group(char* name);

  virtual void addChild(Node* child);
  virtual void addChildFirst(Node* child);
  virtual void removeChild(Node* child);
  virtual Node* find(const std::string& name);

  virtual void setup();
  virtual void update(const int delta_time); // in ms
  virtual void render(const glm::mat4& matrix);

  virtual void initializeBoundingSphere();
  virtual BoundingSphere* getBoundingSphere();
};

#endif

