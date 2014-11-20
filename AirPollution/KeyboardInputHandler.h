#ifndef _KEYBOARD_INPUT_HANDLER_H_
#define _KEYBOARD_INPUT_HANDLER_H_

#include "InputHandler.h"

class KeyboardInputHandler : public InputHandler
{
public:
  virtual void keyPressed(unsigned char key, int x, int y);
};

#endif

