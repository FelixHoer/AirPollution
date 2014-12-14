#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Window.h"
#include "ObjectCounter.h"
#include "SmokeShader.h"

#include "SmokeSource.h"


SmokeSource::SmokeSource(GLfloat* intensities, unsigned int intensities_size)
: SmokeSource("SmokeSource", intensities, intensities_size)
{}

SmokeSource::SmokeSource(char* n, GLfloat* is, unsigned int is_size)
: Geode(n), particles(0), desired_particles(0)
{
  ObjectCounter::registerObject();

  intensities = new GLfloat[is_size];
  for (unsigned int i = 0; i < is_size; i++)
    intensities[i] = is[i];
}

void SmokeSource::setup()
{
  initVertexData(vertex_data);
  initTextureData(texture_data);

  createArrayBuffer(vertex_data, position_data);

  initializeBoundingSphere();
}

void SmokeSource::initVertexData(GLfloat* vertex_data)
{
  GLfloat points[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f
  };

  int faces[] = {
    0, 1, 3, 2
  };

  for (int i = 0; i < 4 * FACES; i++) {
    vertex_data[i * 3 + 0] = 0.5f * points[faces[i] * 3 + 0];
    vertex_data[i * 3 + 1] = 0.0f;
    vertex_data[i * 3 + 2] = 0.5f * points[faces[i] * 3 + 1];
  }
}

