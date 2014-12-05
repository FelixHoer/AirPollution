#ifndef _SMOKE_SOURCE_H_
#define _SMOKE_SOURCE_H_

#include "Geode.h"

class SmokeSource : public Geode
{
protected:
  BoundingSphere bounding_sphere;

  static const GLuint FACES = 1;
  static const GLuint PARTICLES = 1000;

  GLuint vertex_array;
  GLuint vertex_buffer;
  GLuint position_buffer;

  GLfloat vertex_data[4 * 3 * FACES];
  GLfloat position_data[3 * PARTICLES];
  GLfloat velocity_data[3 * PARTICLES];

  void initVertexData(GLfloat* vertex_data);

  void spawn(GLfloat* position, GLfloat* velocity);

  void createArrayBuffer(GLfloat* vertex_data, 
                         GLfloat* position_data);
  void createVertexAttribute(GLfloat* vertex_data);
  void createPositionAttribute(GLfloat* position_data);

  void configureArrayBuffer();
  void configureVertexAttribute();
  void configurePositionAttribute();

  void animate();

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

