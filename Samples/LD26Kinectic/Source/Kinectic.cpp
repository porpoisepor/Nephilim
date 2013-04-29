#include "Kinectic.h"

#include <Nephilim/CGL.h>

#include <Nephilim/KxScene.h>
#include <Nephilim/KxDraw.h>
#include <Nephilim/KxMousePicker.h>

#include <Nephilim/ParticleSystem.h>
#include <Nephilim/Texture.h>
#include <Nephilim/Sprite.h>
#include <Nephilim/MMath.h>

#include <iostream>
using namespace std;

KxScene world;

Texture tex;

ParticleSystem p1;

std::vector<KxBody*> actors;
std::vector<ParticleSystem> actorParticles;

float angle = 0; 
float angleInc = Math::pi*2 / 4;
float torusRadius = 30;

float p = 2;
float q = 3;


KxMousePicker picker;

void Kinectic::onCreate()
{	
	world.CreateQuickCircle(200,200,20);
	world.CreateStaticBox(0,600,2500, 10);

	picker.scene = &world;

	p1.create();
	p1.tank = 150;
	p1.position = vec3(200,200,0);
}

void Kinectic::onEvent(Event &event)
{
	if(event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
	{
		picker.attachAt(event.mouseButton.x, event.mouseButton.y);
	}
	if(event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
	{
		picker.detach();
	}
	if(event.type == Event::MouseMoved)
	{
		picker.update(event.mouseMove.x, event.mouseMove.y);
	}

   if(event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::XButton1)
   {
	   world.CreateQuickCircle(event.mouseButton.x,event.mouseButton.y,20);
   }

   if(event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Right)
   {
	   KxBody* actor = world.CreateQuickBox(event.mouseButton.x,event.mouseButton.y, 20, 20);
	   actors.push_back(actor);
	   ParticleSystem pp;
	   pp.create();
	   pp.tank = 35;
	   actorParticles.push_back(pp);
	   cout << "box position: "<<actor->m_body->GetPosition().y * 100<<endl;
   }

   if(event.type == Event::KeyPressed && event.key.code == Keyboard::Up)
   {
	   p++;cout << "p: "<< p << " q: "<<q<<endl;
   }
   if(event.type == Event::KeyPressed && event.key.code == Keyboard::Down)
   {
	   p--;cout << "p: "<< p << " q: "<<q<<endl;
   }
   if(event.type == Event::KeyPressed && event.key.code == Keyboard::Right)
   {
	   q++;cout << "p: "<< p << " q: "<<q<<endl;
   }
   if(event.type == Event::KeyPressed && event.key.code == Keyboard::Left)
   {
	   q--; cout << "p: "<< p << " q: "<<q<<endl;
   }
}

void Kinectic::onUpdate(Time time)
{ 
	world.update(time.asSeconds());

	for(int i =0; i < actors.size(); i++)
	{
		//cout << "box position: "<<actors[i]->getPosition().y<<endl;
		actorParticles[i].position = vec3(actors[i]->getPosition(), 0);
		actorParticles[i].update(time.asSeconds());
	}

	angle += angleInc * time.asSeconds();

	float r = cos(q * angle) + 2;
	p1.position = vec3(300 + r * cos(p * angle) * torusRadius, 300 + r * sin(p * angle) * torusRadius, 0);
	p1.update(time.asSeconds());
}

void Kinectic::onRender()
{
	getRenderer()->setClearColor(Color::Blue);
	getRenderer()->setProjectionMatrix(View(0,0,1024,768).getMatrix());


	Sprite s;
	s.setTexture(tex);
	s.resize(1024,768);
	getRenderer()->draw(s);

	getRenderer()->draw(KxDraw(world));

	for(int i =0; i < actors.size(); i++)
	{
		getRenderer()->draw(actorParticles[i]);
	}

	getRenderer()->draw(p1);
}