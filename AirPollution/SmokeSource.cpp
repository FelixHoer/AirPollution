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
  initColorData(color_data);
  initNormalData(normal_data);

  createArrayBuffer(vertex_data, color_data, normal_data);

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

void SmokeSource::initColorData(GLubyte* color_data)
{
  for (unsigned int i = 0; i < 4 * FACES; i++) {
    color_data[i * 4 + 0] = 200;
    color_data[i * 4 + 1] = 200;
    color_data[i * 4 + 2] = 200;
    color_data[i * 4 + 3] = 255;
  }
}

void SmokeSource::initNormalData(GLfloat* normal_data)
{
  for (int i = 0; i < 4 * FACES; i++) {
    normal_data[i * 3 + 0] = 0.0f;
    normal_data[i * 3 + 1] = 0.0f;
    normal_data[i * 3 + 2] = 1.0f;
  }
}

void SmokeSource::createArrayBuffer(GLfloat* vertex_data, GLubyte* color_data, GLfloat* normal_data)
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
  createColorAttribute(color_data);
  createNormalAttribute(normal_data);
}

void SmokeSource::configureArrayBuffer()
{
  GLuint error;

  // bind vertex array
  glBindVertexArray(vertex_array);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, bind vertex array: " << error << std::endl;

  configureVertexAttribute();
  configureColorAttribute();
  configureNormalAttribute();
}

void SmokeSource::createVertexAttribute(GLfloat* vertex_data)
{
  GLuint error;

  // create buffer for attribute 1: vertex
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, 4 * FACES * 3 * sizeof(GLfloat), vertex_data, GL_STATIC_DRAW);
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
}

void SmokeSource::createColorAttribute(GLubyte* color_data)
{
  GLuint error;

  // create buffer for attribute 2: color
  glGenBuffers(1, &color_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
  glBufferData(GL_ARRAY_BUFFER, 4 * FACES * 4 * sizeof(GLubyte), color_data, GL_STATIC_DRAW);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, gen color buffer: " << error << std::endl;
}

void SmokeSource::configureColorAttribute()
{
  GLuint error;

  glBindBuffer(GL_ARRAY_BUFFER, color_buffer);

  // enable color data as attribute 2
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, attrib pointer: " << error << std::endl;
}

void SmokeSource::createNormalAttribute(GLfloat* normal_data)
{
  GLuint error;

  // create buffer for attribute 3: normals
  glGenBuffers(1, &normal_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
  glBufferData(GL_ARRAY_BUFFER, 4 * FACES * 3 * sizeof(GLfloat), normal_data, GL_STATIC_DRAW);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, gen normal buffer: " << error << std::endl;
}

void SmokeSource::configureNormalAttribute()
{
  GLuint error;

  glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);

  // enable normals data as attribute 3
  // (index, size, type, normalized, stride, pointer)
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, attrib pointer: " << error << std::endl;
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

  glDrawArrays(GL_QUADS, 0, 4 * FACES);

  glDisableVertexAttribArray(0);
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
