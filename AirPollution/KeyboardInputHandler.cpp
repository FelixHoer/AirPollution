#include <stdio.h>
#include <stdlib.h>
#include "Window.h"

#include "KeyboardInputHandler.h"

void KeyboardInputHandler::keyPressed(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 100: // d
    Window::debug = !Window::debug; break;
  case 49: // 1
    Window::active_measurement = 0; break;
  case 50: // 2
    Window::active_measurement = 1; break;
  case 51: // 3
    Window::active_measurement = 2; break;
  case 52: // 0
    Window::active_measurement = 3; break;
  case 27: // escape
    exit(0); break;
  }
}