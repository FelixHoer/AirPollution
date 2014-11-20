#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <GL/glut.h>
#include "Window.h"
#include "ObjectCounter.h"

int Window::width = 512;    // set window width in pixels here
int Window::height = 512;   // set window height in pixels here

Node* Window::root = NULL;
Camera* Window::camera = NULL;

FPSCounter* Window::fps_counter = NULL;

bool Window::show_bounding_sphere = true;
bool Window::enable_culling = true;

int Window::time_since_start = 0;

int Window::getDeltaTime()
{
  int new_time_since_start = glutGet(GLUT_ELAPSED_TIME);
  int delta_time = new_time_since_start - time_since_start;
  time_since_start = new_time_since_start;
  return delta_time;
}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
void Window::idleCallback(void)
{
  int delta_time = getDeltaTime();
  root->update(delta_time);

  displayCallback();    // call display routine to redraw cube
}

//----------------------------------------------------------------------------
// Callback method called when window is resized.
void Window::reshapeCallback(int w, int h)
{
  width = w;
  height = h;

  glViewport(0, 0, w, h);  // set new viewport size
}

void Window::renderText(const char* text)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);

  glRasterPos2f(-0.95, 0.9); // top, left corner
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  do 
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
  }
  while (*(++text));

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
}

//----------------------------------------------------------------------------
// Callback method called when window readraw is necessary or
// when glutPostRedisplay() was called.
void Window::displayCallback(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers

  root->render(glm::mat4(), RenderType::CAMERA);
  root->render(camera->getMatrix(), RenderType::LIGHT);
  root->render(camera->getMatrix(), RenderType::OBJECT);
  if (show_bounding_sphere)
    root->render(camera->getMatrix(), RenderType::DEBUG);
  
  std::ostringstream os;
  os << "FPS: " << fps_counter->getCount() << ", "
     << "Objects: " << ObjectCounter::getCount();
  renderText(os.str().c_str());

  fps_counter->count();
  ObjectCounter::endFrame();

  glFlush();
  glutSwapBuffers();
}