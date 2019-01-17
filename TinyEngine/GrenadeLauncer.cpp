#include "Type.h"
#include "Bat.h"
#include "GrenadeLauncer.h"
#include "TinyEngine.h"
#include "World.h"
#include "Bullet.h"
#include "Player.h"




void Grenade::Explode()
{
	MyWorld->SpawnEntity(new Bullet(X, Y, 1, 1));
	MyWorld->SpawnEntity(new Bullet(X, Y, 1, 0));
	MyWorld->SpawnEntity(new Bullet(X, Y, 1, -1));
	MyWorld->SpawnEntity(new Bullet(X, Y, -1, 1));
	MyWorld->SpawnEntity(new Bullet(X, Y, -1, 0));
	MyWorld->SpawnEntity(new Bullet(X, Y, -1, -1));
	MyWorld->SpawnEntity(new Bullet(X, Y, 0, 1));
	MyWorld->SpawnEntity(new Bullet(X, Y, 0, -1));
	MyWorld->SpawnEntity(new Bullet(X, Y, 0.5f, -0.5f));
	MyWorld->SpawnEntity(new Bullet(X, Y, 0.5f, 0.5f));
	MyWorld->SpawnEntity(new Bullet(X, Y, -0.5f, 0.5f));
	MyWorld->SpawnEntity(new Bullet(X, Y, -0.5f, -0.5f));
}

void Grenade::Update()
{
	X += engDelta() * 300.f * DirX;
	Y += engDelta() * 300.f * DirY;


	FuseTmr -= engDelta();
	if (FuseTmr <= 0.f)
	{
		Explode();
		FuseTmr = 1.0f;
	}

	LifeTime -= engDelta();
	if (LifeTime <= 0.f)
	{
		MyWorld->DestroyEntity(this);
		return;
	}
}

void Grenade::Draw()
{
	engSetColor(0x00FF00FF);
	engDrawRect(X, Y, 10, 10);
}

