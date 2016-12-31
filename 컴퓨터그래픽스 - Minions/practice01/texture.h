#pragma once
#include <glut.h>
#include "image_loader.h"

class Texture {
public:
	/*
	* 0: yello
	* 1: blue_leather
	* 2: leather
	* 3: brown_eye
	* 4: gray
	*/
	GLuint _textureId[5];

	void loadTexture(Image* image, GLuint texture);
	void initRendering();
};

