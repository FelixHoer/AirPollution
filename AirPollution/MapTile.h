#ifndef _MAP_TILE_H_
#define _MAP_TILE_H_

#include "Geode.h"

class MapTile : public Geode
{
protected:
  GLuint texture;

	BoundingSphere bounding_sphere;

public:
  MapTile(char* name);
  MapTile(std::string name);

  virtual void setup();
  virtual void render(const glm::mat4& matrix);

	virtual void initializeBoundingSphere();
	virtual BoundingSphere* getBoundingSphere();
};

#endif
