#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>

#include "MatrixTransform.h"

MatrixTransform::MatrixTransform() : Group("MatrixTransform")
{}

MatrixTransform::MatrixTransform(char* name) : Group(name)
{}

void MatrixTransform::setMatrix(const glm::mat4& new_matrix){
  matrix = new_matrix;
  recalculateBoundingSphere();
}

glm::mat4 MatrixTransform::getMatrix()
{
  return matrix;
}

void MatrixTransform::updateVisibility(const glm::mat4& parent_matrix)
{
  Group::updateVisibility(parent_matrix * matrix);
}

void MatrixTransform::render(const glm::mat4& parent_matrix)
{
  Group::render(parent_matrix * matrix);
}

void MatrixTransform::initializeBoundingSphere()
{
  Group::initializeBoundingSphere();
  transformed_sphere = bounding_sphere.transform(matrix);
}

BoundingSphere* MatrixTransform::getBoundingSphere()
{
  return &transformed_sphere;
}