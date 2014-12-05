#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "ObjectCounter.h"

#include "SmokeShader.h"


const GLchar* SmokeShader::VERTEX_SHADER_CODE =
"#version 330 \n\
uniform mat4 mvp_matrix; \n\
layout(location = 0) in vec3 vertex_position; \n\
layout(location = 1) in vec4 vertex_color; \n\
layout(location = 2) in vec3 vertex_normal; \n\
layout(location = 0) out vec4 color; \n\
void main(void) \n\
{ \n\
  gl_Position = mvp_matrix * vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0f); \n\
  color = vertex_color; \n\
} \n";

const GLchar* SmokeShader::FRAGMENT_SHADER_CODE =
"#version 330 \n\
layout(location = 0) in vec4 color; \n\
void main(void) \n\
{ \n\
  gl_FragColor = color; \n\
} \n";


SmokeShader::SmokeShader() : SmokeShader("SmokeShader")
{}

SmokeShader::SmokeShader(char* n) : MatrixTransform(n)
{}

void SmokeShader::setup()
{
  createProgramWithShaders();
  MatrixTransform::setup();
}

void SmokeShader::createProgramWithShaders()
{
  GLuint error;
  GLchar* error_log = new GLchar[1000];
  GLsizei error_log_size = 0;
  GLint compile_status;

  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init program error: " << error << std::endl;

  // create and compile vertex shader

  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init program error, create shader: " << error << std::endl;

  glShaderSource(vertex_shader, 1, (const GLchar **)&VERTEX_SHADER_CODE, NULL);
  glCompileShader(vertex_shader);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init program error, vertex shader: " << error << std::endl;

  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
  if (compile_status != GL_TRUE)
  {
    glGetShaderInfoLog(program, 1000, &error_log_size, error_log);
    std::cerr << "vertex compile error log: " << error_log << std::endl;
  }

  // create and compile fragment shader

  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init program error, create shader: " << error << std::endl;
  glShaderSource(fragment_shader, 1, (const GLchar **)&FRAGMENT_SHADER_CODE, NULL);
  glCompileShader(fragment_shader);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init program error, fragment shader: " << error << std::endl;

  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
  if (compile_status != GL_TRUE)
  {
    glGetShaderInfoLog(program, 1000, &error_log_size, error_log);
    std::cerr << "fragment compile error log: " << error_log << std::endl;
  }

  // create program, attach shaders and link program

  program = glCreateProgram();
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init program error, create program: " << error << std::endl;
  glAttachShader(program, vertex_shader);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init program error, attach: " << error << std::endl;
  glAttachShader(program, fragment_shader);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init program error, attach: " << error << std::endl;
  glLinkProgram(program);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init program error, link: " << error << std::endl;
}

void SmokeShader::configureProgramWithShaders()
{
  GLuint error;
  GLchar* error_log = new GLchar[1000];
  GLsizei error_log_size = 0;

  // use linked program with the shaders

  glUseProgram(program);
  if ((error = glGetError()) != GL_NO_ERROR)
  {
    std::cerr << "init program error, use program: " << error << std::endl;
    glGetProgramInfoLog(program, 1000, &error_log_size, error_log);
    std::cerr << "error log: " << error_log << std::endl;
  }
}

void SmokeShader::render(const glm::mat4& matrix, const RenderType type)
{
  if (type == RenderType::OBJECT)
  {
    configureProgramWithShaders();
    setLightSource();
  }

  MatrixTransform::render(matrix, type);

  if (type == RenderType::OBJECT)
  {
    glUseProgram(0);
  }
}

void SmokeShader::setLightSource()
{
  GLint intensity_location = glGetUniformLocation(program, "lightsource_intensity");
  glUniform3f(intensity_location, 4.0f, 4.0f, 4.0f);

  GLint direction_location = glGetUniformLocation(program, "lightsource_direction");
  glUniform3f(direction_location, 0.6f, 0.8f, 1.0f);
}

GLuint SmokeShader::getProgram()
{
  return program;
}