#pragma once
#include "Type.h"
#include "Collision.h"

class Entity;
const int MAX_ENTITIES = 250;

class World
{
public:
	World();
	~World();
	Entity* ObjectList[MAX_ENTITIES];

	void Update();
	void Draw();
	void SpawnEntity(Entity* InEntity);
	void DestroyEntity(Entity* InEntity);

	template<typename T>
	T* GetCollision(const engBoundingBox& BoundingBox)
	{
		for (int i = 0; i <MAX_ENTITIES; i++)
		{
			if (ObjectList[i] == nullptr || !IsA<T>(ObjectList[i]))
				continue;
			
			if (BoundingBox.CollidesWith(ObjectList[i]->BoundingBox))
				return dynamic_cast<T*>(ObjectList[i]);
			
		}
		return nullptr;
	}
	bool PlayerDied = false; 
	int kills = 10;
	int Level = 0;
private:
	float BatSpawnTimer = 3.f;
};