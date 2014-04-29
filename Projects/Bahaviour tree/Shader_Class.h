//#pragma once
#ifndef ShaderClass_H_
#define ShaderClass_H_

#include "Application.h"
#include <glm/glm.hpp>
#include "Gizmos.h"
#include "Utilities.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <string>
#include <sstream>
#include <vector>

class ShaderHandle
{
public:
	ShaderHandle();
	virtual~ShaderHandle();
	//FILE TYPE ONLY 4 CHAR AFTER '.' (#shaders, .vert, .frag, .geom...)
	void Load(int _Count, ...);

	void Reload(GLFWwindow *_Window, int _Key);
	void Unload();


	unsigned int m_shader;
	
protected:

	void Load(std::vector<const char *> _PathList);

	std::vector <const char*>PathList;
	
	GLint vshader, cshader, eshader, gshader, fshader;

};

#endif