#include "World.h"
#include "Entity.h"
#include "Bat.h"
#include "TinyEngine.h"
#include "GrenadeLauncer.h"
#include <cstdlib>


World::World()
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		ObjectList[i] = nullptr;
	}
}

World::~World()
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (ObjectList[i] != nullptr)
		{
			delete ObjectList[i];
			ObjectList[i] = nullptr;
		}
	}
}

void World::Update()
{
	if (kills <= 0)
	{
		Level++;
		kills = 10;
	}
	engSetColor(0xFFFFFFFF);
	engDrawStringFormat(400, 10, "Stronger enemies in: %d kills", kills);
	int XSpawn = (rand() % 399) + 400;
	int YSpawn = (rand() % 600);

	BatSpawnTimer -= engDelta();
	if (BatSpawnTimer <= 0.f)
	{
		SpawnEntity(new Bat(XSpawn, YSpawn));
		BatSpawnTimer = 1.f;
	}

	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (ObjectList[i] == nullptr)
			continue;

		ObjectList[i]->Update();
	}

}

void World::Draw()
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (ObjectList[i] == nullptr)
			continue;
		ObjectList[i]->Draw();
	}
}

void World::SpawnEntity(Entity* InEntity)
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
      		if (ObjectList[i] == nullptr)
		{
			ObjectList[i] = InEntity;
			break;
		}
	}
	InEntity->SetWorld(this);
}

void World::DestroyEntity(Entity* InEntity)
{
	for (int i = 0; i < MAX_ENTITIES; i++)
		if (ObjectList[i] == InEntity)
		{
			ObjectList[i] = nullptr;
			delete InEntity;
			break;
		}

}

