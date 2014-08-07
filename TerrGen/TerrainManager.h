//TerrainManager.h - Declaration of the terrain manager, which holds the list of terrains generated and manages
//  the generation of new blocks.
//Programmed by Ed Lu

#include <list>

#include "Terrain.h"

class TerrainManager
{
private:
	Terrain* center;		//The terrain at (0,0)
	Terrain* current;		//The terrain the player is currently on
	int curX, curZ;			//The current X and Z of the terrain (if it were in an array)
public:
	//Constructor
	TerrainManager();

	//Run this every frame to update what terrain the player is on and what terrains to render
	void Update(Vec3 cameraPos);
	//Render all terrains that need to be
	void Render();
};