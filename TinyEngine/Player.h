#pragma once
#include "Entity.h"
#include "SDL/SDL_gamecontroller.h"

enum class WeaponType
{
	Pistol,
	MachineGun,
	SMG,
	Shotgun,
	MAX
};

class Player : public Entity
{
public:
	Player(float inX, float inY);
	void Update() override;
	void Draw() override;
	bool IsColliding= false;
	WeaponType CurrentWeapon = WeaponType::Pistol;

private:
	float WeaponCooldown = 0.f;
	float Burst = 0.f;
	int BurstCount = 5;
	float XSpread1 = 0.f;
	float XSpread2 = 0.f;
	float YSpread1 = 0.f;
	float YSpread2 = 0.f;
	int GrenadeCount = 3;
	SDL_GameController* GameController;

};