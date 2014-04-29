#ifndef __NavMesh_H_
#define __NavMesh_H_

#include <Gizmos.h>
#include "Application.h"
#include <glm/glm.hpp>
#include <iostream>

#include <FBXFile.h>


// Derived application class that wraps up all globals neatly
class NavMesh : public Application
{
public:

	NavMesh();
	virtual ~NavMesh();

protected:

	virtual bool onCreate(int a_argc, char* a_argv[]);
	virtual void onUpdate(float a_deltaTime);
	virtual void onDraw();
	virtual void onDestroy();

	struct NavNode
	{
		glm::vec3 Position, Vertices[3];
		NavNode *edgeTarget[3], *Parent;
		int Score;

	};

	std::vector <NavNode*> m_Graph, Open, Closed, PathList;

struct Compare
{
	inline bool operator() (NavNode *_Node1, NavNode *_Node2)
	{
		return (_Node1->Score<_Node2->Score);
	}


};
	//NavNode* GetCurrentNode(glm::vec3 _Pos, std::vector<NavNode*> &_List);
	std::vector <NavNode*> Path(glm::vec3 _StartPos, glm::vec3 _TargetPos);
	void Pathtest(int _counter);

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

	void BuildNavMesh(FBXMeshNode *a_Mesh, std::vector<NavNode*> &a_Graph, glm::vec3 _StartPos, glm::vec3 _EndPos);


	void	createOpenGLBuffers(FBXFile* a_fbx);
	void	cleanupOpenGLBuffers(FBXFile* a_fbx);

	struct GLData
	{
		unsigned int	vao, vbo, ibo;
	};

	FBXFile*	m_sponza;
	FBXFile*	m_navMesh;

	unsigned int	m_shader;

	glm::mat4	m_cameraMatrix;
	glm::mat4	m_projectionMatrix;

	int Start, End, count;
	bool down;

	NavNode *CurrentNode, *EndNode, *StartNode;
};

#endif // __NavMesh_H_