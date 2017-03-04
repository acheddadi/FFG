#ifndef PROJECTILE_H
#define PROJECTILE_H
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
// Includes
#include "Global.h"
#include "Texture.h"
#include "Shader.h"
class Projectile
{
	GLuint VBO, VAO, EBO;
	Texture *bulletTexture;
	Shader *projectileShader;
	GLfloat vertex[16];
	GLuint indice[6];
	GLfloat projectileAngle;
	glm::mat4 projection, model;
	GLfloat speed;
	GLfloat posX, posY, originX, originY;
	GLuint iteration;
	GLvoid calcPos();
public:
	Projectile(GLfloat angle, Texture *texture, Shader *shader);
	~Projectile();
	GLvoid draw();
	GLboolean testBounds(GLuint width, GLuint height);
};
#endif
