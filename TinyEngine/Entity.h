#pragma once
#include "Collision.h"

class World;

class Entity
{
public:
	Entity(float inX, float inY);

	virtual void Update() = 0;
	virtual void Draw();
	virtual void SetWorld(World* InWorld);
	float X;
	float Y;

	engBoundingBox BoundingBox;
protected:
	World* MyWorld;
};
