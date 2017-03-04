#version 330 core
layout (location = 0) in vec4 position;

out vec2 TexCoord;
uniform mat4 model;
uniform mat4 projection;
uniform float translate;

void main()
{
	gl_Position = projection * model * vec4(position.x, position.y - translate, 0.0f, 1.0f);
	// We swap the y-axis by substracing our coordinates from 1.
	// This is done because most images have the top y-axis inversed with OpenGL's top y-axis.
	// TexCoord = texCoord;
	TexCoord = vec2(position.z, 1.0f - position.w);
}