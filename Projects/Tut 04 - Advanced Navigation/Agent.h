#pragma once

#include "Behaviour.h"
#include "Utilities.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <vector>
#include "NavNode.h"

class Flag;

class Agent
{
public:
 //																			add '...' to allow multiple enemies, or add a list. List is probably better.
	Agent(){Timer = 0;Attack = false;}
	virtual ~Agent (){}

	const glm::vec3& GetPos () const {return Position;}

	const glm::vec3 GetTarget () const{return Target;}
	void CalcEnemy(std::vector <Agent*> _Team){Enemies = _Team;}
	void SetPos (const glm::vec3& _Pos) { Velocity += _Pos;}
	void SetTarget (glm::vec3 _Target) { Target = _Target;}
	glm::vec3 Move(float t)
	{
		glm::vec3 Dest = GetTarget();
		glm::vec3 Pos = Position;
		if (Path.size() >= 2)
		{
			Dest = Path[1]->Position;
			float t1 = 1-t;			
			Dest = t1 * ((t1 * Position) + (t * Path[0]->Position)) + t * ((t1 * Path[0]->Position) + (t * Path[1]->Position));
		}
	
			//	for now i will just normalise there
			//	Interpolate linearly to find the third vector // use a number greater than 1 as time // then spline
		 
		float m = glm::length(Dest);
		Dest.x /= m;
		Dest.y /= m;
		Dest.z /= m;
		return Dest;
	}
	void SetBehaviour(Behaviour* _Behaviour) {Behave = _Behaviour;}

	void update(float _DeltaTime)
	{
		if(!(bool)Timer){Timer = 100000 * Utility::getDeltaTime(); Attack =!Attack/*(bool)(rand()%2)*/;}
		else{Timer--;}
		
		if (Behave != nullptr)
			Behave->Execute(this);
		
		Position += (Velocity * _DeltaTime);
		Velocity = glm::vec3(0);
	}

	Behaviour* Behave;
	std::vector <NavNode*> Path;
	std::vector <Agent*> Enemies;
	std::vector <Flag*> Flags;

	bool Attack;
	int Timer;
	glm::vec3 Position, Target, Velocity;
};

class Team
{
public:
	Team(){}
	virtual ~Team(){}
	void AddMember(){Members.emplace_back(new Agent());}
	void DelMember(){Members.erase(Members.begin());}
	
	
	std::vector <Agent*> Members;

};

class Flag
{
public:
	Flag(){fRed , fGreen, fBlue = 0;}
	virtual ~Flag(){}
	glm::vec3 GetPos(){return Position;}
	void Update(){Gizmos::addAABBFilled(Position, glm::vec3(0.1f), glm::vec4(fRed, fGreen, fBlue, 1));}
	
	Team GetController(Team _Red, Team _Blue){if(fRed>fBlue){return _Red;}else{return _Blue;}}

	float fRed, fGreen, fBlue;

	glm::vec3 Position;
};