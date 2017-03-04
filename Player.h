#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// Other Libs
#include <SOIL/SOIL.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
// Includes
#include <vector>
#include "Global.h"
#include "Texture.h"
#include "Shader.h"
#include "Projectile.h"
class Player
{
	GLfloat deltaTime, lastFrame, timeElapsed;
	GLuint VBO, VAO, EBO;
	Texture soldierTexture, bulletTexture;
	Shader playerShader, projectileShader;
	GLfloat vertex[16];
	GLuint indice[6];
	glm::vec2 playerVector, mouseVector;
	glm::mat4 projection, model;
	GLfloat vectorAngle, lastAngle, projectileAngle;
	enum Reset { UP, DOWN, LEFT, RIGHT };
	std::vector<Projectile> shotsFired;
	GLvoid updateVector();
public:
	Player();
	~Player();
	GLvoid updatePlayer();
	GLvoid resetVector(Reset orientation);
	GLvoid shootProjectile();
};
#endif