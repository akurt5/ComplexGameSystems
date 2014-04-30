#version 330   
 
//--------------------------    
// sent as vertex attributes
in vec4 Position;
in vec3 Normal;
in vec3 Tangent;
in vec3 BiNormal;
in vec2 TexCoord1;	
//--------------------------  

//--------------------------    
// values sent to the fragment shader
out vec2 vTexCoord1;
out vec3 vNormal; 
out vec3 vLightDir;	
//--------------------------    


uniform mat4 Model; 
uniform mat4 View;
uniform mat4 Projection;

void main() 
{
    vUV     = TexCoord1;
    vColor  = Color;
    
    gl_Position = Projection * View * Model * Position; 
}