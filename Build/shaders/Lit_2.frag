#version 150

in vec4 normal;
in vec4 color;
in vec2 texcoord1;

void main()
{

	vec3 N = normalize( normal ).xyz;
	// fake light shining "down"
	vec3 L = vec3( 0, 2, 0 );
	// lambert term
	float d = max( 0, dot( N, L ) );
	gl_FragColor = vec4( d, d, d, 1 );
}