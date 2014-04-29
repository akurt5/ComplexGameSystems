#include "Bahaviour_tree.h"
#include "Gizmos.h"
#include "Utilities.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include "Button.h"

#include "Agent.h"



class WithinRange : public Behaviour
{
public:

	WithinRange(float _Range) : Range2 (_Range*_Range){}
	virtual ~WithinRange(){}
	virtual bool Execute(Agent *_Agent)
	{
		float dist2;
		if(_Agent->Attack)
		{
			Agent *TargetEnemy;
			for ( auto EnemyCurrent : _Agent->Enemies)
			{
				TargetEnemy = EnemyCurrent;
				float Len = 0, tLen;
				
				tLen = glm::length(_Agent->GetPos()) - glm::length(EnemyCurrent->GetPos());
				if(tLen < 0){tLen *= -1;}
				if(tLen > Len){Len = tLen;TargetEnemy = EnemyCurrent;}
			}
			dist2 = glm::distance2(_Agent->GetPos(), TargetEnemy->GetPos());
		}
		else
		{
			dist2 = glm::distance2(_Agent->GetPos(), _Agent->GetTarget());
		}
		
		if (dist2 < Range2)
			return true;
		return false;
	}

	float Range2;
};
class RandomiseTarget : public Behaviour
{
public:
	RandomiseTarget(float _Radius) : Radius(_Radius){}
	virtual ~RandomiseTarget(){}
	virtual bool Execute(Agent *_Agent)
	{
		glm::vec3 Target(0);

		Target.xz = glm::circularRand(Radius);
		if(_Agent->Attack)
		{
			Agent *TargetEnemy;
			for ( auto EnemyCurrent : _Agent->Enemies)
			{
				float Len = 0, tLen;
				
				tLen = glm::length(_Agent->GetPos()) - glm::length(EnemyCurrent->GetPos());
				if(tLen < 0){tLen *= -1;}
				if(tLen > Len){Len = tLen;TargetEnemy = EnemyCurrent;}
			}
			TargetEnemy->SetPos(Target);
		}
		else
		{
			_Agent->SetTarget(Target);
		}
		return true;
	}
	float Radius;
};
class SeekTarget : public Behaviour
{
public:
	SeekTarget(float _Speed) : Speed(_Speed){}
	virtual ~SeekTarget(){}

	virtual bool  Execute(Agent *_Agent)
	{
		glm::vec3 Pos, Dir;
		if(_Agent->Attack)
		{
			Agent *TargetEnemy;
			for ( auto EnemyCurrent : _Agent->Enemies)
			{
				float Len = 0, tLen;
				
				tLen = glm::length(_Agent->GetPos()) - glm::length(EnemyCurrent->GetPos());
				if(tLen < 0){tLen *= -1;}
				if(tLen > Len){Len = tLen;TargetEnemy = EnemyCurrent;}
			}
			Pos = _Agent->GetPos();
			Dir = glm::normalize(TargetEnemy->GetPos() - Pos + TargetEnemy->Velocity);
		}
		else
		{
			Pos = _Agent->GetPos();
			Dir = glm::normalize(_Agent->GetTarget() - Pos);
		}
		_Agent->Velocity +=(Dir * Speed* Utility::getDeltaTime());
		
		return true;
	}
	float Speed;
};

class FleeTarget : public Behaviour
{
public:
	FleeTarget(float _Speed) : Speed(_Speed){}
	virtual ~FleeTarget(){}

	virtual bool  Execute(Agent *_Agent)
	{
		glm::vec3 Pos, Dir;
		Agent *TargetEnemy;
		float Len = 0, tLen;
		for ( auto EnemyCurrent : _Agent->Enemies)
			{
				tLen = glm::length(_Agent->GetPos()) - glm::length(EnemyCurrent->GetPos());
				if(tLen < 0){tLen *= -1;}
				if(tLen > Len){Len = tLen;TargetEnemy = EnemyCurrent;}
			}
		if(Len < 2.5)
		{
		Pos = _Agent->GetPos();
		Dir = glm::normalize(TargetEnemy->GetPos() - Pos);
		_Agent->Velocity -=(Dir * Speed* Utility::getDeltaTime());
		
		return true;
		}
		else 
			return false;
	}
	float Speed;
};


#define DEFAULT_SCREENWIDTH 1280
#define DEFAULT_SCREENHEIGHT 720

Bahaviour_tree::Bahaviour_tree()
{

}

