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
*            Terrain.cpp
*/




#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Window.h"
#include "ObjectCounter.h"

#include "Terrain.h"
#include "MapReader.h"

std::string textureFilename = "..\\Texture\\mapSquare.ppm";




Terrain::Terrain() : Terrain("Terrain")
{
	loadTexture(textureFilename);
}

Terrain::Terrain(char* n) : Geode(n)
{
	ObjectCounter::registerObject();
}

void Terrain::render(const glm::mat4& matrix, const RenderType type)
{
	if (Window::enable_culling &&
		!Window::camera->getFrustum()->isInside(bounding_sphere.transform(matrix)))
		return; // object was culled

	if (type == RenderType::OBJECT)
		renderTerrain(matrix);
	else if (type == RenderType::DEBUG)
		bounding_sphere.render(matrix);
}

void Terrain::renderTerrain(const glm::mat4& matrix)
{
	ObjectCounter::count();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(matrix));

	glEnable(GL_TEXTURE_2D);							// enable 2D texture
	glDisable(GL_TEXTURE_GEN_S);						// disable previous textures
	glDisable(GL_TEXTURE_GEN_T);

	glPushMatrix();

	// Draw sides of cube in object coordinate system:
	glBegin(GL_QUADS);

	// drawing color white to see original texture colors
	glColor3f(1, 1, 1);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 1); glVertex3f(-5, -5, -1);
	glTexCoord2f(1, 1); glVertex3f(5, -5, -1);
	glTexCoord2f(1, 0); glVertex3f(5, 5, -1);
	glTexCoord2f(0, 0); glVertex3f(-5, 5, -1);

	glEnd();

	glPopMatrix();

}

void Terrain::initializeBoundingSphere()
{
	glm::vec3 center(0, 0, 0);
	float radius = glm::length(center - glm::vec3(0.5, 0.5, 0.5));
	bounding_sphere = BoundingSphere(center, radius);
}

BoundingSphere* Terrain::getBoundingSphere()
{
	return &bounding_sphere;
}
