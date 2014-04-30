#version 150

in vec4 Position;
in vec4 Colour;
in vec2 Uv;

out vec4 colour;
out vec2 uv;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	uv = Uv;
	colour = Colour;
	gl_Position = projection * view * Position;
}