#include "SceneEvade.h"

using namespace std;

SceneEvade::SceneEvade()
{
	Agent *agent = new Agent;
	agent->setPosition(Vector2D(640, 360));
	agent->setTarget(Vector2D(240, 360));
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agents.push_back(agent);
	target = Vector2D(640, 360);

	Agent *zombie = new Agent;
	zombie->setPosition(Vector2D(740, 360));
	zombie->setTarget(Vector2D(740, 360));
	zombie->loadSpriteTexture("../res/zombie1.png", 8);
	agents.push_back(zombie);
	target = Vector2D(740, 360);
}

SceneEvade::~SceneEvade()
{
	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}
}

void SceneEvade::update(float dtime, SDL_Event *event)
{
	switch (event->type) {
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			target = Vector2D((float)(event->button.x), (float)(event->button.y));
			agents[0]->setTarget(target);
		}
		break;
	default:
		break;
	}
	Vector2D steering_forceSOLDIER = agents[0]->Behavior()->KinematicSeek(agents[0], agents[0]->getTarget(), dtime);
	agents[0]->update(steering_forceSOLDIER, dtime, event);

	Vector2D steering_force = agents[1]->Behavior()->Evade(agents[1], agents[0], dtime);
	agents[1]->update(steering_force, dtime, event);
}

void SceneEvade::draw()
{
	draw_circle(TheApp::Instance()->getRenderer(), (int)target.x, (int)target.y, 15, 255, 0, 0, 255);
	agents[0]->draw();
	agents[1]->draw();
}

const char* SceneEvade::getTitle()
{
	return "SDL Steering Behaviors :: Evade Demo";
}