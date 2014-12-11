#ifndef _MAP_TILE_H_
#define _MAP_TILE_H_

#include "Geode.h"

class MapTile : public Geode
{
protected:
  GLuint texture;

	BoundingSphere bounding_sphere;

	virtual void renderTile(const glm::mat4& matrix);

public:
  MapTile(char* name);
  MapTile(std::string name);

  virtual void setup();
  virtual void render(const glm::mat4& matrix, const RenderType type);

	virtual void initializeBoundingSphere();
	virtual BoundingSphere* getBoundingSphere();
};

#endif
