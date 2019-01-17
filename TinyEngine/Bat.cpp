#include "Bat.h"
#include "TinyEngine.h"
#include <cstdlib>
#include "World.h"
#include "Pickup.h"

int killcnt = 10;

Bat::Bat(float inX, float inY) : Entity(inX, inY)
{
	SetNewTarget();
	FlySpeed = (rand() % 50) + 50;
}

void Bat::Update()
{
	

	if (X < TargetX)
		X += FlySpeed * engDelta();
	if (X > TargetX)
		X -= FlySpeed * engDelta();
	if (Y < TargetY)
		Y += FlySpeed * engDelta();
	if (Y > TargetY)
		Y -= FlySpeed * engDelta();

	BoundingBox.X = X;
	BoundingBox.Y = Y;

	float XDistance = TargetX - X;
	float YDistance = TargetY - Y;

	if (abs(XDistance) < 1.0f && abs(YDistance) < 1.0f)
	{
		WaitTimer -= engDelta();
		if (WaitTimer <= 0.f)
		{
			SetNewTarget();
			WaitTimer = 3.f;
		}
	}
}

void Bat::Draw()
{
	
	engSetColor(0xFFFFFFFF);
	engDrawRect(X, Y, 32, 32);
	engSetColor(0x000000FF);
	engDrawRect(X + 8, Y + 8, 16, 16);
}

void Bat::SetWorld(World* InWorld)
{
	Entity::SetWorld(InWorld);
	Health += InWorld->Level;
}

void Bat::TakeDamage(int Damage)
{
	Pickupchance = (rand() % 100);
	Health -= Damage;
	if (Health <= 0)
	{
		if (Pickupchance <= 15)
		{
			MyWorld->SpawnEntity(new Pickup(X, Y));
		}
		MyWorld->kills--;
		MyWorld->DestroyEntity(this);
		engPlaySound("Pain.wav");
	}
}

void Bat::SetNewTarget()
{
	engBoundingBox ScreenBox(0, 0, 800, 600);
	engBoundingBox BatBox = BoundingBox;
	do 
	{
	TargetX = X + (rand() % 400) - 200;
	TargetY = Y + (rand() % 400) - 200;

	BatBox.X = TargetX;
	BatBox.Y = TargetY;
	} while (!ScreenBox.Contains(BatBox));

}

