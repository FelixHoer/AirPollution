#include <stdio.h>
#include <stdlib.h>
#include "Window.h"

#include "KeyboardInputHandler.h"

void KeyboardInputHandler::keyPressed(unsigned char key, int x, int y)
{
  if (key == 100) // d
    Window::debug = !Window::debug;
}