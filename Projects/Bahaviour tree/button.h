#pragma once 

#include <glm/ext.hpp>

struct sPosUV{
	glm::vec4 position;
	glm::vec2 uv;
};

class Button{
public:
	Button(glm::vec2 v2_Position,glm::vec2 v2_Size);
	~Button();
	void Draw();
	void Update();
	bool IsActivated();
protected:
	sPosUV verts[4];
	bool bActivated;
	unsigned int uiVAO, uiVBO, uiIBO;	

};


