#ifndef __NavMesh_H_
#define __NavMesh_H_

#include <Gizmos.h>
#include "Application.h"
#include <glm/glm.hpp>
#include <iostream>
#include "Agent.h"
#include "Behaviour.h"
#include "button.h"
#include "Shader_Class.h"
#include <FBXFile.h>
#include "NavNode.h"

class Agent;
class Team;
class Behaviour;
class Flag;
class NavMesh;

// Derived application class that wraps up all globals neatly
class NavMesh : public Application
{
public:

	NavMesh();
	virtual ~NavMesh();


	
	
	struct Compare
	{
		inline bool operator() (NavNode *_Node1, NavNode *_Node2)
		{
			NavNode *Temp1 = _Node1, *Temp2 = _Node2;
			if(Temp1->Score < 0)
			{
				//Temp1->Score *= -1;
			}
			return (Temp1->Score<Temp2->Score);
		}
	};

	struct GLData
	{
		unsigned int	vao, vbo, ibo;
	};


	virtual bool onCreate(int a_argc, char* a_argv[]);
	virtual void onUpdate(float a_deltaTime);
	virtual void onDraw();
	virtual void onDestroy();

	void	createOpenGLBuffers(FBXFile* a_fbx);
	void	cleanupOpenGLBuffers(FBXFile* a_fbx);

	void BuildNavMesh(FBXMeshNode *a_Mesh, std::vector<NavNode*> &a_Graph);


	std::vector <NavNode*> Path(glm::vec3 _StartPos, glm::vec3 _EndPos);
	void Pathtest(int _counter);

	NavNode* GiveScore(std::vector<NavNode*> a_Graph, glm::vec3 _Target);
	
	NavNode* ScoreCompare (NavNode *_NodeA, NavNode *_NodeB)
	{
		if(_NodeA == nullptr)
		{
			return _NodeB;
		}if(_NodeB == nullptr)
		{
			return _NodeA;
		}
		if(_NodeA->Score < _NodeB->Score)
			return _NodeA;
		else 
			return _NodeB;
	}

	FBXFile*	m_sponza;
	FBXFile*	m_navMesh;

	unsigned int	m_shader;

	glm::mat4	m_cameraMatrix;
	glm::mat4	m_projectionMatrix;

	int Start, End, count;
	bool down;

	std::vector <NavNode*> m_Graph, Open, Closed, PathList;


	NavNode *CurrentNode, *EndNode, *StartNode, *TestNode;

	Behaviour *Agenda, *Agenda2;
	Team *Red, *Blue;
	glm::mat4	m_screenProjectionMatrix;
	unsigned int uiRedUp, uiRedDown, uiBlueUp, uiBlueDown;

	std::vector<Flag*> Flags;

	ShaderHandle* Screen;
	Button *bRedUp,*bRedDown, *bBlueUp, *bBlueDown;

	int RedSize, BlueSize;
};

#endif // __NavMesh_H_