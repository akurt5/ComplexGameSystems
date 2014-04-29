#ifndef __Bahaviour_tree_H_
#define __Bahaviour_tree_H_

#include "Application.h"
#include <glm/glm.hpp>
#include <vector>

class Agent;
class Team;
class Behaviour;

// Derived application class that wraps up all globals neatly
class Bahaviour_tree : public Application
{
public:

	Bahaviour_tree();
	virtual ~Bahaviour_tree(){}

	virtual bool onCreate(int a_argc, char* a_argv[]);
	virtual void onUpdate(float a_deltaTime);
	virtual void onDraw();
	virtual void onDestroy();

	Behaviour *Agenda, *Agenda2;
	Team *Red, *Blue;
	glm::mat4	m_cameraMatrix;
	glm::mat4	m_projectionMatrix;

	int RedSize, BlueSize;
};

#endif // __Bahaviour_tree_H_