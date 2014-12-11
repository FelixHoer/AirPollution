#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>

#include "Node.h"
#include "Window.h"

Node::Node()
{}

Node::Node(char* n) : name(n), parent(NULL), visible(false)
{}

void Node::setup()
{
  initializeBoundingSphere();
}

void Node::update(const int delta_time)
{}

void Node::updateVisibility(const glm::mat4& matrix)
{
  BoundingSphere* sphere = getBoundingSphere();
  if (sphere == NULL) return;
  BoundingSphere transformed_sphere = sphere->transform(matrix);
  Status status = Window::camera->getFrustum()->check(transformed_sphere);
  if (status == Status::OUTSIDE)
    setVisible(false);
  else
    setVisible(true);
}

void Node::setVisible(bool v)
{
  visible = v;
}

void Node::render(const glm::mat4& matrix)
{}

void Node::destroy()
{}

Node* Node::find(const std::string& n)
{
  if (name.compare(n) == 0)
    return this;
  else
    return NULL;
}

void Node::initializeBoundingSphere()
{}

void Node::recalculateBoundingSphere()
{
  initializeBoundingSphere();
  if (parent != NULL)
    parent->recalculateBoundingSphere();
}

BoundingSphere* Node::getBoundingSphere()
{
  // default implementation
  // should only be used for Nodes that do not care about bounding spheres
  // and should be rendered either way, like lights, the camera, ...
  return NULL;
}

std::string Node::getName()
{
  return name;
}

Node* Node::getParent()
{
  return parent;
}

void Node::setParent(Node* p)
{
  parent = p;
}