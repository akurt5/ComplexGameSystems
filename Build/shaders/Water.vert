#version 150

in vec4 Position;
in vec4 Colour;

out vec4 vColour;
out vec4 position;

uniform mat4 ProjectionView;
uniform float time;
uniform float height;

void main() { 
	float speed = 0.098f;
	float magnitude = (sin((time * 3.14159265f / ((528.0f) * speed))) * 0.01f + 0.40f) * height;
	vec4 P = Position;
	P.y += sin(( time * 3.1415927 / (15.0 * speed)) + (Position.z + Position.x) * (3.1415927*2/16*3)) * magnitude;
	//Passing to Fragment
	position = P;
	vColour = vec4(0.0f,0.0f,0.8f,0.2f);
	//OpenGL stuff
	gl_Position = ProjectionView * P;
}