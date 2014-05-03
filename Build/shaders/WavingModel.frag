#version 330

in vec2 vUV;
in vec4 vColor;
in vec4 vPosition;

out vec4 outColor;

uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;
uniform sampler2D NormalTexture;

void main() 
{ 
	outColor = texture2D( DiffuseTexture, vUV.xy ) * vColor;
	outColor.r = vPosition.x;
	outColor.g = vPosition.y;
	outColor.b = vPosition.z;
	outColor.a = 1.0f;
}