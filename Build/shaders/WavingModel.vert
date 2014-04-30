#version 330   
 
in vec4 Position;
in vec4 Color;
in vec2 TexCoord1;

out vec2 vUV;
out vec4 vColor;
out vec4 vPosition;

uniform mat4 Model; 
uniform mat4 View;
uniform mat4 Projection;

uniform float time;
uniform float height;

void main() 
{
    vUV     = TexCoord1;
    vColor  = Color;
	float speed = 0.098f;
	float magnitude = (sin((time * 3.1415927f / ((528.0f) * speed))) * 0.01f + 0.40f) * height;
	vec4 P = Position;
	P.x += sin(( time * 3.1415927f / (15.0f * speed)) + (Position.z + Position.y) * (3.1415927f*2.0f/16.0f*3.0f)) * magnitude;
	P.y += sin(( time * 3.1415927f / (15.0f * speed)) + (Position.z + Position.x) * (3.1415927f*2.0f/16.0f*3.0f)) * magnitude;
	P.z += sin(( time * 3.1415927f / (15.0f * speed)) + (Position.y + Position.x) * (3.1415927f*2.0f/16.0f*3.0f)) * magnitude;
	//Passing to Fragment
	gl_Position = Projection * View * Model * P; 
	vPosition = Position;
}