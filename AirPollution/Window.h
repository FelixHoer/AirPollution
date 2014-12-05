#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "Node.h"
#include "Camera.h"
#include "FPSCounter.h"

class Window	  // output window related routines
{
public:
  static Node* root;
  static Camera* camera;

  static FPSCounter* fps_counter;

  static bool show_bounding_sphere;
  static bool enable_culling;

  static int time_since_start;
  static int delta_time;

  static int width, height; 	            // window size

  static void idleCallback(void);
  static void reshapeCallback(int, int);
  static void displayCallback(void);

  static int calculateDeltaTime();
  static void renderText(const char*);
};

#endif

