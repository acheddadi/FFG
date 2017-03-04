#include "Shader.h"
Shader::Shader()
{
	std::cout << "Shader object created." << std::endl;
}
GLvoid Shader::init(const GLchar *vertexSource, const GLchar *fragmentSource)
{
	vertexSourceFile = readShaderFile(vertexSource);
	fragmentSourceFile = readShaderFile(fragmentSource);
	id = createShaderProgram(vertexSourceFile, fragmentSourceFile);
}
GLvoid Shader::compileCheck(GLuint &shader)
{
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}
GLvoid Shader::linkCheck(GLuint &program)
{
	GLint success;
	glGetShaderiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
}
GLuint Shader::createShaderProgram(const GLchar *vertex, const GLchar *fragment)
{
	GLuint vertexShader; //Create shader object referenced by an ID
	vertexShader = glCreateShader(GL_VERTEX_SHADER); //Provide the type of shader you want to create.
	glShaderSource(vertexShader, 1, &vertex, NULL); //We attach the shader source code to the shader object and compile it.
	//test if compilation was a success.
	glCompileShader(vertexShader);
	compileCheck(vertexShader);

	GLuint fragmentShader; //Create shader object referenced by an ID
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment, NULL); //Attach shader source.
	//test compilation
	glCompileShader(fragmentShader);
	compileCheck(fragmentShader);

	//Create shader program that ecompasses both shaders.
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader); //Attach shaders.
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); //Link shaders together.
	linkCheck(shaderProgram);
	glDeleteShader(vertexShader); //Don't forget to delete shader after it's been created.
	glDeleteShader(fragmentShader);
	std::cout << "Shader program created." << std::endl;
	return shaderProgram;
}
const GLchar* Shader::readShaderFile(const char* filePath)
{
	const GLchar *shaderFileBuffer = NULL;

	std::ifstream inSdrFileStream(filePath);
	if (inSdrFileStream)
	{
		// Get length of shader file by seeking and telling (offset of 0)
		inSdrFileStream.seekg(0, inSdrFileStream.end);
		unsigned long fileLength = (unsigned long)inSdrFileStream.tellg() + 1;
		inSdrFileStream.seekg(0, inSdrFileStream.beg);

		std::cout << "Shader File: " << filePath << std::endl;
		std::cout << "Reading " << fileLength << " chars...\n";

		// Read shader file into a memory buffer (array)
		char *inputFileBuffer = new char[fileLength];
		memset(inputFileBuffer, 0, fileLength);
		inSdrFileStream.read(inputFileBuffer, fileLength);
		inputFileBuffer[fileLength - 1] = 0;

		// Close file and print status
		if (inputFileBuffer)
		{
			std::cout << "---------------------------------\n";
			std::cout << inputFileBuffer << std::endl;
			std::cout << "---------------------------------\n";
			std::cout << "... Read successfully.\n\n";
			std::cout << std::endl;
			inSdrFileStream.close();
		}
		else
		{
			std::cout << "... Error: Only " << inSdrFileStream.gcount() << " could be read!\n";
			std::cout << std::endl;
			inSdrFileStream.close();
			delete[] inputFileBuffer;
			return NULL;
		}

		// Hand over file contents to a const pointer
		shaderFileBuffer = inputFileBuffer;
		inputFileBuffer = NULL;
	}
	else
	{
		std::cout << "Shader File: " << filePath << "\n";
		std::cout << "Error. Not found!" << std::endl;
		std::cout << std::endl;
		return NULL;
	}

	return shaderFileBuffer;
}
GLvoid Shader::useProgram()
{
	glUseProgram(id);
}
GLint Shader::getUniform(const GLchar *uniform)
{
	return glGetUniformLocation(id, uniform);
}
