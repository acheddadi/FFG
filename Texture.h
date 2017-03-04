#ifndef TEXTURE_H
#define TEXTURE_H
#include <iostream>
#include <GL\glew.h>
#include <SOIL\SOIL.h>
class Texture
{
	GLuint id;
	GLint width, height;
public:
	Texture();
	GLvoid init(const GLchar *filename, GLint width, GLint height);
	GLvoid bindTexture();
};
#endif