#pragma once

#include <vector>

class Agent;

class Behaviour
{
public:
	Behaviour(){}
	~Behaviour(){}


	virtual bool Execute(Agent *_Agent) = 0;


};


class Composite : public Behaviour
{
public:
	Composite(){}
	virtual ~Composite(){}

	void addchild(Behaviour* _Behaviour) {Children.push_back(_Behaviour);}

protected:
	std::vector <Behaviour*> Children;
};


class Selector : public Composite//																			OR
{
public:

	Selector (){}
	virtual ~Selector(){}

	virtual bool Execute(Agent *_Agent)
	{
		for ( auto Behaviour : Children )
		{
			if( Behaviour->Execute(_Agent) == true)
				return true;
		}
		return false;
	}
};
class Sequence : public Composite//																			AND
{
public:

	Sequence (){}
	virtual ~Sequence(){}

	virtual bool Execute(Agent *_Agent)
	{
		for ( auto Behaviour : Children )
		{
			if( Behaviour->Execute(_Agent) == false)
				return false;
		}
		return true;
	}
};
