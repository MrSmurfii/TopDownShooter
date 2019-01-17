#pragma once
#include "Entity.h"

class Pickup : public Entity
{
public:
	Pickup(float InX, float InY) : Entity(InX, InY) {}

	void Draw() override;
	void Update() override;

private:
	
};