#include <stdio.h>
#include <stdlib.h>
#include "Window.h"

#include "KeyboardInputHandler.h"

void KeyboardInputHandler::keyPressed(unsigned char key, int x, int y)
{
  if (key == 99) // c
    Window::enable_culling = !Window::enable_culling;
  else if (key == 115) // s
    Window::debug = !Window::debug;
}