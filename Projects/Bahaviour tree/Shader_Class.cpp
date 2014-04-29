#include "Shader_Class.h"


ShaderHandle::ShaderHandle(){}
ShaderHandle::~ShaderHandle(){}


void ShaderHandle::Load(int _Count, ...)
{
	char *a;
	va_list VariablePathList;
	va_start (VariablePathList, _Count);
	
	
		vshader = 0;
		cshader = 0;
		eshader = 0;
		gshader = 0;
		fshader = 0;

	for(int i=0; i!=_Count; i++){
	
		a = va_arg(VariablePathList, char *);
		
		std::string sName(a);
		
		std::string sType;
		//add the file type to sType
		sType.append(sName.end()-4, sName.end());
		//all shaders equal 0
		//allows us to use only what we want
		
		
		std::stringstream ssPath;

		ssPath<<"../../Build/shaders/"<<sName;
		sName = ssPath.str();

			//switch to identify shader type
			  if(!sType.compare("vert")){

			vshader = Utility::loadShader(sName.c_str(), GL_VERTEX_SHADER);

		}else if(!sType.compare("cont")){

			cshader = Utility::loadShader(sName.c_str(), GL_TESS_CONTROL_SHADER);

		}else if(!sType.compare("eval")){

			eshader = Utility::loadShader(sName.c_str(), GL_TESS_EVALUATION_SHADER);

		}else if(!sType.compare("geom")){

			gshader = Utility::loadShader(sName.c_str(), GL_GEOMETRY_SHADER);

		}else if(!sType.compare("frag")){
			
			fshader = Utility::loadShader(sName.c_str(), GL_FRAGMENT_SHADER);

		}else{
			printf("Cannot define type of shader     ", sName.c_str(), '\n');
		}
		const char* FullPath = sName.c_str();
		PathList.emplace_back(FullPath);
		printf(PathList[i]);printf("\n");

	}
	va_end (VariablePathList);
		
	const char* aszInputs[] =  { "Position", "Color", "Normal", "Tangent", "Binormal", "Indices", "Weights", "Texcoord1", "Texcoord2" };
	const char* aszOutputs[] = { "outColour" };
	
	m_shader = Utility::createProgram( vshader, cshader, eshader, gshader, fshader);

	// free our shader once we built our program
	glDeleteShader( vshader );
	glDeleteShader( cshader );
	glDeleteShader( eshader );
	glDeleteShader( gshader );
	glDeleteShader( fshader );
}
void ShaderHandle::Load(std::vector<const char*> _PathList)
{
	for(int i=0; i!=_PathList.size(); ++i){
	
		
		std::string sName(_PathList[i]);

		printf(sName.data());
		
		std::string sType;
		//add the file type to sType
		sType.append(sName.end()-4, sName.end());
		//all shaders equal 0
		//allows us to use only what we want
		vshader = 0;
		cshader = 0;
		eshader = 0;
		gshader = 0;
		fshader = 0;
		
		std::stringstream ssPath;

		ssPath<<"../../Build/shaders/"<<sName;
		sName = ssPath.str();
	
			//switch to identify shader type
			  if(!sType.compare("vert")){

			vshader = Utility::loadShader(sName.c_str(), GL_VERTEX_SHADER);

		}else if(!sType.compare("cont")){

			cshader = Utility::loadShader(sName.c_str(), GL_TESS_CONTROL_SHADER);

		}else if(!sType.compare("eval")){

			eshader = Utility::loadShader(sName.c_str(), GL_TESS_EVALUATION_SHADER);

		}else if(!sType.compare("geom")){

			gshader = Utility::loadShader(sName.c_str(), GL_GEOMETRY_SHADER);

		}else if(!sType.compare("frag")){
			
			fshader = Utility::loadShader(sName.c_str(), GL_FRAGMENT_SHADER);

		}else{
			printf("Cannot define type of shader     ", sName.c_str(), '\n');
		}
			PathList.push_back(sName.c_str());
			printf(PathList[i]);

	}
		
	const char* aszInputs[] =  { "Position", "Color", "Normal", "Tangent", "Binormal", "Indices", "Weights", "Texcoord1", "Texcoord2" };
	const char* aszOutputs[] = { "outColour" };
	
	m_shader = Utility::createProgram( vshader, cshader, eshader, gshader, fshader);

	// free our shader once we built our program
	glDeleteShader( vshader );
	glDeleteShader( cshader );
	glDeleteShader( eshader );
	glDeleteShader( gshader );
	glDeleteShader( fshader );
}
void ShaderHandle::Reload(GLFWwindow *_Window, int _Key)
{
	if(glfwGetKey(_Window, _Key))
	{
		Load(PathList);
		printf("Reloading Shaders... \n");
	}
}
void ShaderHandle::Unload()
{
	glDeleteShader(m_shader);

}
	
