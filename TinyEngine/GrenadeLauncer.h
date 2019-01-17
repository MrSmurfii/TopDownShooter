#pragma once
#include "Entity.h"

class Grenade : public Entity
{
public:
	Grenade(float InX, float InY, float DirX, float DirY):
		Entity(InX , InY), DirX(DirX), DirY(DirY)
	{}
	

	void Explode();
	void Update() override;
	void Draw() override;

private:
	float FuseTmr = 1.f;
	float DirX;
	float DirY;
	float LifeTime = 1.05f;
}; 