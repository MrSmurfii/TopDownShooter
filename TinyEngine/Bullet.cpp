#include "Bullet.h"
#include "TinyEngine.h"
#include "World.h"
#include "Type.h"
#include "Bat.h"
#include <stdlib.h>
#include "Pickup.h"
#include "iDamageable.h"

void Bullet::Update()
{
	X += engDelta() * 600.f * DirectionX;
	Y += engDelta() * 600.f * DirectionY;
	BoundingBox.X = X;
	BoundingBox.Y = Y;
	BoundingBox.Width = 10;
	BoundingBox.Height = 10;

	IDamageable* HitDamagable = MyWorld->GetCollision<IDamageable>(BoundingBox);

	if (HitDamagable != nullptr)
	{

		HitDamagable->TakeDamage(1);
		MyWorld->DestroyEntity(this);
		return;
	}

	LifeTime -= engDelta();
	if (LifeTime <= 0.f)
	{
		MyWorld->DestroyEntity(this);
		return;
	}
}

void Bullet::Draw()
{
	engSetColor(0xFFFF00FF);
	engDrawRect(X, Y, 10, 10);
}

