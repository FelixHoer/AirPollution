#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

#include "Window.h"
#include "MatrixTransform.h"
#include "DirectionalLight.h"
#include "Map.h"
#include "Camera.h"
#include "Cube.h"
#include "EgoMovement.h"
#include "InputManager.h"
#include "KeyboardInputHandler.h"
#include "DataReader.h"
#include "SmokeShader.h"
#include "SmokeSource.h"



typedef struct {
  glm::dvec2 position;
  float intensity[4];
} Location;

bool isInvalid(float a, float b)
{
  return a > 0.9f * b;
}
bool isInvalid(double a, double b)
{
  return a > 0.9 * b;
}
bool isInvalid(unsigned int a, unsigned int b)
{
  return a == b;
}
bool isInvalid(unsigned short a, unsigned short b)
{
  return a == b;
}

Location buildLocation(Data data, unsigned int from, unsigned int to)
{
  // build a new location
  Location location;
  location.position = glm::dvec2(0, 0);
  location.intensity[0] = 0;
  location.intensity[1] = 0;
  location.intensity[2] = 0;
  location.intensity[3] = 0;

  // calculate average
  unsigned int sizes[4] = {0, 0, 0, 0};
  for (unsigned int j = from; j <= to; j++)
  {
    DataPoint* old_point = data.points + j;

    location.position += glm::dvec2(old_point->latitude, old_point->longitude);

    if (!isInvalid(old_point->pm10, DataReader::INVALID_PM10))
    {
      sizes[0]++;
      location.intensity[0] += old_point->pm10;
    }
    if (!isInvalid(old_point->pm25, DataReader::INVALID_PM25))
    {
      sizes[1]++;
      location.intensity[1] += old_point->pm25;
    }
    if (!isInvalid(old_point->carbon, DataReader::INVALID_CARBON))
    {
      sizes[2]++;
      location.intensity[2] += old_point->carbon;
    }
    if (!isInvalid(old_point->no2, DataReader::INVALID_NO2))
    {
      sizes[3]++;
      location.intensity[3] += old_point->no2;
    }
  }

  // normalize
  location.position /= to - from + 1;
  for (unsigned int i = 0; i < 4; i++)
    if (sizes[i] > 0)
      location.intensity[i] /= sizes[i];
    else
      location.intensity[i] = FLT_MAX; // invalid

  return location;
}

std::vector<Location> filterData(Data data)
{
  std::vector<Location> locations;

  unsigned int same_count = 0;
  unsigned int start_index = 0;

  DataPoint* last_point = data.points;
  for (unsigned int i = 1; i < data.size; i++)
  {
    DataPoint* point = data.points + i;
    if (abs(point->latitude - last_point->latitude) < 0.0001 &&
        abs(point->longitude - last_point->longitude) < 0.0001)
    {
      same_count++;
    }
    else
    {
      if (same_count > 10)
        locations.push_back(buildLocation(data, start_index, i - 1));
      same_count = 0;
      start_index = i;
    }
    last_point = point;
  }

  if (same_count > 10)
    locations.push_back(buildLocation(data, start_index, data.size - 1));

  return locations;
}

std::vector<Location> selectData(Data data)
{
  std::vector<Location> locations = filterData(data);
  std::vector<Location> output;

  float min_intensities[4] = { FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX };
  float max_intensities[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  
  for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); ++it)
  {
    Location location = *it;
    for (unsigned int i = 0; i < 4; i++)
    {
      if (!isInvalid(location.intensity[i], FLT_MAX))
      {
        if (location.intensity[i] < min_intensities[i])
          min_intensities[i] = location.intensity[i];
        if (location.intensity[i] > max_intensities[i])
          max_intensities[i] = location.intensity[i];
      }
    }
  }

  for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); ++it)
  {
    Location location = *it;
    for (unsigned int i = 0; i < 4; i++)
    {
      if (!isInvalid(location.intensity[i], FLT_MAX)
        && abs(min_intensities[i] - max_intensities[i]) > 0.00001
        && min_intensities[i] < 0.9f * FLT_MAX
        && max_intensities[i] > 0.1f)
        location.intensity[i] = (location.intensity[i] - min_intensities[i]) / (max_intensities[i] - min_intensities[i]);
      else
        location.intensity[i] = FLT_MAX;
    }
    output.push_back(location);
  }

  return output;
}

