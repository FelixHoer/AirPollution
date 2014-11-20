#ifndef _INPUT_HANDLER_H_
#define _INPUT_HANDLER_H_

#include <vector>

class InputHandler
{
public:
  virtual void keyPressed(unsigned char key, int x, int y);
  virtual void keyReleased(unsigned char key, int x, int y);
  virtual void specialKeyPressed(int key, int x, int y);
  virtual void specialKeyReleased(int key, int x, int y);
  virtual void mouseButtonChanged(int button, int state, int x, int y);
  virtual void mouseDrag(int x, int y);
  virtual void mouseMove(int x, int y);
};

#endif

