#include "Physics.h"



Physics::Physics()
{
}


Physics::~Physics()
{
}

float Physics::Velocity(GameObject go)
{
	float velocity = 0;

	return velocity;
}

float Physics::Impulse(GameObject go) //or momentoum?
{
	//Impulse = mass*velocity

	float impulse;

	impulse = Velocity(go) * go.mass;

	return impulse;
}

void Physics::ResolveCollision(GameObject go1, GameObject go2)
{
	//relative velocity
	float rv = Velocity(go1) - Velocity(go2);
	
	//relative velocity in terms of normal direction
}

//todo: gravity n shit