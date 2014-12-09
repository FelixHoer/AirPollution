/*
*      Group Final Assignment 7
*      CS 596 Computer Graphics
*     SAN DIEGO STATE UNIVERSITY
*
*
*
*   	    AIR POLLUTION
*
*    CITY OF CHULA VISTA, CA, USA
*        __________________
*
*    Professor Jurgen P. SCHULZE, PhD
*  		  TA Bela MHASAVADE
*
*             Fall 2014
*  ________________________________
*
*		     Mila CRIDLIG
*          FELIX HOERANDNER
*
*         December 15, 2014
*         __________________
*/

/*
*            Terrain.h
*/




#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "Geode.h"

class Terrain : public Geode
{
protected:
	BoundingSphere bounding_sphere;

	virtual void renderTerrain(const glm::mat4& matrix);

public:
	Terrain();
	Terrain(char* name);

	virtual void render(const glm::mat4& matrix, const RenderType type);

	virtual void initializeBoundingSphere();
	virtual BoundingSphere* getBoundingSphere();
};

#endif
