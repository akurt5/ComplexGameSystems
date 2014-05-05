#include "Bahaviour_tree.h"
#include "Gizmos.h"
#include "Utilities.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include "Button.h"
#include <algorithm>

#include <SOIL.h>

#include "Agent.h"



class WithinRange : public Behaviour
{
public:

	WithinRange(float _Range) : Range2 (_Range*_Range){}
	virtual ~WithinRange(){}
	virtual bool Execute(Agent *_Agent)
	{
		float dist2 = 0;
		if(_Agent->Attack)
		{
			Agent *TargetEnemy = nullptr;
			for ( auto EnemyCurrent : _Agent->Enemies)
			{
				TargetEnemy = EnemyCurrent;
				float Len = 0, tLen;
				
				tLen = glm::length(_Agent->GetPos()) - glm::length(EnemyCurrent->GetPos());
				if(tLen < 0){tLen *= -1;}
				if(tLen > Len){Len = tLen;TargetEnemy = EnemyCurrent;}
			}
			if(TargetEnemy != nullptr)
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
			Agent *TargetEnemy = nullptr;
			for ( auto EnemyCurrent : _Agent->Enemies)
			{
				float Len = 0, tLen;
				
				tLen = glm::length(_Agent->GetPos()) - glm::length(EnemyCurrent->GetPos());
				if(tLen < 0){tLen *= -1;}
				if(tLen > Len){Len = tLen;TargetEnemy = EnemyCurrent;}
			}
			if(TargetEnemy != nullptr)
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
		glm::vec3 Pos, Dir(0);
		if(_Agent->Attack)
		{
			Agent *TargetEnemy = nullptr;
			for ( auto EnemyCurrent : _Agent->Enemies)
			{
				float Len = 0, tLen;
				
				tLen = glm::length(_Agent->GetPos()) - glm::length(EnemyCurrent->GetPos());
				if(tLen < 0){tLen *= -1;}
				if(tLen > Len){Len = tLen;TargetEnemy = EnemyCurrent;}
			}
			Pos = _Agent->GetPos();
			if(TargetEnemy != nullptr)
				Dir = glm::normalize(TargetEnemy->GetPos() - Pos + TargetEnemy->Velocity);
		}
		else
		{
				Pos = _Agent->GetPos();
				Dir = glm::normalize(_Agent->GetTarget() - Pos);
		}
		_Agent->Velocity +=(Dir * Speed) * Utility::getDeltaTime();
		
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
		glm::vec3 Pos, Dir(0);
		Agent *TargetEnemy = nullptr;
		float Len = 0, tLen;
		for ( auto EnemyCurrent : _Agent->Enemies)
			{
				tLen = glm::length(_Agent->GetPos()) - glm::length(EnemyCurrent->GetPos());
				if(tLen < 0){tLen *= -1;}
				if(tLen > Len){Len = tLen;TargetEnemy = EnemyCurrent;}
			}
		if(Len < 0.5)
		{
			
				Pos = _Agent->GetPos();
				if(TargetEnemy != nullptr)
					Dir = glm::normalize(TargetEnemy->GetPos() - Pos);
				_Agent->Velocity -=(Dir * Speed) * Utility::getDeltaTime();

		
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

	uiBlueUp = SOIL_load_OGL_texture("../../Build/textures/blueup.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	uiBlueDown = SOIL_load_OGL_texture("../../Build/textures/bluedown.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	uiRedUp = SOIL_load_OGL_texture("../../Build/textures/redup.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	uiRedDown = SOIL_load_OGL_texture("../../Build/textures/reddown.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	// create a world-space matrix for a camera
	m_cameraMatrix = glm::inverse( glm::lookAt(glm::vec3(10,10,10),glm::vec3(0,0,0), glm::vec3(0,1,0)) );
	
	// create a perspective projection matrix with a 90 degree field-of-view and widescreen aspect ratio
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, DEFAULT_SCREENWIDTH/(float)DEFAULT_SCREENHEIGHT, 0.1f, 1000.0f);
	m_screenProjectionMatrix = glm::ortho<float>(0, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, 0,0,100);
	// set the clear colour and enable depth testing and backface culling
	glClearColor(0.25f,0.25f,0.25f,1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	Screen = new ShaderHandle();
	Screen->Load(2, "Screen.vert", "Screen.frag");

	bBlueUp = new Button(glm::vec2(25 , 25), glm::vec2(50));
	bBlueDown = new Button(glm::vec2(25, 100), glm::vec2(50));
	bRedUp = new Button(glm::vec2(100, 25), glm::vec2(50));
	bRedDown = new Button(glm::vec2(100 ,100), glm::vec2(50));

	Behaviour* Seek = new SeekTarget(3);
	Behaviour* Rand = new RandomiseTarget(10);
	Behaviour* Within = new WithinRange(0.5f);
	Behaviour* Flee = new FleeTarget(3);

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
	Root->addchild(Seek);
	
	Agenda = Root;

	RedSize = 3;
	BlueSize = 3;



	Red = new Team();
	Blue = new Team();

	int FlagCount = 3;
	for (int i=0;i<FlagCount;i++)
	{
		Flags.emplace_back(new Flag());

	}
	Flags[0]->Position = glm::vec3(9, 0, 9);
	Flags[1]->Position = glm::vec3(0, 0, 0);
	Flags[2]->Position = glm::vec3(-9, 0, -9);

	for (int i=0;i<RedSize;++i)
	{
		Red->AddMember();
		glm::vec3 NewPos;
		NewPos.xz = glm::circularRand(10.0f);
		Red->Members[i]->SetPos(NewPos);
		
				
	}
	for (int i=0;i<BlueSize;++i)
	{
		Blue->AddMember();
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

	
	bBlueUp->Update();
	if (bBlueUp->IsActivated()){
		Blue->AddMember();
		glm::vec3 NewPos;
		NewPos.xz = glm::circularRand(10.0f);
		Blue->Members.back()->SetPos(NewPos);
		Blue->Members.back()->SetTarget(Flags[0]->GetPos());
		Blue->Members.back()->SetBehaviour(Agenda);
		Blue->Members.back()->CalcEnemy(Red->Members);
	}
	bBlueDown->Update();
	if (bBlueDown->IsActivated()){
		if(Blue->Members.size() <= 1)
		{
			Blue->DelMember();
		}
	}
	bRedUp->Update();
	if (bRedUp->IsActivated()){
		Red->AddMember();
		glm::vec3 NewPos;
		NewPos.xz = glm::circularRand(10.0f);
		Red->Members.back()->SetPos(NewPos);
		Red->Members.back()->SetTarget(Flags[0]->GetPos());
		Red->Members.back()->SetBehaviour(Agenda);
		Red->Members.back()->CalcEnemy(Blue->Members);
	}
	bRedDown->Update();
	if (bRedDown->IsActivated()){
		if(Red->Members.size() <= 1)
		{
			Red->DelMember();
		}
	}

	for (int i=0;i<Red->Members.size();++i)
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
	for (int i=0;i<Blue->Members.size();++i)
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

	glUseProgram(Screen->m_shader);

	int location = glGetUniformLocation(Screen->m_shader, "Projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr( m_screenProjectionMatrix));

	{
		GLint uDiffuseTexture1 = glGetUniformLocation(Screen->m_shader, "Texture");
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, uiBlueUp );
		glUniform1i( uDiffuseTexture1, 0 );
		bBlueUp->Draw();

		GLint uDiffuseTexture2 = glGetUniformLocation(Screen->m_shader, "Texture");
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, uiBlueDown );
		glUniform1i( uDiffuseTexture2, 0 );
		bBlueDown->Draw();

		GLint uDiffuseTexture3 = glGetUniformLocation(Screen->m_shader, "Texture");
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, uiRedUp );
		glUniform1i( uDiffuseTexture3, 0 );
		bRedUp->Draw();

		GLint uDiffuseTexture4 = glGetUniformLocation(Screen->m_shader, "Texture");
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, uiRedDown );
		glUniform1i( uDiffuseTexture4, 0 );
		bRedDown->Draw();
	}


	if (glfwGetKey(m_window, GLFW_KEY_SPACE))
	{
		unsigned int count = m_sponza->getMeshCount();
		for (unsigned int i = 0 ; i < count ; ++i )
		{
			FBXMeshNode* mesh = m_sponza->getMeshByIndex(i);

			GLData* data = (GLData*)mesh->m_userData;

			location = glGetUniformLocation(sponzashader, "model");
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr( mesh->m_globalTransform ));

			location = glGetUniformLocation(m_shader, "invTransposeModel");
			glUniformMatrix4fv(location, 1, GL_TRUE, glm::value_ptr( glm::inverse( mesh->m_globalTransform ) ));

			glBindVertexArray(data->vao);
			glDrawElements(GL_TRIANGLES, mesh->m_indices.size(), GL_UNSIGNED_INT, nullptr);
		}
	}

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