bool Bahaviour_tree::onCreate(int a_argc, char* a_argv[]) 
{
	// initialise the Gizmos helper class
	Gizmos::create();

	// create a world-space matrix for a camera
	m_cameraMatrix = glm::inverse( glm::lookAt(glm::vec3(10,10,10),glm::vec3(0,0,0), glm::vec3(0,1,0)) );
	
	// create a perspective projection matrix with a 90 degree field-of-view and widescreen aspect ratio
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, DEFAULT_SCREENWIDTH/(float)DEFAULT_SCREENHEIGHT, 0.1f, 1000.0f);

	// set the clear colour and enable depth testing and backface culling
	glClearColor(0.25f,0.25f,0.25f,1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	Screen = new ShaderHandle();
	Screen->Load(2, "Screen.vert", "Screen.frag");

	Behaviour* Seek = new SeekTarget(3);
	Behaviour* Rand = new RandomiseTarget(10);
	Behaviour* Within = new WithinRange(0.5f);
	Behaviour* Flee = new FleeTarget(0.5f);

	//																			IF	not within range	THEN	seek	ELSE	randomise target
	//																			IF	not attack			THEN	Above
	
	Sequence* Seq = new Sequence();
	Seq->addchild(Within);
	Seq->addchild(Rand);

	Sequence* Seq2 = new Sequence();
	Seq2->addchild(Flee);
	Seq2->addchild(Seek);
	Selector* Root = new Selector();
	Root->addchild(Seq);
	Root->addchild(Seq2);
	
	Agenda = Root;

	RedSize = 3;
	BlueSize = 3;

	//glm::vec4 CRed = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	//glm::vec4 CBlue = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	//Red->SetFlagColour(CRed);
	//Blue->SetFlagColour(CBlue);


	Red = new Team();
	//Red->SetFlagPos(glm::vec3(0, 0, 10));
	Blue = new Team();
	//Blue->SetFlagPos(glm::vec3(0, 0, -10));

	int FlagCount = 3;
	for (int i=0;i<FlagCount;i++)
	{
		Flags.emplace_back(new Flag());

		//Flags[i]->Position.x = rand()%9;
		//Flags[i]->Position.z = rand()%9;
	}
	Flags[0]->Position = glm::vec3(9, 0, 9);
	Flags[1]->Position = glm::vec3(0, 0, 0);
	Flags[2]->Position = glm::vec3(-9, 0, -9);

	for (int i=0;i<RedSize;++i)
	{
		Red->AddMember(new Agent());
		glm::vec3 NewPos;
		NewPos.xz = glm::circularRand(10.0f);
		Red->Members[i]->SetPos(NewPos);
		
				
	}
	for (int i=0;i<BlueSize;++i)
	{
		Blue->AddMember(new Agent());
		glm::vec3 NewPos;
		NewPos.xz = glm::circularRand(10.0f);
		Blue->Members[i]->SetPos(NewPos);
		Blue->Members[i]->SetTarget(Flags[0]->GetPos());

		
		
	}
	for(int i=0;i<RedSize;++i)
	{
		Red->Members[i]->SetTarget(Flags[0]->GetPos());
		Red->Members[i]->CalcEnemy(Blue->Members);
		Red->Members[i]->SetBehaviour(Agenda);
		
	}
	for (int i=0;i<BlueSize;++i)
	{
		Blue->Members[i]->SetBehaviour(Agenda);
		Blue->Members[i]->CalcEnemy(Red->Members);
	}

	return true;
}

void Bahaviour_tree::onUpdate(float a_deltaTime) 
{
	// update our camera matrix using the keyboard/mouse
	Utility::freeMovement( m_cameraMatrix, a_deltaTime, 10 );

	// clear all gizmos from last frame
	Gizmos::clear();
	
	// add an identity matrix gizmo
	Gizmos::addTransform( glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1) );

	// add a 20x20 grid on the XZ-plane
	for ( int i = 0 ; i < 21 ; ++i )
	{
		Gizmos::addLine( glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10), 
						 i == 10 ? glm::vec4(1,1,1,1) : glm::vec4(0,0,0,1) );
		
		Gizmos::addLine( glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i), 
						 i == 10 ? glm::vec4(1,1,1,1) : glm::vec4(0,0,0,1) );
	}

	for (int i=0;i<RedSize;++i)
	{
		Red->Members[i]->CalcEnemy(Blue->Members);
		
		Red->Members[i]->update(a_deltaTime);
		Red->Members[i]->SetBehaviour(Agenda);

		if(Red->Members[i]->Position.x < -10)	{Red->Members[i]->Position.x=-10;}
		if(Red->Members[i]->Position.x > 10)	{Red->Members[i]->Position.x=10;}
		if(Red->Members[i]->Position.y < -10)	{Red->Members[i]->Position.y=-10;}
		if(Red->Members[i]->Position.y > 10)	{Red->Members[i]->Position.y=10;}

		Gizmos::addAABBFilled(Red->Members[i]->GetPos(), glm::vec3(0.5f), glm::vec4(1, 0, 0, 1));

	}
	for (int i=0;i<BlueSize;++i)
	{
		Blue->Members[i]->CalcEnemy(Red->Members);
		
		Blue->Members[i]->update(a_deltaTime);
		Blue->Members[i]->SetBehaviour(Agenda);

		if(Blue->Members[i]->Position.x < -10)	{Blue->Members[i]->Position.x=-10;}
		if(Blue->Members[i]->Position.x > 10)	{Blue->Members[i]->Position.x=10;}
		if(Blue->Members[i]->Position.y < -10)	{Blue->Members[i]->Position.y=-10;}
		if(Blue->Members[i]->Position.y > 10)	{Blue->Members[i]->Position.y=10;}



		Gizmos::addAABBFilled(Blue->Members[i]->GetPos(), glm::vec3(0.5f), glm::vec4(0, 0, 1, 1));

	}

	for (auto i : Flags)
	{
		i->Update();
	}
	// quit our application when escape is pressed
	if (glfwGetKey(m_window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
		quit();
}

void Bahaviour_tree::onDraw() 
{
	// clear the backbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// get the view matrix from the world-space camera matrix
	glm::mat4 viewMatrix = glm::inverse( m_cameraMatrix );
	
	// draw the gizmos from this frame
	Gizmos::draw(viewMatrix, m_projectionMatrix);

}

void Bahaviour_tree::onDestroy()
{
	// clean up anything we created
	Gizmos::destroy();
}

// main that controls the creation/destruction of an application
int main(int argc, char* argv[])
{
	// explicitly control the creation of our application
	Application* app = new Bahaviour_tree();
	
	if (app->create("AIE - Bahaviour tree",DEFAULT_SCREENWIDTH,DEFAULT_SCREENHEIGHT,argc,argv) == true)
		app->run();
		
	// explicitly control the destruction of our application
	delete app;

	return 0;
}