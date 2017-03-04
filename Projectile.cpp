#include "Projectile.h"
Projectile::Projectile(GLfloat angle, Texture *texture, Shader *shader)
{
	// Loading textures
	bulletTexture = texture;
	//Create shader program
	projectileShader = shader;

	//Vertex coordinates
	GLfloat leftX = (WIDTH - 320.0f) / 2.0f, topY = (HEIGHT - 320.0f) / 2;
	GLfloat rightX = leftX + 320.0f, bottomY = topY + 320.0f;

	topY -= 320.0f; bottomY -= 320.0f;
	GLfloat vert[16] = {
		//Vertices			//Texture Coordinates
		leftX, topY, 0.0f, 1.0f,		//Top left		-0
		rightX, topY, 1.0f, 1.0f,		//Top right		-1
		leftX, bottomY, 0.0f, 0.0f,		//Bottom left	-2
		rightX, bottomY, 1.0f, 0.0f		//Bottom right	-3
	};
	GLuint ind[6] = {
		0, 1, 2,
		2, 3, 1
	};
	originX = (560 - 240) / 2;
	originY = (140 + 180) / 2;
	for (int i = 0; i < 16; i++) vertex[i] = vert[i];
	for (int i = 0; i < 6; i++) indice[i] = ind[i];

	// Initialize angle
	projectileAngle = angle;

	// Orthogonal matrix
	projection = glm::ortho(0.0f, WIDTH, HEIGHT, 0.0f, -1.0f, 1.0f);

	// Model matrix
	model = glm::translate(model, glm::vec3(0.5f * WIDTH, 0.5f * HEIGHT, 0.0f)); // Move origin of rotation to center of quad
	model = glm::rotate(model, projectileAngle, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.0f)); // Then scale
	model = glm::translate(model, glm::vec3(-0.5f * WIDTH, -0.5f * HEIGHT, 0.0f)); // Move origin back

	// Speed
	speed = 1.25f;

	// Initilize iterarion
	iteration = 0;
}
Projectile::~Projectile()
{

}
GLvoid Projectile::draw()
{
	// Create buffer objects
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	// Player sprite
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indice), indice, GL_STATIC_DRAW);
	// Set shader locations and stride
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Calculate movement
	GLfloat translate = iteration * speed;
	calcPos();
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	bulletTexture->bindTexture();
	glUniform1i(projectileShader->getUniform("myTexture"), 0);
	// Use program
	projectileShader->useProgram();
	//Apply transformation matrices
	glUniform1f(projectileShader->getUniform("translate"), translate);
	glUniformMatrix4fv(projectileShader->getUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(projectileShader->getUniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
	// Draw player
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Clear memory buffers
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	iteration++;
}
GLvoid Projectile::calcPos()
{
	glm::vec4 position = projection * model * glm::vec4(originX, originY + iteration * speed, 0.0f, 1.0f);
	posX = -(position.x);
	posY = -(position.y);
}
GLboolean Projectile::testBounds(GLuint width, GLuint height)
{
	if (posX < -1 || posX > 1 || posY < -1 || posY > 1) return false;
	else return true;
}