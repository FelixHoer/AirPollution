#ifndef _EGO_MOVEMENT_H_
#define _EGO_MOVEMENT_H_

#include <glm/glm.hpp>

#include "Node.h"
#include "InputHandler.h"

class EgoMovement : public Node, public InputHandler
{
protected:
  glm::ivec2 old_mouse_position;
  glm::ivec2 new_mouse_position;

  bool up_pressed;
  bool down_pressed;
  bool left_pressed;
  bool right_pressed;

  glm::vec3 position;
  glm::vec2 rotation;

  virtual void move(const int delta_time);
  virtual void rotate();

public:
  EgoMovement();

  virtual void setup();
  virtual void update(const int delta_time);

  virtual void specialKeyPressed(int key, int x, int y);
  virtual void specialKeyReleased(int key, int x, int y);
  virtual void mouseMove(int x, int y);
};

#endif

