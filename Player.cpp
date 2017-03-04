#include "Player.h"
Player::Player()
{
	// Delta
	deltaTime = 0.0f;
	lastFrame = 0.0f;
	timeElapsed = 0.0f;
	// Loading textures
	soldierTexture.init("Images/soldier.png", 320, 320);
	bulletTexture.init("Images/bullet.png", 320, 320);
	//Create shader program
	playerShader.init("Shaders/Player/vertex.vert", "Shaders/Player/fragment.frag");
	projectileShader.init("Shaders/Player/projectile.vert", "Shaders/Player/projectile.frag");
	// Orthogonal matrix
	projection = glm::ortho(0.0f, WIDTH, HEIGHT, 0.0f, -1.0f, 1.0f);
	// Default direction vector position
	playerVector = glm::vec2(0.0f, 1.0f);
	// Model matrix
	model = glm::translate(model, glm::vec3(0.5f * WIDTH, 0.5f * HEIGHT, 0.0f)); // Move origin of rotation to center of quad
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.0f)); // Then scale
	model = glm::translate(model, glm::vec3(-0.5f * WIDTH, -0.5f * HEIGHT, 0.0f)); // Move origin back
	//Vertex coordinates
	GLfloat leftX = (WIDTH - 320.0f) / 2.0f, topY = (HEIGHT - 320.0f) / 2;
	GLfloat rightX = leftX + 320.0f, bottomY = topY + 320.0f;

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
	for (int i = 0; i < 16; i++) vertex[i] = vert[i];
	for (int i = 0; i < 6; i++) indice[i] = ind[i];
}
Player::~Player()
{
}
GLvoid Player::updatePlayer()
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

	// Update sprite rotation
	updateVector();
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	soldierTexture.bindTexture();
	glUniform1i(playerShader.getUniform("myTexture"), 0);
	// Use program
	playerShader.useProgram();
	//Apply transformation matrices
	glUniformMatrix4fv(playerShader.getUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(playerShader.getUniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
	// Draw player
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Clear memory buffers
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	//Shoot projectile
	shootProjectile();
}
GLvoid Player::updateVector()
{
	// Find angle between two vectors
	mouseVector = glm::vec2(WIDTH / 2 - xPos, HEIGHT / 2 - yPos);
	mouseVector = glm::normalize(mouseVector);
	vectorAngle = glm::orientedAngle(playerVector, mouseVector);
	projectileAngle = glm::orientedAngle(glm::vec2(0.0f, 1.0f), mouseVector);
	// Update direction vector
	playerVector = mouseVector;
	// Prepare model matrix by changing origin
	model = glm::translate(model, glm::vec3(0.5f * WIDTH, 0.5f * HEIGHT, 0.0f));	// Move origin of rotation to center of quad
	if (vectorAngle != lastAngle)
	{
		model = glm::rotate(model, vectorAngle, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate
		lastAngle = vectorAngle;
	}
	model = glm::translate(model, glm::vec3(-0.5f * WIDTH, -0.5f * HEIGHT, 0.0f)); // Move origin back
	// Reset position
	if (xPos == WIDTH / 2 && yPos < HEIGHT / 2) resetVector(UP);
	else if (xPos == WIDTH / 2 && yPos > HEIGHT / 2) resetVector(DOWN);
	else if (yPos == HEIGHT / 2 && xPos < WIDTH / 2) resetVector(LEFT);
	else if (yPos == HEIGHT / 2 && xPos > WIDTH / 2) resetVector(RIGHT);
}
GLvoid Player::resetVector(Reset orientation)
{
	// Reset position in case of syncing issues
	glm::mat4 temp;
	model = temp;
	model = glm::translate(model, glm::vec3(0.5f * WIDTH, 0.5f * HEIGHT, 0.0f)); // Move origin of rotation to center of quad
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 1.0f)); // Then scale
	switch (orientation)
	{
	case DOWN:
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate
		break;
	case LEFT:
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate
		break;
	case RIGHT:
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate
		break;
	}
	model = glm::translate(model, glm::vec3(-0.5f * WIDTH, -0.5f * HEIGHT, 0.0f)); // Move origin back
}
GLvoid Player::shootProjectile()
{
	// Set frame time
	GLfloat currentFrame = (GLfloat)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	timeElapsed += deltaTime;

	// Fire bullet
	if (clicks[GLFW_MOUSE_BUTTON_LEFT] && timeElapsed > 0.125f)
	{
		Projectile *bullet = new Projectile(projectileAngle, &bulletTexture, &projectileShader);
		shotsFired.push_back(*bullet);
		delete bullet;
		timeElapsed = 0.0f;
	}
	// Render shots
	for (unsigned int i = 0; i < shotsFired.size(); i++)
	{
		shotsFired[i].draw();
		// Remove if out of bounds
		if (!shotsFired[i].testBounds((GLuint)WIDTH, (GLuint)HEIGHT))
		{
			shotsFired.erase(shotsFired.begin() + i);
		}
	}
}
