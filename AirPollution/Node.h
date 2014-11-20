#ifndef _NODE_H_
#define _NODE_H_

#include <glm/glm.hpp>
#include <string>
#include "BoundingSphere.h"

enum RenderType {CAMERA, LIGHT, OBJECT, DEBUG};

class Node
{
protected:
  Node* parent;
  std::string name;

public:
  Node();
  Node(char* name);

  virtual void setup();
  virtual void update(const int delta_time); // in ms
  virtual void render(const glm::mat4& matrix, const RenderType type);
  virtual void render(const glm::mat4& matrix);
  virtual void destroy();

  virtual Node* find(const std::string& name);

  virtual void initializeBoundingSphere();
  virtual void recalculateBoundingSphere();
  virtual BoundingSphere* getBoundingSphere();

  virtual std::string getName();
  virtual Node* getParent();
  virtual void setParent(Node* parent);
};

#endif

