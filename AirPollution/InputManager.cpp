#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>

#include "InputManager.h"

std::vector<InputHandler*> InputManager::input_handlers;

void InputManager::keyPressed(unsigned char key, int x, int y)
{
  std::vector<InputHandler*>::iterator it;
  for (it = input_handlers.begin(); it != input_handlers.end(); ++it)
    (*it)->keyPressed(key, x, y);
}

void InputManager::keyReleased(unsigned char key, int x, int y)
{
  std::vector<InputHandler*>::iterator it;
  for (it = input_handlers.begin(); it != input_handlers.end(); ++it)
    (*it)->keyReleased(key, x, y);
}

void InputManager::specialKeyPressed(int key, int x, int y)
{
  std::vector<InputHandler*>::iterator it;
  for (it = input_handlers.begin(); it != input_handlers.end(); ++it)
    (*it)->specialKeyPressed(key, x, y);
}

void InputManager::specialKeyReleased(int key, int x, int y)
{
  std::vector<InputHandler*>::iterator it;
  for (it = input_handlers.begin(); it != input_handlers.end(); ++it)
    (*it)->specialKeyReleased(key, x, y);
}

void InputManager::mouseButtonChanged(int button, int state, int x, int y)
{
  std::vector<InputHandler*>::iterator it;
  for (it = input_handlers.begin(); it != input_handlers.end(); ++it)
    (*it)->mouseButtonChanged(button, state, x, y);
}

void InputManager::mouseDrag(int x, int y)
{
  std::vector<InputHandler*>::iterator it;
  for (it = input_handlers.begin(); it != input_handlers.end(); ++it)
    (*it)->mouseDrag(x, y);
}

void InputManager::mouseMove(int x, int y)
{
  std::vector<InputHandler*>::iterator it;
  for (it = input_handlers.begin(); it != input_handlers.end(); ++it)
    (*it)->mouseMove(x, y);
}

void InputManager::attatchInputHandler(InputHandler* handler)
{
  if (input_handlers.empty())
  {
    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyReleased);
    glutSpecialFunc(specialKeyPressed);
    glutSpecialUpFunc(specialKeyReleased);
    glutMouseFunc(mouseButtonChanged);
    glutMotionFunc(mouseDrag);
    glutPassiveMotionFunc(mouseMove);
  }
  input_handlers.push_back(handler);
}