void setupScene(Data data)
{
  glm::mat4 matrix;

  // light

  DirectionalLight* light = new DirectionalLight(GL_LIGHT0);

  MatrixTransform* directional_light = new MatrixTransform();
  directional_light->setMatrix(glm::rotate(glm::mat4(), (float)M_PI / 6.f, glm::vec3(1, 0, 0)));
  directional_light->addChild(light);

  // camera

  Camera* camera_object = new Camera();

  MatrixTransform* movement_matrix = new MatrixTransform();
  movement_matrix->addChild(camera_object);
  movement_matrix->addChild(new EgoMovement());

  MatrixTransform* camera = new MatrixTransform();
  matrix = glm::translate(glm::mat4(), glm::vec3(16, 16, 0));
  camera->setMatrix(matrix);
  camera->addChild(movement_matrix);

  // map

  Map* map_object = new Map(11434, 26458, 32, 16);

  MatrixTransform* map = new MatrixTransform();
  map->setMatrix(glm::translate(glm::mat4(), glm::vec3(16, 0, 16)));
  map->addChild(map_object);

  // object smoke

  SmokeShader* smoke_shader = new SmokeShader();

  std::vector<Location> locations = selectData(data);
  for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); ++it)
  {
    Location location = *it;

    glm::vec2 pos = map_object->getPosition(location.position);

    MatrixTransform* smoke_transform = new MatrixTransform();
    smoke_transform->setMatrix(glm::translate(glm::mat4(), glm::vec3(pos.x, 0, pos.y)));
    smoke_transform->addChild(new SmokeSource(location.intensity, 4));

    smoke_shader->addChild(smoke_transform);
  }

  MatrixTransform* smoke = new MatrixTransform();
  smoke->addChild(smoke_shader);

  // cube

  /*
  MatrixTransform* cube = new MatrixTransform();
  matrix = glm::translate(glm::mat4(), glm::vec3(pos_1.x, 0, pos_1.y))
         * glm::scale(glm::mat4(), glm::vec3(0.1, 0.1, 0.1));
  cube->setMatrix(matrix);
  cube->addChild(new Cube());
  */

  // scene

  MatrixTransform* scene = new MatrixTransform();
  scene->setMatrix(glm::scale(glm::mat4(), glm::vec3(10, 10, 10)));
  scene->addChild(camera);
  scene->addChild(directional_light);
  scene->addChild(map);
  scene->addChild(smoke);
  //scene->addChild(cube);

  // register in Window

  Window::camera = camera_object;
  Window::lights.push_back(light);
  Window::root = scene;
  Window::map = map_object;

  // setup

  std::cout << "Setting up scene... ";
  scene->setup();
  std::cout << "done." << std::endl;
}

void setupApplication()
{
  std::cout << "Reading data... ";
  Data data = DataReader::read("..\\Data\\data.csv");
  std::cout << "done." << std::endl;

  setupScene(data);

  Window::fps_counter = new FPSCounter();

  InputManager::attatchInputHandler(new KeyboardInputHandler());
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);      	      	      // initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
  glutInitWindowSize(Window::width, Window::height);      // set initial window size
  glutCreateWindow("OpenGL Cube");    	      // open window and set window title
  
  if (glutGet(GLUT_WINDOW_COLORMAP_SIZE) != 0)
  {
    std::cerr << "Fatal Error: bad RGBA colormap size\n" << std::endl;
    return 1;
  }

  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    /* Problem: glewInit failed, something is seriously wrong. */
    std::cout << "Error: " << glewGetErrorString(err) << std::endl;
    return 2;
  }
  std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

  if (!GL_VERSION_3_3)
  {
    std::cout << "Error: GL v3.3" << std::endl;
    return 3;
  }

  glEnable(GL_DEPTH_TEST);            	      // enable depth buffering
  glClear(GL_DEPTH_BUFFER_BIT);       	      // clear depth buffer
  glClearColor(0.0, 0.0, 0.0, 0.0);   	      // set clear color to black
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // set polygon drawing mode to fill front and back of each polygon
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glShadeModel(GL_SMOOTH);             	      // set shading to smooth
  glMatrixMode(GL_PROJECTION);

  //glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glEnable(GL_NORMALIZE);

  // Install callback functions:
  glutDisplayFunc(Window::displayCallback);
  glutReshapeFunc(Window::reshapeCallback);
  glutIdleFunc(Window::idleCallback);

  setupApplication();

  glutMainLoop();
  return 0;
}