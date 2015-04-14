//SDLGame.cpp - Implementation of the main game object, containing everything that has to do with the game.
//Programmed by Ed Lu

#pragma once

#include <string>

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <ctime>

#include "Timer.h"
#include "Vec3.h"
#include "Camera.h"
#include "Terrain.h"

#include <glconsole/glconsole.h>

#define NUM_TEXTURES 32
#define WIN_WIDTH  1024
#define WIN_HEIGHT 768
#define GENERATEDEBUG 1

class SDLGame
{
public:
	SDLGame();
	int Execute();
	bool Initialize();
	void ProcessEvent(SDL_Event& Event);
	void Loop();
	void Render();
	void Cleanup();

    void TestBox();
    void GlutFakeInit();

    SDL_Window *screen;
    SDL_GLContext context;
	bool Running;

	Camera camera;		//Camera in 3D space
	int rotateTime;		//Amount of time between updating rotations (30 fps, 1000 milliseconds/30 = 33.333)
	Timer rotateTimer;	//Timer to keep track of FPS
	GLuint *textureIDs;	//Textures loaded by devIL
	GLConsole console;	//GLConsole object
	Terrain terrain;	//Terrain object

	bool& wireframe;	//Wireframe on/off
	bool& lighting;		//Lighting on/off
	bool& drawNormals;	//Draw normals on/off
	bool& drawQuad;		//Draw using quadtree on/off

	//CVars for the terrain generation
	float& baseHeight;
	float& randScale;
	float& scaleDiv;

	bool hasFocus;		//Does the window have focus?
	int polyCount;
	
	bool keyW, keyA, keyS, keyD;

    static SDLGame &getInstance();
};