#ifndef _SMOKE_SOURCE_H_
#define _SMOKE_SOURCE_H_

#include "Geode.h"

class SmokeSource : public Geode
{
protected:
  BoundingSphere bounding_sphere;

  static const GLuint FACES = 1;
  static const GLuint MAX_PARTICLES = 100;

  unsigned int particles;
  unsigned int desired_particles;

  GLuint vertex_array;
  GLuint vertex_buffer;
  GLuint texture_buffer;
  GLuint position_buffer;

  GLfloat vertex_data[4 * 3 * FACES];
  GLfloat texture_data[4 * 2 * FACES];
  GLfloat position_data[3 * MAX_PARTICLES];
  GLfloat velocity_data[3 * MAX_PARTICLES];

  void initVertexData(GLfloat* vertex_data);
  void initTextureData(GLfloat* texture_data);

  void spawn(unsigned int index);
  void kill(unsigned int index);

  void createArrayBuffer(GLfloat* vertex_data, 
                         GLfloat* position_data);
  void createVertexAttribute(GLfloat* vertex_data);
  void createTextureAttribute(GLfloat* texture_data);
  void createPositionAttribute();

  void configureArrayBuffer();
  void configureVertexAttribute();
  void configureTextureAttribute();
  void configurePositionAttribute(GLfloat* position_data);

  void adjustLevelOfDetail(float distance);
  void animate();
  void setShaderMatrix(const glm::mat4& matrix);

  virtual void renderSmoke(const glm::mat4& matrix);

public:
  SmokeSource();
  SmokeSource(char* name);

  virtual void setup();
  virtual void render(const glm::mat4& matrix, const RenderType type);

  virtual void initializeBoundingSphere();
  virtual BoundingSphere* getBoundingSphere();
};

#endif

