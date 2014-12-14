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

  float s_intensity_1 = 1.0f;
  float s_intensity_2 = 0.5f;
  float s_intensity_3 = 1.0f;

  glm::vec2 pos_1 = map_object->getPosition(glm::dvec2(32.66273436, -117.1153007));
  glm::vec2 pos_2 = map_object->getPosition(glm::dvec2(32.68041808, -117.0796912));
  glm::vec2 pos_3 = map_object->getPosition(glm::dvec2(32.68160058, -117.0808056));
  
  MatrixTransform* smoke_transform_1 = new MatrixTransform();
  smoke_transform_1->setMatrix(glm::translate(glm::mat4(), glm::vec3(pos_1.x, 0, pos_1.y)));
  smoke_transform_1->addChild(new SmokeSource(&s_intensity_1, 1));

  MatrixTransform* smoke_transform_2 = new MatrixTransform();
  smoke_transform_2->setMatrix(glm::translate(glm::mat4(), glm::vec3(pos_2.x, 0, pos_2.y)));
  smoke_transform_2->addChild(new SmokeSource(&s_intensity_2, 1));

  MatrixTransform* smoke_transform_3 = new MatrixTransform();
  smoke_transform_3->setMatrix(glm::translate(glm::mat4(), glm::vec3(pos_3.x, 0, pos_3.y)));
  smoke_transform_3->addChild(new SmokeSource(&s_intensity_3, 1));

  SmokeShader* smoke_shader = new SmokeShader();
  smoke_shader->addChild(smoke_transform_1);
  smoke_shader->addChild(smoke_transform_2);
  smoke_shader->addChild(smoke_transform_3);

  MatrixTransform* smoke = new MatrixTransform();
  smoke->addChild(smoke_shader);

  // cube

  MatrixTransform* cube = new MatrixTransform();
  matrix = glm::translate(glm::mat4(), glm::vec3(pos_1.x, 0, pos_1.y))
         * glm::scale(glm::mat4(), glm::vec3(0.1, 0.1, 0.1));
  cube->setMatrix(matrix);
  cube->addChild(new Cube());

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