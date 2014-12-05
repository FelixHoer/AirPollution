#ifndef _SMOKE_SOURCE_H_
#define _SMOKE_SOURCE_H_

#include "Geode.h"

class SmokeSource : public Geode
{
protected:
  BoundingSphere bounding_sphere;

  static const GLuint FACES = 6 * 2;

  GLuint vertex_array;
  GLuint vertex_buffer;
  GLuint color_buffer;
  GLuint normal_buffer;

  GLfloat vertex_data[3 * 3 * FACES];
  GLubyte color_data[3 * 4 * FACES];
  GLfloat normal_data[3 * 3 * FACES];

  void initVertexData(GLfloat* vertex_data);
  void initColorData(GLubyte* color_data);
  void initNormalData(GLfloat* normal_data);

  void createArrayBuffer(GLfloat* vertex_data, 
                         GLubyte* color_data, 
                         GLfloat* normal_data);
  void createVertexAttribute(GLfloat* vertex_data);
  void createColorAttribute(GLubyte* color_data);
  void createNormalAttribute(GLfloat* normal_data);

  void configureArrayBuffer();
  void configureVertexAttribute();
  void configureColorAttribute();
  void configureNormalAttribute();

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

