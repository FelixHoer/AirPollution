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
#include "Camera.h"
#include "Cube.h"
#include "EgoMovement.h"
#include "InputManager.h"
#include "KeyboardInputHandler.h"
#include "DataReader.h"
#include "SmokeShader.h"
#include "SmokeSource.h"


void setupScene()
{
  glm::mat4 matrix;

  // light

  MatrixTransform* directional_light = new MatrixTransform();
  directional_light->setMatrix(glm::rotate(glm::mat4(), (float)M_PI / 6.f, glm::vec3(1, 0, 0)));
  directional_light->addChild(new DirectionalLight(GL_LIGHT0));

  // camera

  Camera* cam = new Camera();
  MatrixTransform* movement_matrix = new MatrixTransform();
  movement_matrix->addChild(cam);
  movement_matrix->addChild(new EgoMovement());

  MatrixTransform* camera = new MatrixTransform();
  matrix = glm::rotate(glm::mat4(), (float)M_PI / 6.f, glm::vec3(0, 1, 0))
         * glm::rotate(glm::mat4(), (float)-M_PI / 6.f, glm::vec3(1, 0, 0))
         * glm::translate(glm::mat4(), glm::vec3(0, 0, 20));
  camera->setMatrix(matrix);
  camera->addChild(movement_matrix);

  // object

  /*
  MatrixTransform* cube = new MatrixTransform();
  matrix = glm::scale(glm::mat4(), glm::vec3(10, 10, 10));
  cube->setMatrix(matrix);
  cube->addChild(new Cube());
  */

  SmokeSource* ssource = new SmokeSource();

  SmokeShader* sshader = new SmokeShader();
  sshader->addChild(ssource);

  MatrixTransform* cube = new MatrixTransform();
  matrix = glm::scale(glm::mat4(), glm::vec3(10, 10, 10));
  cube->setMatrix(matrix);
  cube->addChild(sshader);


  // scene

  MatrixTransform* scene = new MatrixTransform();
  scene->addChild(directional_light);
  scene->addChild(camera);
  scene->addChild(cube);
  scene->setup();

  Window::root = scene;
  Window::camera = cam;
}

void setupApplication()
{
  Data data = DataReader::read("..\\Data\\data.csv");

  setupScene();

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