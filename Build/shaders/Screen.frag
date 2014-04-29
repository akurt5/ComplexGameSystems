#version 150

in vec2 vUV;

out vec4 outColour;

uniform sampler2D Texture;

void main(){
	outColour = texture(Texture,vUV);
	if (outColour.a < 0.05f) { discard; }
}

