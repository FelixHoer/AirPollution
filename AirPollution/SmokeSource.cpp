#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Window.h"
#include "ObjectCounter.h"
#include "SmokeShader.h"

#include "SmokeSource.h"


SmokeSource::SmokeSource() : SmokeSource("SmokeSource")
{}

SmokeSource::SmokeSource(char* n) : Geode(n)
{
  ObjectCounter::registerObject();
}

void SmokeSource::setup()
{
  initVertexData(vertex_data);
  initPositionData(position_data);

  createArrayBuffer(vertex_data, position_data);

  initializeBoundingSphere();
}

void SmokeSource::initVertexData(GLfloat* vertex_data)
{
  GLfloat points[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,
    1.0f,  1.0f, 0.0f
  };

  int faces[] = {
    0, 1, 3, 2
  };

  for (int i = 0; i < 4 * FACES; i++) {
    vertex_data[i * 3 + 1] = 0.5f * points[faces[i] * 3 + 0];
    vertex_data[i * 3 + 0] = 0.5f * points[faces[i] * 3 + 1];
    vertex_data[i * 3 + 2] = 0.5f * points[faces[i] * 3 + 2];
  }
}

void SmokeSource::initPositionData(GLfloat* position_data)
{
  for (int i = 0; i < 3 * PARTICLES; i++) {
    float r1 = float(std::rand()) / RAND_MAX;
    float r2 = float(std::rand()) / RAND_MAX;
    float r3 = float(std::rand()) / RAND_MAX;
    position_data[i * 3 + 0] = r1 * 2.0f - 1.0f;
    position_data[i * 3 + 1] = r2 * 2.0f - 1.0f;
    position_data[i * 3 + 2] = r3 * 2.0f - 1.0f;
  }
}

void SmokeSource::createArrayBuffer(GLfloat* vertex_data, GLfloat* position_data)
{
  GLuint error;

  // create vertex array
  glGenVertexArrays(1, &vertex_array);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, gen vertex array: " << error << std::endl;

  // bind vertex array
  glBindVertexArray(vertex_array);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, bind vertex array: " << error << std::endl;

  createVertexAttribute(vertex_data);
  createPositionAttribute(position_data);
}

void SmokeSource::configureArrayBuffer()
{
  GLuint error;

  // bind vertex array
  glBindVertexArray(vertex_array);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, bind vertex array: " << error << std::endl;

  configureVertexAttribute();
  configurePositionAttribute();
}

void SmokeSource::createVertexAttribute(GLfloat* vertex_data)
{
  GLuint error;

  // create buffer for attribute 1: vertex
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, 4 * FACES * 3 * sizeof(GLfloat), vertex_data, GL_STREAM_DRAW);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, gen vertex buffer: " << error << std::endl;
}

void SmokeSource::configureVertexAttribute()
{
  GLuint error;

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

  // enable vertex data as attribute 1
  // (index, size, type, normalized, stride, pointer)
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, attrib pointer: " << error << std::endl;

  // set attribute divisor
  glVertexAttribDivisor(0, 0);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, attribute divisor: " << error << std::endl;
}

void SmokeSource::createPositionAttribute(GLfloat* position_data)
{
  GLuint error;

  // create buffer for attribute 2: position
  glGenBuffers(1, &position_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
  glBufferData(GL_ARRAY_BUFFER, PARTICLES * 3 * sizeof(GLfloat), position_data, GL_STREAM_DRAW);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, gen position buffer: " << error << std::endl;
}

void SmokeSource::configurePositionAttribute()
{
  GLuint error;

  glBindBuffer(GL_ARRAY_BUFFER, position_buffer);

  // enable position data as attribute 2
  // (index, size, type, normalized, stride, pointer)
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, attrib pointer: " << error << std::endl;

  // set attribute divisor
  glVertexAttribDivisor(1, 1);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, attribute divisor: " << error << std::endl;
}

void SmokeSource::render(const glm::mat4& matrix, const RenderType type)
{
  if (Window::enable_culling &&
      !Window::camera->getFrustum()->isInside(bounding_sphere.transform(matrix)))
    return; // object was culled

  if (type == RenderType::OBJECT)
    renderSmoke(matrix);
  else if (type == RenderType::DEBUG)
    bounding_sphere.render(matrix);
}

void SmokeSource::renderSmoke(const glm::mat4& matrix)
{
  ObjectCounter::count();

  configureArrayBuffer();

  SmokeShader* ss = (SmokeShader*)parent;
  glm::mat4 p_matrix;
  glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(p_matrix));
  glm::mat4 mvp_matrix = p_matrix * matrix;
  GLint mvp_location = glGetUniformLocation(ss->getProgram(), "mvp_matrix");
  glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp_matrix));

  //glDrawArrays(GL_QUADS, 0, 4 * FACES);
  glDrawArraysInstanced(GL_QUADS, 0, 4 * FACES, PARTICLES);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void SmokeSource::initializeBoundingSphere()
{
  glm::vec3 center(0, 0, 0);
  float radius = glm::length(center - glm::vec3(0.5f, 0.5f, 0.5f));
  bounding_sphere = BoundingSphere(center, radius);
}

BoundingSphere* SmokeSource::getBoundingSphere()
{
  return &bounding_sphere;
}
