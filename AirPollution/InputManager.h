#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include <vector>
#include "InputHandler.h"

class InputManager
{
protected:
  static std::vector<InputHandler*> input_handlers;

  static void keyPressed(unsigned char key, int x, int y);
  static void keyReleased(unsigned char key, int x, int y);
  static void specialKeyPressed(int key, int x, int y);
  static void specialKeyReleased(int key, int x, int y);
  static void mouseButtonChanged(int button, int state, int x, int y);
  static void mouseDrag(int x, int y);
  static void mouseMove(int x, int y);

public:
  static void attatchInputHandler(InputHandler* handler);
};

#endif

