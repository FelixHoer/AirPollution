#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <glm/glm.hpp>
#include "Window.h"

#include "Group.h"

Group::Group()
{}

Group::Group(char* name) : Node(name)
{}

void Group::addChild(Node* child)
{
  child->setParent(this);
  children.push_back(child);
  recalculateBoundingSphere();
}

void Group::addChildFirst(Node* child)
{
  child->setParent(this);
  children.push_front(child);
  recalculateBoundingSphere();
}

void Group::removeChild(Node* child)
{
  child->setParent(NULL);
  children.remove(child);
  recalculateBoundingSphere();
}

Node* Group::findDown(const std::string& name)
{
  Node* result = NULL;

  result = Node::findDown(name);
  if (result != NULL)
    return result;

  for (std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it)
  {
    result = (*it)->findDown(name);
    if (result != NULL)
      return result;
  }

  return NULL;
}

void Group::setup()
{
  for (std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it)
    (*it)->setup();
  initializeBoundingSphere();
}

void Group::update(const int delta_time)
{
  for (std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it)
    (*it)->update(delta_time);
}

void Group::updateVisibility(const glm::mat4& matrix)
{
  BoundingSphere transformed_sphere = getBoundingSphere()->transform(matrix);
  Status status = Window::camera->getFrustum()->check(transformed_sphere);
  if (status == Status::OUTSIDE)
    setVisible(false);
  else if (status == Status::INSIDE)
    setVisible(true);
  else
  {
    visible = true;
    for (std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it)
      (*it)->updateVisibility(matrix);
  }
}

void Group::setVisible(bool v)
{
  visible = v;
  for (std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it)
    (*it)->setVisible(v);
}

void Group::render(const glm::mat4& matrix)
{
  if (!visible)
    return; // object was culled
  
  for (std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it)
    (*it)->render(matrix);

  if (Window::debug)
    bounding_sphere.render(matrix);
}

void Group::initializeBoundingSphere()
{
  glm::vec3 center = glm::vec3(0, 0, 0);
  int count = 0;

  // calculate center of mass of children
  for (std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it)
  {
    BoundingSphere* sphere = (*it)->getBoundingSphere();
    if (sphere == NULL)
      continue;

    center += sphere->getCenter();
    count++;
  }
  if (count == 0) // count might be 0
  {
    bounding_sphere = BoundingSphere();
    return;
  }
  center /= count; 

  // find max distance (center of child + radius) from that center
  float max_distance = 0.0f;
  for (std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it)
  {
    BoundingSphere* sphere = (*it)->getBoundingSphere();
    if (sphere == NULL)
      continue;

    float distance = glm::length(center - sphere->getCenter()) + sphere->getRadius();
    if (distance > max_distance)
      max_distance = distance;
  }

  bounding_sphere = BoundingSphere(center, max_distance);
}

BoundingSphere* Group::getBoundingSphere()
{
  return &bounding_sphere;
}