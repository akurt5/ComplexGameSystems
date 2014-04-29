#include "Button.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include "Utilities.h"

Button::Button(glm::vec2 v2_Position,glm::vec2 v2_Size){
	bActivated = false;

	verts[0].position	= glm::vec4( v2_Position.x				, v2_Position.y + v2_Size.y	,0.0f,1.0f); 
	verts[0].uv 		= glm::vec2(0.0f ,1.0f);
	verts[1].position	= glm::vec4( v2_Position.x + v2_Size.x	, v2_Position.y + v2_Size.y	,0.0f,1.0f); 
	verts[1].uv 		= glm::vec2(1.0f ,1.0f);
	verts[2].position	= glm::vec4( v2_Position.x + v2_Size.x	, v2_Position.y 			,0.0f,1.0f); 
	verts[2].uv 		= glm::vec2(1.0f ,0.0f);
	verts[3].position	= glm::vec4( v2_Position.x				, v2_Position.y 			,0.0f,1.0f); 
	verts[3].uv 		= glm::vec2(0.0f ,0.0f);

	unsigned int ind[6] = {
		0,1,2,0,2,3,
	};

	// create opengl buffers
	glGenVertexArrays(1, &uiVAO);
	glBindVertexArray(uiVAO);

	glGenBuffers(1, &uiVBO);
	glGenBuffers(1, &uiIBO);	

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(sPosUV), verts, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), ind , GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(sPosUV), (void*) + 0);
	glEnableVertexAttribArray(1); // UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(sPosUV), (void*) + sizeof(glm::vec4));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

Button::~Button(){
	glDeleteVertexArrays(1,&uiVAO);
	glDeleteBuffers(1, &uiVBO);
	glDeleteBuffers(1, &uiIBO);	
}

void Button::Draw() {

	// Bind the texture
	// Bind the Ortho Matrix4

	glBindVertexArray( uiVAO );
	glDrawElements(GL_TRIANGLES, 6 , GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

bool Button::IsActivated(){
	return bActivated;
}

void Button::Update() {

	glm::dvec2 v2MousePosition;
	glfwSetCursorPos(glfwGetCurrentContext(), v2MousePosition.x, v2MousePosition.y);
	// GET MOUSE HERE USING ABOVE

	glm::vec2 p1 = (verts[0].position.xy);
	glm::vec2 p2 = (verts[3].position.xy);

	if (v2MousePosition.x > p1.x && 
		v2MousePosition.y > p1.y && 
		v2MousePosition.x < p2.x && 
		v2MousePosition.y < p2.y ){
			if(glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_1))
			{
				bActivated = true;
			}

	}else{
		bActivated = false;
	}
}
