#include <glut.h>
#include "image_loader.h"
#include "texture.h"
#include <assert.h>

//Makes the image into a texture
void Texture::loadTexture(Image* image, GLuint texture) {
	glBindTexture(GL_TEXTURE_2D, texture); //Tell OpenGL which texture to edit
											 //Map the image to the texture

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image->width, image->height, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

}

void Texture::initRendering() {
	Image* images[5];

	glGenTextures(5, _textureId); //Make room for our texture
	
	Image::loadBMP("yellow.bmp", &images[0]);
	Image::loadBMP("blue_leather.bmp", &images[1]);
	Image::loadBMP("leather.bmp", &images[2]);
	Image::loadBMP("brown_eye.bmp", &images[3]);
	Image::loadBMP("gray.bmp", &images[4]);

	assert(images[0] != 0 || !"image null");
	assert(images[1] != 0 || !"image null");
	assert(images[2] != 0 || !"image null");
	assert(images[3] != 0 || !"image null");
	assert(images[4] != 0 || !"image null");

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	for (int i = 0; i < 5; i++) {
		loadTexture(images[i], _textureId[i]);
		delete images[i];
	}
	
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
}