#version 330

layout (location = 0)in vec4 Position;
layout (location = 1)in vec2 UV;

out vec2 vUV;

uniform mat4 Projection = mat4(1);

void main(){
	vUV 		= UV;
	gl_Position = Projection * Position;
}

