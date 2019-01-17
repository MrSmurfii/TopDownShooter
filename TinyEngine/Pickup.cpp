#include "Pickup.h"
#include "TinyEngine.h"
#include "Entity.h"


void Pickup::Draw()
{
	engSetColor(0xFF0000FF);
	engDrawRect(X, Y, 10, 10);
}

void Pickup::Update()
{
	BoundingBox.X = X;
	BoundingBox.Y = Y;
	BoundingBox.Width = 10;
	BoundingBox.Height = 10;
}

