#version 330

layout( location = 0 ) in vec4 Position;
layout( location = 1 ) in vec4 Color;
layout( location = 2 ) in vec4 Normal;
//layout( location = 3 ) in vec4 Tangent;
//layout( location = 4 ) in vec4 Binormal;
layout( location = 5 ) in vec4 Indices;
layout( location = 6 ) in vec4 Weights;
layout( location = 7 ) in vec2 Texcoord1;
//layout( location = 8 ) in vec2 Texcoord2;

out vec3 position;
out vec3 normal;
out vec2 texcoord1;
out vec4 color;

uniform mat4 Model; 
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Global;

uniform bool Animate;
uniform bool Texture;

const int MAX_BONES = 128;
uniform mat4 Bones[MAX_BONES];

void main()
{

	position = Position.xyz;
	normal = Normal.xyz;
	color = Color;

	if(Texture == true){
		texcoord1 = Texcoord1;
	}

	if(Animate == true){
		ivec4 Index = ivec4(Indices);

		// sample bones and blend up to 4
		vec4 P  = Bones[ Index.x ] * Position * Weights.x;
			 P += Bones[ Index.y ] * Position * Weights.y;
			 P += Bones[ Index.z ] * Position * Weights.z;
			 P += Bones[ Index.w ] * Position * Weights.w;

			 gl_Position = Projection * View * Global * Position;
	}
	else{
		gl_Position = Projection * View * Model * Position; 
	}

	
	
}
