#ifndef _NODE_H_
#define _NODE_H_

#include <glm/glm.hpp>
#include <string>
#include <typeinfo>
#include "BoundingSphere.h"

class Node
{
protected:
  std::string name;
  Node* parent;
  bool visible;

public:
  Node();
  Node(char* name);

  virtual void setup();
  virtual void update(const int delta_time); // in ms
  virtual void updateVisibility(const glm::mat4& matrix);
  virtual void setVisible(bool visible);
  virtual void render(const glm::mat4& matrix);
  virtual void destroy();

  virtual Node* findDown(const std::string& name);
  virtual Node* findUp(const std::string& name);
  virtual Node* findUp(const std::type_info& type);

  virtual void initializeBoundingSphere();
  virtual void recalculateBoundingSphere();
  virtual BoundingSphere* getBoundingSphere();

  virtual std::string getName();
  virtual Node* getParent();
  virtual void setParent(Node* parent);
};

#endif

