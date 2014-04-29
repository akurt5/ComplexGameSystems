#pragma once

#include "Behaviour.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <vector>


class Agent
{
public:
	//Agent(Agent* _Other = nullptr){Enemy = _Other;} //																			add '...' to allow multiple enemies, or add a list. List is probably better.
	Agent(){Timer = 0;Attack = false;}
	virtual ~Agent (){}

	const glm::vec3& GetPos () const {return Position;}
	//const glm::vec3& GetTarget (glm::vec3 Pos1, glm::vec3 _Pos2) const{return Target;}
	const glm::vec3& GetTarget () const{return Target;}
	void CalcEnemy(std::vector <Agent*> _Team){Enemies = _Team;}
	void SetPos (const glm::vec3& _Pos) { Position = _Pos;}
	void SetTarget (const glm::vec3& _Target) { Target = _Target;}

	void SetBehaviour(Behaviour* _Behaviour) {Behave = _Behaviour;}

	virtual void update(float _DeltaTime)
	{
		if(!Timer){(int)Timer = 10000;Attack = (rand()%2);}
		else{Timer--;}
		//else{Attack = !Attack;Timer--;}
		Velocity = glm::vec3(0);
		if (Behave != nullptr)
			Behave->Execute(this);
		
		(Position += Velocity) * _DeltaTime;
	}

	Behaviour* Behave;
	
	std::vector <Agent*> Enemies;
	bool Attack;
	int Timer;
	glm::vec3 Position, Target, Velocity;
};

class Team
{
public:
	Team(){}
	virtual ~Team(){}
	void AddMember(Agent *_Agent){Members.emplace_back(_Agent);}
	void DelMember(Agent *_Agent){Members.erase(Members.begin());}
	
	void SetFlagPos(glm::vec3 _Pos){Flag = _Pos;}
	const glm::vec3 GetFlagPos(){return Flag;}
	void SetFlagColour(glm::vec4 _Colour = glm::vec4(1, 1, 1, 1)){FlagColour = _Colour;}
	const glm::vec4 GetFlagColour(){return FlagColour;}
	
	std::vector <Agent*> Members;
	//int Score;
	glm::vec3 Flag;
	glm::vec4 FlagColour;

};