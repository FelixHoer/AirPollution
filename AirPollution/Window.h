#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <vector>
#include "Node.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "Map.h"
#include "FPSCounter.h"

class Window	  // output window related routines
{
public:
  static Node* root;
  static Camera* camera;
  static std::vector<DirectionalLight*> lights;
  static Map* map;

  static FPSCounter* fps_counter;

  static bool debug;

  static unsigned int active_measurement;

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

