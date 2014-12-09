#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "ObjectCounter.h"
#include "TextureReader.h"

#include "SmokeShader.h"


const char* SmokeShader::VERTEX_SHADER_PATH = "..\\Shader\\SmokeVertexShader.glsl";
const char* SmokeShader::FRAGMENT_SHADER_PATH = "..\\Shader\\SmokeFragmentShader.glsl";

const char* SmokeShader::TEXTURE_PATH = "..\\Texture\\smokeparticle.ppm";


SmokeShader::SmokeShader() : SmokeShader("SmokeShader")
{}

SmokeShader::SmokeShader(char* n) : MatrixTransform(n)
{}

void SmokeShader::setup()
{
  createProgramWithShaders();
  configureProgramWithShaders();
  createTexture();
  configureTexture();
  MatrixTransform::setup();
}

std::string SmokeShader::readFile(std::string path)
{
  std::string file;

  std::ifstream myfile(path);
  if (!myfile.is_open())
  {
    std::cout << "could not open file: " << path << std::endl;
    return "";
  }

  std::string line;
  while (std::getline(myfile, line))
  {
    file += line;
    file += "\n";
  }
  myfile.close();

  return file;
}

void SmokeShader::createProgramWithShaders()
{
  GLuint error;
  GLchar* error_log = new GLchar[1000];
  GLsizei error_log_size = 0;
  GLint compile_status;
  const GLchar* code;

  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init program error: " << error << std::endl;

  // create and compile vertex shader

  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  if ((error = glGetError()) != GL_NO_ERROR)
    std::cerr << "init program error, create shader: " << error << std::endl;

  std::string vertex_shader_code = readFile(VERTEX_SHADER_PATH);
  code = vertex_shader_code.c_str();
  glShaderSource(vertex_shader, 1, &code, NULL);
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

  std::string fragment_shader_code = readFile(FRAGMENT_SHADER_PATH);
  code = fragment_shader_code.c_str();
  glShaderSource(fragment_shader, 1, &code, NULL);
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

void SmokeShader::createTexture()
{
  texture = TextureReader::loadTexture(TEXTURE_PATH);
}

void SmokeShader::configureTexture()
{
  TextureReader::configureTexture(GL_TEXTURE1, texture);

  GLuint error;
  GLchar* error_log = new GLchar[1000];
  GLsizei error_log_size = 0;

  GLint texture_image_location = glGetUniformLocation(program, "texture_image");
  glUniform1i(texture_image_location, 1);

  if ((error = glGetError()) != GL_NO_ERROR)
  {
    std::cerr << "configure texture: " << error << std::endl;
    glGetProgramInfoLog(program, 1000, &error_log_size, error_log);
    std::cerr << "error log: " << error_log << std::endl;
  }
}

void SmokeShader::render(const glm::mat4& matrix, const RenderType type)
{
  if (type == RenderType::OBJECT)
  {
    configureProgramWithShaders();
    configureTexture();
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