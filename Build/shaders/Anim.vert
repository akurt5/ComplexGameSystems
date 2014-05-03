#version 330   
 
//--------------------------    
// sent as vertex attributes
layout( location = 0 ) in vec4 Position;     
layout( location = 1 ) in vec4 Color;     
layout( location = 2 ) in vec4 Normal;     
layout( location = 3 ) in vec4 Tangent;     
layout( location = 4 ) in vec4 Binormal;
layout( location = 5 ) in vec4 Indices;
layout( location = 6 ) in vec4 Weights;
layout( location = 7 ) in vec2 Texcoord1;	
layout( location = 8 ) in vec2 Texcoord2;	

//--------------------------  

//--------------------------    
// values sent to the fragment shader
out vec2 vUV;
out vec4 vColor;	
//--------------------------    


uniform mat4 Model; 
uniform mat4 View;
uniform mat4 Projection;

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

    vUV     = Texcoord1;
    vColor  = Color;
    gl_Position = Projection * View * Model * P; 
}