#ifndef _SMOKE_SHADER_H_
#define _SMOKE_SHADER_H_

#include "MatrixTransform.h"

class SmokeShader : public MatrixTransform
{
protected:
  static const char* VERTEX_SHADER_PATH;
  static const char* FRAGMENT_SHADER_PATH;

  static const char* TEXTURE_PATH;

  GLuint program;
  GLuint vertex_shader;
  GLuint fragment_shader;
  GLuint texture;

  std::string readFile(std::string path);

  void createProgramWithShaders();
  void configureProgramWithShaders();

  void createTexture();
  void configureTexture();

public:
  SmokeShader();
  SmokeShader(char* name);

  virtual void setup();
  virtual void render(const glm::mat4& matrix);

  virtual GLuint getProgram();
};

#endif

