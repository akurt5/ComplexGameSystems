#version 330

layout( location = 0 ) in vec4 Position;
layout( location = 7 ) in vec2 Texcoord1;
layout( location = 5 ) in vec4 Indices;
layout( location = 6 ) in vec4 Weights;

//out vec3 position;
//out vec3 normal;
out vec2 texcoord1;
//out vec4 color;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Global;

// we need to give our bone array a limit
const int MAX_BONES = 128;
uniform mat4 bones[MAX_BONES];

void main()
{
	// cast the indices to integer's so they can index an array
	ivec4 index = ivec4(Indices);
	// sample bones and blend up to 4
	vec4 P = bones[ index.x ] * Position * Weights.x;
	P += bones[ index.y ] * Position * Weights.y;
	P += bones[ index.z ] * Position * Weights.z;
	P += bones[ index.w ] * Position * Weights.w;
	
	texcoord1 = Texcoord1;
	gl_Position = Projection * View * Global * P;
}