#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "InputManager.h"
#include "MatrixTransform.h"
#include "Window.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

#include "EgoMovement.h"

EgoMovement::EgoMovement() : Node("EgoMovement")
{
  old_mouse_position = glm::ivec2(0, 0);
  new_mouse_position = glm::ivec2(0, 0);

  up_pressed = false;
  down_pressed = false;
  left_pressed = false;
  right_pressed = false;
}

void EgoMovement::setup()
{
  glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
  InputManager::attatchInputHandler(this);
  rotation = glm::vec2(0, 0);
  position = glm::vec3(0, 0, 0);
}

void EgoMovement::specialKeyPressed(int key, int x, int y)
{
  std::cout << "pressed: " << key << std::endl;
  switch (key)
  {
  case GLUT_KEY_UP:
    up_pressed = true;
    break;
  case GLUT_KEY_DOWN:
    down_pressed = true;
    break;
  case GLUT_KEY_LEFT:
    left_pressed = true;
    break;
  case GLUT_KEY_RIGHT:
    right_pressed = true;
    break;
  }
}

void EgoMovement::specialKeyReleased(int key, int x, int y)
{
  std::cout << "released: " << key << std::endl;
  switch (key)
  {
  case GLUT_KEY_UP:
    up_pressed = false;
    break;
  case GLUT_KEY_DOWN:
    down_pressed = false;
    break;
  case GLUT_KEY_LEFT:
    left_pressed = false;
    break;
  case GLUT_KEY_RIGHT:
    right_pressed = false;
    break;
  }
}

void EgoMovement::mouseMove(int x, int y)
{
  new_mouse_position = glm::ivec2(x, y);
}

void EgoMovement::update(const int delta_time)
{
  move(delta_time);
  rotate();

  MatrixTransform* transform = (MatrixTransform*)parent;
  glm::mat4 parent_matrix = transform->getMatrix();

  glm::mat4 delta_matrix = glm::translate(glm::mat4(), position)
                         * glm::rotate(glm::mat4(), rotation.y, glm::vec3(0, 1, 0))
                         * glm::rotate(glm::mat4(), rotation.x, glm::vec3(1, 0, 0));

  //std::cout << "position: " << glm::to_string(position) << std::endl;
  //std::cout << "rotation: " << glm::to_string(rotation) << std::endl;

  transform->setMatrix(delta_matrix);
}

void EgoMovement::move(const int delta_time)
{
  float distance_per_second = 10.0f;
  float distance = distance_per_second * delta_time / 1000.f;

  glm::mat4 rotation_matrix = glm::rotate(glm::mat4(), rotation.y, glm::vec3(0, 1, 0))
                            * glm::rotate(glm::mat4(), rotation.x, glm::vec3(1, 0, 0));

  glm::vec3 up = glm::normalize(glm::vec3(rotation_matrix * glm::vec4(0, 1, 0, 1)));
  glm::vec3 forward = glm::normalize(glm::vec3(rotation_matrix * glm::vec4(0, 0, -1, 1)));
  glm::vec3 left = glm::normalize(glm::cross(up, forward));
  
  if (up_pressed)
    position += distance * forward;
  if (down_pressed)
    position -= distance * forward;
  if (left_pressed)
    position += distance * left;
  if (right_pressed)
    position -= distance * left;
}

void EgoMovement::rotate()
{
  float angle_for_width  = 2 * M_PI;
  float angle_for_height = 2 * M_PI;

  glm::ivec2 delta_position = new_mouse_position - old_mouse_position;
  old_mouse_position = new_mouse_position;

  float angle_x = angle_for_height * delta_position.y / Window::height;
  float angle_y = angle_for_width  * delta_position.x / Window::width;

  rotation.x += -angle_x;
  rotation.y += -angle_y;

  if (rotation.x >= M_PI / 2.0f)
    rotation.x = M_PI / 2.0f;
  if (rotation.x <= -M_PI / 2.0f)
    rotation.x = -M_PI / 2.0f;
}