#include "SteeringBehavior.h"


float PI = 3.14159265359;


SteeringBehavior::SteeringBehavior()
{
	WanderAngle = 30;
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
	Vector2D DesiredVelocity = target - agent->position;
	DesiredVelocity.Normalize();
	DesiredVelocity *= agent->getMaxVelocity();
	Vector2D SteeringForce = (DesiredVelocity - agent->getVelocity());
	SteeringForce /= agent->getMaxVelocity();
	return SteeringForce * agent->max_force;
}

Vector2D SteeringBehavior::Seek(Agent *agent, Agent *target, float dtime)
{
	return Seek(agent, target->position, dtime);
}

Vector2D SteeringBehavior::Flee(Agent *agent, Vector2D target, float dtime)
{
	Vector2D DesiredVelocity = agent->position - target;
	DesiredVelocity.Normalize();
	DesiredVelocity *= agent->getMaxVelocity();
	Vector2D SteeringForce = (DesiredVelocity - agent->getVelocity());
	SteeringForce /= agent->getMaxVelocity();
	return SteeringForce * agent->max_force;
}

Vector2D SteeringBehavior::Flee(Agent *agent, Agent *target, float dtime)
{
	return Flee(agent, target->position, dtime);
}


Vector2D SteeringBehavior::Arrive(Agent *agent, Vector2D target, float dtime) { //PREGUNTAR AL PROFE COMO NO HACER QUE FRENE
	float radius = 400.f;
	Vector2D MyDestiny = (target-agent->position);
	std::cout << MyDestiny.Length() << std::endl;
	if (MyDestiny.Length()>radius)
	{
		return KinematicSeek(agent, target, dtime);
	}
	else {
		Vector2D steering = target - agent->position;
		float factor = MyDestiny.Length() / radius;
		Vector2D DesiredVelocity = target - agent->position;
		DesiredVelocity.Normalize();
		DesiredVelocity *= agent->getMaxVelocity()*factor;
		Vector2D SteeringForce = (DesiredVelocity - agent->getVelocity());
		SteeringForce /= agent->getMaxVelocity();
		return SteeringForce * agent->max_force;
	}
		
}



Vector2D SteeringBehavior::Pursue(Agent *agent, Agent *target, float dtime) {
	
	if (target->velocity.Length() != 0)
	{
		float T = ((target->position - agent->position) / target->velocity.Length()).Length();
		Vector2D PredictedTarget = target->position + target->velocity * T;
		std::cout << PredictedTarget.x << "   " << PredictedTarget.y << std::endl;
		return  KinematicSeek(agent, PredictedTarget, dtime);
	}
	else
	{
		return KinematicSeek(agent, target, dtime);
	}
}

Vector2D SteeringBehavior::Evade(Agent *agent, Agent *target, float dtime) {

	if (target->velocity.Length() != 0)
	{
		float T = ((agent->position - target->position) / target->velocity.Length()).Length();
		Vector2D PredictedTarget = target->position + target->velocity * T;
		std::cout << PredictedTarget.x << "   " << PredictedTarget.y << std::endl;
		return  KinematicFlee(agent, PredictedTarget, dtime);
	}
	else
	{
		return KinematicFlee(agent, target, dtime);
	}
}

float RandomBinomial() {
	return ((float)rand()/(RAND_MAX))-((float)rand()/ RAND_MAX);
}

Vector2D SteeringBehavior::Wander(Agent *agent, Vector2D target, float dtime) {
	
	const float WanderMaxAngleChange = 50.f;
	const float WanderOffset = 400.f;
	const float WanderRadius = 100.f;

	float TargetAngle;

	WanderAngle += RandomBinomial() * WanderMaxAngleChange;
	TargetAngle = agent->orientation + WanderAngle;
	float RadianAngle = 2 * PI * (TargetAngle / 360);

	Vector2D CircleCenter, TargetPosition;
	CircleCenter = agent->position + agent->velocity.Normalize() * WanderOffset;
	TargetPosition.x = CircleCenter.x + WanderRadius * cos(RadianAngle*DEG2RAD);
	TargetPosition.y = CircleCenter.y + WanderRadius * sin(RadianAngle*DEG2RAD);


	draw_circle(TheApp::Instance()->getRenderer(), (float)TargetPosition.x, (float)TargetPosition.y, 100, 255, 0, 0, 255);

	TargetPosition *= agent->getMaxVelocity();
	Vector2D SteeringForce = (TargetPosition - agent->getVelocity());
	SteeringForce /= agent->getMaxVelocity();
	return SteeringForce * agent->max_force;

}


Vector2D SteeringBehavior::Flocking(std::vector<Agent*> agents, Agent* Self, Vector2D Target, float dtime) {
	const float K_SEPARATION_FORCE = 0.65f;
	const float K_COHESION_FORCE = 0.6f;
	const float K_ALIGNMENT_FORCE = 1.f;

	const float NEIGHBOR_RADIUS = 120.f;
	
	//Flee 
	float neighborCountFlee = 0;
	Vector2D separationVector, separationDirection;
	//Seek 
	float neighborCountSeek = 0;
	Vector2D averagePosition, cohesionDirection;
	//Alignment 
	float neighborCountAligment = 0;
	Vector2D averageVelocity, aligmentDirection;

	//FLEE
	for (int i = 0; i < 10; i++)
	{
		if ((agents[i]->position - Self->position).Length() < NEIGHBOR_RADIUS)
		{
			separationVector += (Self->position -agents[i]->position);
			neighborCountFlee++;
		}
	}
	separationVector /= neighborCountFlee;
	separationDirection = separationVector.Normalize();

	//SEEK
	for (int i = 0; i < 10; i++)
	{
		if ((agents[i]->position - Self->position).Length() < NEIGHBOR_RADIUS)
		{
			averagePosition += agents[i]->position;
			neighborCountSeek++;
		}
	}
	averagePosition /= neighborCountSeek;
	averagePosition -= Self->position;
	cohesionDirection = averagePosition.Normalize();

	//ALIGMENT
	for (int i = 0; i < 10; i++)
	{
		if ((agents[i]->position - Self->position).Length() < NEIGHBOR_RADIUS)
		{
			averageVelocity += agents[i]->velocity;
			neighborCountAligment++;
		}
	}
	averageVelocity /= neighborCountAligment;
	aligmentDirection = averageVelocity.Normalize();

	
	std::cout << neighborCountAligment << std::endl;

	Vector2D flockingForce;
	flockingForce = separationDirection * K_SEPARATION_FORCE + 
					cohesionDirection * K_COHESION_FORCE + 
					aligmentDirection * K_ALIGNMENT_FORCE;
	flockingForce *= 2;
	flockingForce *= Self->getMaxVelocity();
	Vector2D SteeringForce = (flockingForce - Self->getVelocity());
	SteeringForce /= Self->getMaxVelocity();
	return SteeringForce * Self->max_force;
}

