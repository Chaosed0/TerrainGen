//TerrainManager.cpp - Implementation of the terrain manager.
//Programmed by Edward Lu

#include "TerrainManager.h"

/*

Terrain Manager rules:
1.) The player must see the terrain they are standing on and the 8 surrounding terrains.
2.) No more than those 9 terrains must be rendered at a time.
3.) Eventually, the terrain manager must offload terrains onto disk or somehow store them other than in memory.

*/

TerrainManager::TerrainManager()
{
	//We first initialize the center terrain
	center = new Terrain();
	center->generateTerrain(BASE_HEIGHT, RAND_SCALE, SCALE_DIV);
	center->Update(0, 0, center->getLength() - 1);
	//The terrain the player is currently on is the center terrain
	current = center;
	//We arbitrarily designate the center terrain as (0, 0)
	curX = curZ = 0;
	//The 8 surrounding terrains will be generated on the next call to Update()
}

//Update: Should be run every frame. Decides which terrains to draw and, if some terrains are not initialized,
// it generates them.
/*
Since terrains are centered, it goes like this:

(o is (0, 0))
 ___ ___ ___
*   *   *   *
|   |   |   |
*___*___*___*
*   *   *   *
|   | o |   |
*___*___*___*
*   *   *   *
|   |   |   |
*___*___*___*

So, when counting what terrain the camera is in, we subtract half the terrain length for the first (center) terrain
  and then subtract the full length for the rest.

*/
void TerrainManager::Update(Vec3 newCameraPos)
{
	//New (x, z) terrain location of the camera
	int newX, newZ;

	//First, we update the current terrain if it needs to be
	
}

void TerrainManager::Render()
{
}