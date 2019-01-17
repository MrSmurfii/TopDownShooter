#include "TinyEngine.h"
#include "Collision.h"
#include "Entity.h"
#include "Player.h"
#include "Bat.h"
#include "World.h"
#include <cstdlib>
#include <math.h>
#include <time.h>
#include "Type.h"

void main()
{
	engInit();

	srand(time(NULL));

	while (engIsOpen())
	{
		World Gameworld;

		Gameworld.SpawnEntity(new Player(0, 0));
		Gameworld.SpawnEntity(new Bat(450, 20));
		Gameworld.SpawnEntity(new Bat(500, 20));
		Gameworld.SpawnEntity(new Bat(450, 200));
		Gameworld.SpawnEntity(new Bat(500, 400));
		Gameworld.SpawnEntity(new Bat(560, 120));
		Gameworld.SpawnEntity(new Bat(560, 401));
		Gameworld.SpawnEntity(new Bat(590, 305));

		while (engIsOpen())
		{
			if (engGetKeyDown(Key::Escape) || engGetControllerDown(ControllerKey::START))
				engClose();
			Gameworld.Update();
			Gameworld.Draw();

			if (Gameworld.PlayerDied)
			{
				engDelay(2000);
				break;
			}

			engUpdate();
		}
	}
	engDestroy();
}