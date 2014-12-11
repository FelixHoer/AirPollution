#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>

#include "Node.h"

Node::Node()
{}

Node::Node(char* n) : name(n), parent(NULL)
{}

void Node::setup()
{
  initializeBoundingSphere();
}

void Node::update(const int delta_time)
{}

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