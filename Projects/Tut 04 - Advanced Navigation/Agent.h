#pragma once

#include "Behaviour.h"
#include <glm\glm.hpp>

class Agent
{
public:
	Agent(){}
	virtual ~Agent (){}

	const glm::vec3& GetPos () const {return Position;}
	const glm::vec3& GetTarget () const{return Target;}

	void SetPos (const glm::vec3& _Pos) { Position = _Pos;}
	void SetTarget (const glm::vec3& _Target) { Target = _Target;}

	void SetBehaviour(Behaviour* _Behaviour) {Behave = _Behaviour;}

	virtual void update(float _DeltaTime)
	{
		if (Behave != nullptr)
			Behave->Execute(this);
	}
private:
	Behaviour* Behave;

	glm::vec3 Position, Target;
};