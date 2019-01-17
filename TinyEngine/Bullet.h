#pragma once
#include "Entity.h"

void engPlaySound(char* path);

class Bullet : public Entity
{
public:
	Bullet(int InX, int InY, float DirX, float DirY) 
		: Entity(InX, InY), DirectionX(DirX), DirectionY(DirY)
	{
		engPlaySound("Shoot.wav");
	}

	void Update() override;
	void Draw() override;
private:
	float LifeTime = 5.f;
	float DirectionX;
	float DirectionY;
};