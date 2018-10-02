

#include "SceneFlocking.h"

using namespace std;


SceneFlocking::SceneFlocking() {
	int w = 400;
	int h = 300;
	for (int i = 0; i < 10; i++)
	{
		Agent *agent = new Agent;
		agent->setPosition(Vector2D(w, h));
		agent->setTarget(Vector2D(w, h));
		agent->loadSpriteTexture("../res/soldier.png", 4);
		agents.push_back(agent);
		target = Vector2D(w, h);

		w += 40;
		h += 40;
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
			for (int i = 0; i < 10; i++)
			{
				agents[i]->setTarget(target);
			}
			
		}
		break;
	default:
		break;
	}
	for (int i = 0; i < 10; i++)
	{
		Vector2D steering_force = agents[i]->Behavior()->Flocking(agents, agents[i], agents[0]->getTarget(), dtime);
		agents[i]->update(steering_force, dtime, event);
	}

}

void SceneFlocking::draw() {
	draw_circle(TheApp::Instance()->getRenderer(), (int)target.x, (int)target.y, 15, 255, 0, 0, 255);
	for (int i = 0; i < 10; i++)
	{
		agents[i]->draw();
	}
		
}

const char* SceneFlocking::getTitle() {

	return "SDL Steering Behaviors :: Flocking Demo";
}