void SmokeSource::initTextureData(GLfloat* texture_data)
{
  GLfloat texture[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f
  };

  for (int i = 0; i < 4 * FACES; i++) {
    texture_data[i * 2 + 0] = texture[i * 2 + 0];
    texture_data[i * 2 + 1] = texture[i * 2 + 1];
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
  createTextureAttribute(texture_data);
  createPositionAttribute();
}

void SmokeSource::configureArrayBuffer()
{
  GLuint error;

  // bind vertex array
  glBindVertexArray(vertex_array);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, bind vertex array: " << error << std::endl;

  configureVertexAttribute();
  configureTextureAttribute();
  configurePositionAttribute(NULL);
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

  // set attribute divisor
  glVertexAttribDivisor(0, 0);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, attribute divisor: " << error << std::endl;
}

void SmokeSource::createTextureAttribute(GLfloat* texture_data)
{
  GLuint error;

  // create buffer for attribute 2: texture
  glGenBuffers(1, &texture_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, texture_buffer);
  glBufferData(GL_ARRAY_BUFFER, 4 * FACES * 2 * sizeof(GLfloat), texture_data, GL_STATIC_DRAW);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, gen vertex buffer: " << error << std::endl;
}

void SmokeSource::configureTextureAttribute()
{
  GLuint error;

  glBindBuffer(GL_ARRAY_BUFFER, texture_buffer);

  // enable texture data as attribute 2
  // (index, size, type, normalized, stride, pointer)
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, attrib pointer: " << error << std::endl;

  // set attribute divisor
  glVertexAttribDivisor(1, 0);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, attribute divisor: " << error << std::endl;
}

void SmokeSource::createPositionAttribute()
{
  GLuint error;

  // create buffer for attribute 3: position
  glGenBuffers(1, &position_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
  glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, gen position buffer: " << error << std::endl;
}

void SmokeSource::configurePositionAttribute(GLfloat* position_data)
{
  GLuint error;

  glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
  glBufferSubData(GL_ARRAY_BUFFER, 0, particles * 3 * sizeof(GLfloat), position_data);

  // enable position data as attribute 3
  // (index, size, type, normalized, stride, pointer)
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, attrib pointer: " << error << std::endl;

  // set attribute divisor
  glVertexAttribDivisor(2, 1);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init buffer error, attribute divisor: " << error << std::endl;
}

void SmokeSource::adjustLevelOfDetail(float distance)
{
  unsigned int new_particles = MAX_PARTICLES / sqrt(distance);
  desired_particles = std::min(new_particles, MAX_PARTICLES);

  // spawn new if more are needed
  if (desired_particles > particles)
  {
    for (unsigned int i = particles; i < desired_particles; i++)
      spawn(i);
    particles = desired_particles;
  }

  // else let old ones just die
}

void SmokeSource::spawn(unsigned int next_dead)
{
  GLfloat* position = &(position_data[next_dead * 3]);
  GLfloat* velocity = &(velocity_data[next_dead * 3]);

  position[0] = 0.0f;
  position[1] = 0.0f;
  position[2] = 0.0f;

  float r1 = float(std::rand()) / RAND_MAX;
  float r2 = float(std::rand()) / RAND_MAX;
  float r3 = float(std::rand()) / RAND_MAX;
  velocity[0] = r1 * 2.0f - 1.0f;
  velocity[1] = r2;
  velocity[2] = r3 * 2.0f - 1.0f;
}

void SmokeSource::kill(unsigned int index)
{
  int last_alive = particles - 1;

  // swap positions
  for (unsigned int i = 0; i < 3; i++)
  {
    float temp = position_data[index * 3 + i];
    position_data[index * 3 + i] = position_data[last_alive * 3 + i];
    position_data[last_alive * 3 + i] = temp;
  }

  // swap velocities
  for (unsigned int i = 0; i < 3; i++)
  {
    float temp = velocity_data[index * 3 + i];
    velocity_data[index * 3 + i] = velocity_data[last_alive * 3 + i];
    velocity_data[last_alive * 3 + i] = temp;
  }

  particles--;
}

void SmokeSource::animate()
{
  float up_scale = Window::delta_time / 2000.0f;
  float side_scale = Window::delta_time / 6000.0f;

  // update velocity
  for (unsigned int i = 0; i < particles; i++)
    velocity_data[i * 3 + 1] += up_scale * -1.0f;

  // update position
  for (unsigned int i = 0; i < particles; i++)
  {
    position_data[i * 3 + 0] += velocity_data[i * 3 + 0] * side_scale;
    position_data[i * 3 + 1] += velocity_data[i * 3 + 1] * side_scale;
    position_data[i * 3 + 2] += velocity_data[i * 3 + 2] * side_scale;
  }

  // kill and respawn particles
  for (unsigned int i = 0; i < particles; i++)
  {
    if (position_data[i * 3 + 1] <= 0.0f)
    {
      if (particles <= desired_particles)
        spawn(i);
      else
      {
        kill(i); // kill and swap with last particle
        i--; // check again on this position, since another particle will be here
      }
    }
  }

  // update data in buffer
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
  glBufferSubData(GL_ARRAY_BUFFER, 0, particles * 3 * sizeof(GLfloat), position_data);
}

void SmokeSource::setShaderMatrix(const glm::mat4& matrix)
{
  SmokeShader* ss = (SmokeShader*) findUp(typeid(SmokeShader));
  glm::mat4 p_matrix;
  glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(p_matrix));

  glm::mat4 mvp_matrix = p_matrix * matrix;

  GLint mvp_location = glGetUniformLocation(ss->getProgram(), "mvp_matrix");
  glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp_matrix));
}

void SmokeSource::setIntensity(GLfloat intensity)
{
  SmokeShader* ss = (SmokeShader*) findUp(typeid(SmokeShader));
  GLint location = glGetUniformLocation(ss->getProgram(), "intensity");
  glUniform1f(location, intensity);
}

void SmokeSource::render(const glm::mat4& matrix)
{
  if (!visible)
  {
    particles = 0;
    return; // object was culled
  }

  if (Window::debug)
    bounding_sphere.render(matrix);

  ObjectCounter::count();

  configureArrayBuffer();

  glm::vec3 position = glm::vec3(matrix * glm::vec4(0, 0, 0, 1));
  float distance = glm::length(position);
  adjustLevelOfDetail(distance);

  animate();

  setShaderMatrix(matrix);
  float intensity = intensities[Window::active_measurement];
  if (intensity == FLT_MAX)
    return;
  setIntensity(intensity);
  
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDrawArraysInstanced(GL_QUADS, 0, 4 * FACES, particles);

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void SmokeSource::initializeBoundingSphere()
{
  glm::vec3 center(0,0,0);
  float radius = glm::length(center - glm::vec3(0.5f, 0.5f, 0.5f));
  bounding_sphere = BoundingSphere(center, radius);
}

BoundingSphere* SmokeSource::getBoundingSphere()
{
  return &bounding_sphere;
}
