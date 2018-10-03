#include "SceneAvoidance.h"

using namespace std;


const int MAX_AGENTS = 33;
int w = 600;
int h = 300;

SceneAvoidance::SceneAvoidance() {


	for (int i = 0; i < MAX_AGENTS; i++)
	{
		int innerW = rand() % (w+200);
		int innerH = rand() % (h+200);
		Agent *agent = new Agent;
		agent->setPosition(Vector2D(600, 300));
		agent->setTarget(Vector2D(600, 300));
		agent->loadSpriteTexture("../res/soldier.png", 4);
		agents.push_back(agent);
		target = Vector2D(600, 300);

		agent->setMass(0.05);

	}

}


SceneAvoidance::~SceneAvoidance() {
	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}
}


void SceneAvoidance::update(float dtime, SDL_Event *event) {
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
	Vector2D total;
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		Vector2D steering_force = agents[i]->Behavior()->Flocking(agents, agents[i], MAX_AGENTS, dtime);
		Vector2D steering_force_02 = agents[i]->Behavior()->Seek(agents[i], target, dtime);
		Vector2D steering_force_03 = agents[i]->Behavior()->Avoidance(agents, agents[i], SDL_SimpleApp::Instance()->getWinSize().x, SDL_SimpleApp::Instance()->getWinSize().y,dtime);
		total = steering_force + steering_force_02 * 0.3 + steering_force_03;
		agents[i]->update(total, dtime, event);

	}

}

void SceneAvoidance::draw() {
	draw_circle(TheApp::Instance()->getRenderer(), (int)target.x, (int)target.y, 15, 255, 0, 0, 255);
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		agents[i]->draw();
	}

}

const char* SceneAvoidance::getTitle() {

	return "SDL Steering Behaviors :: Avoidance Demo";
}
