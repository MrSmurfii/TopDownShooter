#include "TinyEngine.h"
#include "Player.h"
#include "World.h"
#include "Bullet.h"
#include "Type.h"
#include "Bat.h"
#include "GrenadeLauncer.h"
#include "Pickup.h"
#include "SDL/SDL_gamecontroller.h"
#include <SDL/SDL_audio.h>


Player::Player(float inX, float inY) : Entity(inX, inY)
{
	GameController = SDL_GameControllerOpen(0);
}

void Player::Update()
{

	engSetColor(0xFFFFFFFF);
	engDrawStringFormat(150, 10, "Grenades: %d", GrenadeCount);
	//Controller X & Y axis
	float Xinput = engGetAxis(ControllerAxis::AXIS_LEFTX);
	float Yinput = engGetAxis(ControllerAxis::AXIS_LEFTY);


	float DirectionX = 0;
	float DirectionY = 0;

	//Controller Movement
	X += Xinput * 150 * engDelta();
	Y += Yinput * 150 * engDelta();

	// Shoot direction (controller movement)
	if (Xinput < 0)
	{
		DirectionX--;
	}
	if (Xinput > 0)
	{
		DirectionX++;
	}
	if (Yinput < 0)
	{
		DirectionY--;
	}
	if (Yinput > 0)
	{
		DirectionY++;
	}


	// Keyboard Movement
	if (engGetKey(Key::Right))
	{
		X += 100.f * engDelta();
		DirectionX++;
	}
	if (engGetKey(Key::Left))
	{
		X -= 100.f * engDelta();
		DirectionX--;
	}
	if (engGetKey(Key::Up))
	{
		Y -= 100.f * engDelta();
		DirectionY--;
	}
	if (engGetKey(Key::Down))
	{
		Y += 100.f * engDelta();
		DirectionY++;
	}

	//Screen Wrapping
	if (X >= 808.f)
		X = -32.f;
	if (X <= -38.f)
		X = 800.f;
	if (Y >= 608.f)
		Y = -32.f;
	if (Y <= -38.f)
		Y = 600.f;

	// change weapon
	if (engGetKeyDown(Key::Tab) ||
		engGetControllerDown(ControllerKey::Y))
	{
		CurrentWeapon = (WeaponType)(((int)CurrentWeapon + 1) % (int)WeaponType::MAX);
	}

	// Throw grenade
	if (engGetKeyDown(Key::E) ||
		engGetControllerDown(ControllerKey::X))
	{
		if (GrenadeCount > 0)
		{
			MyWorld->SpawnEntity(new Grenade(X + 32.f, Y + 16.f, DirectionX, DirectionY));
			GrenadeCount--;
		}
	}

	//shoot forward if standing still
	if (DirectionX == 0 && DirectionY == 0)
		DirectionX = 1;

	//weapontypes and shooting
	WeaponCooldown -= engDelta();
	switch (CurrentWeapon)
	{
	case WeaponType::Pistol:
	{
		if (engGetKeyDown(Key::Space) ||
			engGetControllerDown(ControllerKey::A))
		{
			MyWorld->SpawnEntity(new Bullet(X + 32 * DirectionX, Y + 16 * DirectionY, DirectionX, DirectionY));
		}

	}break;
	case WeaponType::MachineGun:
	{
		if (engGetKey(Key::Space) && WeaponCooldown <= 0.f ||
			engGetController(ControllerKey::A) && WeaponCooldown <= 0.f)
		{
			MyWorld->SpawnEntity(new Bullet(X + 32 * DirectionX, Y + 16 * DirectionY, DirectionX, DirectionY));
			WeaponCooldown = 0.1f;
		}
	}break;
	case WeaponType::SMG:
	{
		Burst -= engDelta();
		if (engGetKey(Key::Space) && WeaponCooldown <= 0.f ||
			engGetController(ControllerKey::A) && WeaponCooldown <= 0.f)
		{
			if (Burst <= 0)
			{
				MyWorld->SpawnEntity(new Bullet(X + 32 * DirectionX, Y + 16 * DirectionY, DirectionX, DirectionY));
				Burst = 0.1f;
				BurstCount--;
			}
			if (BurstCount == 0)
			{
				WeaponCooldown = 0.5f;
				BurstCount = 5;
			}

		}break;
	case WeaponType::Shotgun:
	{
		if (engGetKeyDown(Key::Space) ||
			engGetControllerDown(ControllerKey::A))
		{
			if (DirectionX == 1 || DirectionX == -1)
			{
				YSpread1 = 0.1f;
				YSpread2 = 0.2f;
			}
			if (DirectionY == 1 || DirectionY == -1)
			{
				XSpread1 = 0.1f;
				XSpread2 = 0.2f;
			}
			if (DirectionX == DirectionY)
			{
				XSpread1 *= -1;
				XSpread2 *= -1;
			}
			MyWorld->SpawnEntity(new Bullet(X + 32 * DirectionX, Y + 16 * DirectionY, DirectionX, DirectionY));
			MyWorld->SpawnEntity(new Bullet(X + 32 * DirectionX, Y + 16 * DirectionY, (DirectionX + XSpread1), (DirectionY + YSpread1)));
			MyWorld->SpawnEntity(new Bullet(X + 32 * DirectionX, Y + 16 * DirectionY, (DirectionX + XSpread2), (DirectionY + YSpread2)));
			MyWorld->SpawnEntity(new Bullet(X + 32 * DirectionX, Y + 16 * DirectionY, (DirectionX - XSpread1), (DirectionY - YSpread1)));
			MyWorld->SpawnEntity(new Bullet(X + 32 * DirectionX, Y + 16 * DirectionY, (DirectionX - XSpread2), (DirectionY - YSpread2)));
			

			XSpread1 = 0.f;
			XSpread2 = 0.f;
			YSpread1 = 0.f;
			YSpread2 = 0.f;

		}
	}
	}
	}


	BoundingBox.X = X;
	BoundingBox.Y = Y;

	// pickups
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		Entity* EntPtr = MyWorld->ObjectList[i];

		if (MyWorld->ObjectList[i] == nullptr || MyWorld->ObjectList[i] == this)
			continue;

		if (BoundingBox.CollidesWith(MyWorld->ObjectList[i]->BoundingBox))
		{
			if (IsA<Pickup>(MyWorld->ObjectList[i]))
			{
				MyWorld->DestroyEntity(EntPtr);
				GrenadeCount++;
			}
		}
	}

	// collision check bat vs. player
	IsColliding = false;
	Bat* BatCollision = MyWorld->GetCollision<Bat>(BoundingBox);
	if (BatCollision != nullptr)
	{
		IsColliding = true;
		MyWorld->PlayerDied = true;
	}




}

// draw player
void Player::Draw()
{
	if (IsColliding)
		engSetColor(0xFF0000FF);
	else
		engSetColor(0xFFFFFFFF);

	engDrawRect(X, Y, 32, 32);
	engSetColor(0x0000FFFF);
	engDrawRect(X + 8, Y + 8, 16, 16);
}



