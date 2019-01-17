#include "Entity.h"
#include "TinyEngine.h"

Entity::Entity(float inX, float inY) :Y(inY), X(inX), BoundingBox(inX, inY, 32, 32)
{}


void Entity::Draw()

{
	engSetColor(0xFFFFFFFF);
	engDrawRect(X, Y, 32, 32);
}

void Entity::SetWorld(World* InWorld)
{
	MyWorld = InWorld;
}
