#ifndef SHADER_H
#define SHADER_H
#include <iostream>
#include <fstream>
#include <GL/glew.h>
class Shader
{
	const GLchar *vertexSourceFile;
	const GLchar *fragmentSourceFile;
	GLuint id;
	GLvoid compileCheck(GLuint &shader);
	GLvoid linkCheck(GLuint &program);
	GLuint createShaderProgram(const GLchar *vertex, const GLchar *fragment);
	const GLchar* readShaderFile(const char* filePath);
public:
	Shader();
	GLvoid init(const GLchar *vertexSource, const GLchar *fragmentSource);
	GLvoid useProgram();
	GLint getUniform(const GLchar *uniform);
};
#endif
