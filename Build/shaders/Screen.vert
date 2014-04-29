#version 150

in vec4 Position;
in vec2 UV;

out vec2 vUV;

uniform mat4 Projection = mat4(1);

void main(){
	vUV 		= UV;
	gl_Position = Projection * Position;
}

