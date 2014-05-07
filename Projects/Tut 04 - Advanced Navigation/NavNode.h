#pragma once

#include <glm/ext.hpp>


struct NavNode
	{
		glm::vec3 Position, Vertices[3];
		NavNode *edgeTarget[3], *Parent;
		float Score;
	};