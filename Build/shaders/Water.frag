#version 150

in vec4 vColour;
in vec4 position;

out vec4 FragColor;

void main() {
	FragColor = vColour;	
	FragColor.r += position.y;
	FragColor.g += position.y * 2;
	FragColor.b += position.y * 3;
}