#ifndef _SMOKE_SHADER_H_
#define _SMOKE_SHADER_H_

#include "MatrixTransform.h"

class SmokeShader : public MatrixTransform
{
protected:
  static const GLchar* VERTEX_SHADER_CODE;
  static const GLchar* FRAGMENT_SHADER_CODE;

  GLuint program;
  GLuint vertex_shader;
  GLuint fragment_shader;

  void createProgramWithShaders();
  void configureProgramWithShaders();

  void setLightSource();

public:
  SmokeShader();
  SmokeShader(char* name);

  virtual void setup();
  virtual void render(const glm::mat4& matrix, const RenderType type);

  virtual GLuint getProgram();
};

#endif

