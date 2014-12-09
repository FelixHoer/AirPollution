#ifndef _TEXTURE_READER_H_
#define _TEXTURE_READER_H_

#include <string>

class TextureReader
{
protected:
  static unsigned char* loadPPM(const char* filename, int& width, int& height);
public:
  static GLuint loadTexture(std::string filename);
  static void configureTexture(GLint texture_number, GLuint texture_id);
};

#endif
