

#include "SceneFlocking.h"

using namespace std;


const int MAX_AGENTS = 33;


SceneFlocking::SceneFlocking() {

	
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		int innerW = rand() % (1281);
		int innerH = rand() % (769);
		Agent *agent = new Agent;
		agent->setPosition(Vector2D(innerW, innerH));
		agent->setTarget(Vector2D(innerW, innerH));
		agent->loadSpriteTexture("../res/soldier.png", 4);
		agents.push_back(agent);
		target = Vector2D(innerW, innerH);

		agent->setMass(0.05);

	}

}


SceneFlocking::~SceneFlocking() {
	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}
}


void SceneFlocking::update(float dtime, SDL_Event *event) {
	switch (event->type) {
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			target = Vector2D((float)(event->button.x), (float)(event->button.y));
			for (int i = 0; i < MAX_AGENTS; i++)
			{
				agents[i]->setTarget(target);
			}
			
		}
		break;
	default:
		break;
	}
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		Vector2D steering_force = agents[i]->Behavior()->Flocking(agents, agents[i], MAX_AGENTS, dtime);
		Vector2D steering_force_02 = agents[i]->Behavior()->Seek(agents[i], target, dtime);
		Vector2D total = steering_force + steering_force_02*0.3;
		agents[i]->update(total, dtime, event);

	}

}

void SceneFlocking::draw() {
	draw_circle(TheApp::Instance()->getRenderer(), (int)target.x, (int)target.y, 15, 255, 0, 0, 255);
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		agents[i]->draw();
	}
		
}

const char* SceneFlocking::getTitle() {

	return "SDL Steering Behaviors :: Flocking Demo";
}


