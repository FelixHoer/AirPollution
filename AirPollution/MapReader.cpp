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
*            Reader.cpp
*/




#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glut.h>

#include "MapReader.h"


using namespace std;



unsigned char* loadPPM(const char* filename, int& width, int& height) {

	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}

// load image file into texture object
void loadTexture(string filename) {

	GLuint texture[1];					// storage for one texture
	int textureWidth, textureHeight;	// texture width/height [pixels]
	unsigned char* texturePointer;		// texture pixel data

	// Load image file
	texturePointer = loadPPM(filename.c_str(), textureWidth, textureHeight);
	if (texturePointer == NULL) return;

	// Create ID for texture
	glGenTextures(1, &texture[0]);

	// Set texture to work with
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Generate texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texturePointer);

	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
