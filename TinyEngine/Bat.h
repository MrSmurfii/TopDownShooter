#pragma once
#include "Entity.h"
#include "iDamageable.h"


class Bat : public Entity, public IDamageable
{
public:
	Bat(float inX, float inY);
	
	void Update() override;
	void TakeDamage(int Damage) override;
	void Draw() override;
	void SetWorld(World* InWorld) override;
	int FlySpeed = 50;
	int Health = 1;

private:
	void SetNewTarget();
	float TargetX, TargetY;
	float WaitTimer = 3;
	int Pickupchance;

};