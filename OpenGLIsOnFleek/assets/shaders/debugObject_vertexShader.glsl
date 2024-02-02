// Vertex shader
#version 420

uniform mat4 MVP;

in vec4 vColour;
in vec4 vPosition;

out vec4 fColour;

void main()
{
	gl_Position = MVP * vPosition;
	
	fColour = vColour;
}
