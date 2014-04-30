#version 150

in vec4 Position;
in vec4 Normal;

out vec3 position;
out vec3 normal;

uniform mat4 view;
uniform mat4 projection;

in vec3 Velocity;
in float Lifetime;
in float Lifespan;

out vec3 position;
out vec3 velocity;
out float lifetime;
out float lifespan;

uniform float time;
uniform float deltaTime;

uniform float lifeMin;
uniform float lifeMax;

uniform vec3 emitterPosition;

const float INVERSE_MAX_UINT = 1.0f / 4294967295.0f;

void main()
{
	position = Position.xyz;
	normal = Normal.xyz;
	gl_Position = projection * view * Position;
}