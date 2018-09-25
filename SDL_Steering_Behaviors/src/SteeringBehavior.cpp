#include "SteeringBehavior.h"



SteeringBehavior::SteeringBehavior()
{
}


SteeringBehavior::~SteeringBehavior()
{
}

Vector2D SteeringBehavior::KinematicSeek(Agent *agent, Vector2D target, float dtime)
{
	Vector2D steering = target - agent->position;
	steering.Normalize();
	return steering * agent->max_velocity;
}

Vector2D SteeringBehavior::KinematicSeek(Agent *agent, Agent *target, float dtime)
{
	return KinematicSeek(agent, target->position, dtime);
}

Vector2D SteeringBehavior::KinematicFlee(Agent *agent, Vector2D target, float dtime)
{
	Vector2D steering = agent->position - target;
	steering.Normalize();
	return steering * agent->max_velocity;
}

Vector2D SteeringBehavior::KinematicFlee(Agent *agent, Agent *target, float dtime)
{
	return KinematicFlee(agent, target->position, dtime);
}

/* Add here your own Steering Behavior functions definitions */

Vector2D SteeringBehavior::Seek(Agent *agent, Vector2D target, float dtime)
{
	return Vector2D(0, 0);
}

Vector2D SteeringBehavior::Seek(Agent *agent, Agent *target, float dtime)
{
	return Seek(agent, target->position, dtime);
}

Vector2D SteeringBehavior::Flee(Agent *agent, Vector2D target, float dtime)
{
	return Vector2D(0,0);
}

Vector2D SteeringBehavior::Flee(Agent *agent, Agent *target, float dtime)
{
	return Flee(agent, target->position, dtime);
}


Vector2D SteeringBehavior::Arrive(Agent *agent, Vector2D target, float dtime) { //PREGUNTAR AL PROFE COMO NO HACER QUE FRENE TAN SEMAO
	float radius = 200.f;
	Vector2D MyDestiny = (target-agent->position);
	std::cout << MyDestiny.Length() << std::endl;
	if (MyDestiny.Length()>radius)
	{
		return KinematicSeek(agent, target, dtime);
	}
	else{
		float factor = MyDestiny.Length() / radius;
		agent->velocity *= factor;
		return KinematicSeek(agent, target, dtime);
	}
}



Vector2D SteeringBehavior::Pursue(Agent *agent, Agent *target, float dtime) {
	float T = ((target->position - agent->position) / target->velocity.Length()).Length();
	std::cout << agent->position.x << "   " << agent->position.y << std::endl;
	Vector2D PredictedTarget = target->position + target->velocity * T;


	return KinematicSeek(agent, target, dtime);
}
