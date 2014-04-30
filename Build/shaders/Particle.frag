#version 150

in vec4 colour;
in vec2 uv;

uniform sampler2D DiffuseTexture;

void main()
{
	gl_FragColor = texture2D( DiffuseTexture, uv  ) * colour;
}