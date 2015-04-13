//SDLGame.cpp - Implementation of the main game object, containing everything that has to do with the game.
//Programmed by Ed Lu

#include "SDLGame.h"

SDLGame theGame;

inline bool genTerrain(std::vector<std::string> *vArgs)
{
	GLConsole* pConsole = GetConsole();

	theGame.terrain.generateTerrain(theGame.baseHeight, theGame.randScale, theGame.scaleDiv);
	theGame.camera.setPos(Vec3(0, theGame.terrain.getHeight(theGame.terrain.getLength()/2, theGame.terrain.getLength()/2) + 10, 0));
	
	return true;
}

inline bool getPolyCount(std::vector<std::string> *vArgs)
{
	GLConsole* pConsole = GetConsole();
	if(!theGame.drawQuad)
		pConsole->Printf("Polys: %i", theGame.polyCount);
	else
		pConsole->Printf("Polys: %i \t Verts: %i", theGame.terrain.numTrgls, theGame.terrain.numVerts);

	return true;
}

inline bool getCameraPos(std::vector<std::string> *vArgs)
{
	GLConsole* pConsole = GetConsole();
	pConsole->Printf("Camera Pos: (%.2f, %.2f, %.2f)", theGame.camera.getPos().x, theGame.camera.getPos().y, theGame.camera.getPos().z);

	return true;
}

SDLGame::SDLGame() :
		lighting(CVarUtils::CreateCVar<bool>("lighting", 1, "Lighting on/off")),
		drawNormals(CVarUtils::CreateCVar<bool>("drawNormals", 0, "Draw Normals")),
		wireframe(CVarUtils::CreateCVar<bool>("wireframe", 0, "Wireframe on/off")),
		drawQuad(CVarUtils::CreateCVar<bool>("terrain.drawQuad", 1, "Drawing using quadtree on/off")),
		baseHeight(CVarUtils::CreateCVar<float>("terrain.baseHeight", BASE_HEIGHT, "Base height at generation")),
		randScale(CVarUtils::CreateCVar<float>("terrain.randScale", RAND_SCALE, "Randomness of generation")),
		scaleDiv(CVarUtils::CreateCVar<float>("terrain.scaleDiv", SCALE_DIV, "Smoothness of generation")),
        wtf(CVarUtils::CreateCVar<float>("wtf", 100.0, "lol"))
{
	polyCount = 0;

	//Initialize variables
	keyW = keyA = keyS = keyD = false;
	rotateTime = 33;
	rotateTimer = Timer(rotateTime);

	Running = true;
    screen = NULL;

	CVarUtils::CreateCVar("polyCount", getPolyCount, "Get the current polygon count");
	CVarUtils::CreateCVar("terrain.generate", genTerrain, "Generates a new terrain with arguments (baseHeight, randScale, scaleDiv)");
	CVarUtils::CreateCVar("camera.getPos", getCameraPos, "Gets the current position of the camera");

	terrain.generateTerrain(baseHeight, randScale, scaleDiv);
	//terrain.flatTerrain();
	/*for(int x = 0; x < terrain.l; x++)
	{
		for(int z = 0; z < terrain.l; z++)
		{
			if(x >= 100)
				terrain.setHeight(x, z, x);
			else
				terrain.setHeight(x, z, 100);
		}
	}*/
	
	//terrain.computeNormals();
	terrain.Update(0, 0, terrain.getLength() - 1);

	/*terrain.flatTerrain();
	terrain.setActive(0, 0);
	terrain.setActive(4, 0);
	terrain.setActive(8, 0);
	terrain.setActive(6, 2);
	terrain.setActive(8, 2);
	terrain.setActive(7, 3);
	terrain.setActive(4, 4);
	terrain.setActive(6, 4);
	terrain.setActive(8, 4);
	terrain.setActive(6, 6);
	terrain.setActive(0, 8);
	terrain.setActive(4, 8);
	terrain.setActive(8, 8);*/

	camera.setPos(Vec3(0, terrain.getHeight(terrain.getLength()/2, terrain.getLength()/2) + 10, 0));
}

bool SDLGame::Initialize()
{
	//Set up the SDL window
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

	if((screen = SDL_CreateWindow("TerrGen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN)) == NULL)
        return false;

    context = SDL_GL_CreateContext(screen);

	//Do openGL initialization
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)WIN_WIDTH / WIN_HEIGHT, 1.0, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);

	GLfloat mat_specular[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat mat_emission[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_position[] = {1.0, 1.0, 0.0, 0.0};

	GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	//glLightfv(GL_FRONT, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE) ;

    GlutFakeInit();

	//Set the background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glLoadIdentity();

	//Initialize the library used to load images
	ilInit();
	iluInit();
	ilutInit();

	//Load some textures
	textureIDs = new GLuint[NUM_TEXTURES];
	glGenTextures(NUM_TEXTURES, textureIDs);
	//textureIDs[0] = ilutGLLoadImage("Content\\GroundTex.png");
	//glBindTexture(GL_TEXTURE_2D, textureIDs[0]);	//Bind the texture

	//Hide the mouse cursor
	SDL_ShowCursor(0);

    //Initialize glconsole now that we have opengl context
    console.Init();

	//Successful initialization
	return true;
}

int SDLGame::Execute()
{
	//Something went wrong in initialization
	if(Initialize() == false)
        return -1;

	rotateTimer.Start();
    SDL_Event Event;

	//Main game loop
    while(Running) {
		//Process events
        while(SDL_PollEvent(&Event))
            ProcessEvent(Event);
		//Run the main game logic code
        Loop();
		//Render necessary object
        Render();
    }

	//Cleanup as necessary
    Cleanup();

    return 0;
}

void SDLGame::ProcessEvent(SDL_Event& Event)
{
	//If the console is open, feed input to the console
	if(console.IsOpen())
	{
		if(Event.type == SDL_TEXTINPUT)
		{
			//Get the key pressed
			char *text = Event.text.text;

			//If the character is `, close the console
			//If the character entered is not a modifier, treat it as a keypress
            if(*text != '`')
				console.KeyboardFunc(text[0]);

			//console.Printf("Key down: %i", Event.key.keysym.unicode);
        }
        else if (Event.type == SDL_KEYDOWN) {
            SDL_Keycode keyPress = Event.key.keysym.sym;
            if (Event.key.keysym.sym == SDLK_BACKQUOTE) {
                console.ToggleConsole();
            } else if (Event.key.keysym.sym == SDLK_RETURN) {
                console.KeyboardFunc('\r');
            } else if (Event.key.keysym.sym == SDLK_TAB) {
                console.KeyboardFunc('\t');
            } else if (Event.key.keysym.sym == SDLK_BACKSPACE) {
                console.KeyboardFunc('\b');
            }
        }
	}
	//If the console isn't open, do stuff
	else
	{
		//Check what type of event it was
        switch (Event.type)
        {
        case SDL_KEYDOWN:							//Key was pressed
            switch (Event.key.keysym.sym)
            {
            case SDLK_BACKQUOTE:				//Tilde, open console
                console.ToggleConsole();
                break;
            case SDLK_ESCAPE:					//Escape, stop running
                Running = false;
                break;
            case SDLK_d:						//D, set keyD to true
                keyD = true;
                break;
            case SDLK_a:						//A, set keyA to true
                keyA = true;
                break;
            case SDLK_w:						//W, set keyW to true
                keyW = true;
                break;
            case SDLK_s:						//S, set keyS to true
                keyS = true;
                break;
            case SDLK_SPACE:					//Space, increment camera's movement modifier
                camera.incMoveModifier();
                break;
            case SDLK_LCTRL:					//Ctrl, decrement camera's movement modifier
                camera.decMoveModifier();
                break;
            }
            break;
        case SDL_KEYUP:								//Key was released
            switch (Event.key.keysym.sym)
            {
            case SDLK_d:						//D, set keyA to false
                keyD = false;
                break;
            case SDLK_a:						//A, set keyA to false
                keyA = false;
                break;
            case SDLK_w:						//W, set keyW to false
                keyW = false;
                break;
            case SDLK_s:						//S, set keyS to false
                keyS = false;
                break;
            }
            break;
        case SDL_MOUSEMOTION:						//Mouse motion - Rotate camera
            camera.updateRot((Event.motion.x - WIN_WIDTH / 2) * .1, (Event.motion.y - WIN_HEIGHT / 2) * .1);
            break;
        case SDL_WINDOWEVENT:
            if (Event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                hasFocus = true;
            }
            else if (Event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                hasFocus = false;
            }
			break;
		case SDL_QUIT:								//Quit event - stop running
			Running = false;
			break;
		}
	}
}

void SDLGame::Loop()
{
	if(keyW)
		camera.Move(true);
	if(keyS)
		camera.Move(false);
	if(keyD)
		camera.Strafe(true);
	if(keyA)
		camera.Strafe(false);

	//Get the lighting state
	if(lighting)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	//If the window has focus, set the mouse to the center so it doesn't go out of bounds
	if(hasFocus)
		SDL_WarpMouseInWindow(screen, WIN_WIDTH/2, WIN_HEIGHT/2);
}

void SDLGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	//Wireframe
	if(wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Get the pointing vector and the up vector
	Vec3 pointVec = camera.getPointVec(0,0);
	Vec3 rotVec = camera.getPointVec(0, 90);

	//OK MEN:
	// Position of the camera is just the stored position
	// The point the camera is looking at is gotten by the position plus a point on the unit circle. That point is gotten by converting
	//   the horizontal/vertical angles from spherical coordinates (r = 1, theta = angleHoriz, phi = angleVert) to cartesian.
	// The up vector is the point vector rotated 90 degrees and projected into the XZ plane. It also points in the Y direction. Quite
	//   honestly, I am not sure how this part works. Just say it works on intuition and magic.
	gluLookAt(camera.getPos().x, camera.getPos().y, camera.getPos().z,
		camera.getPos().x + pointVec.x, camera.getPos().y + pointVec.y, camera.getPos().z + pointVec.z,
		rotVec.x, 1.0, rotVec.z);

	//Center terrain
    glTranslatef(-float(terrain.getLength()) / 2, 0.0f, -float(terrain.getLength()) / 2);

	polyCount = 0;
	if(!drawQuad)
	{
		//glEnable(GL_TEXTURE_2D);
		for(int z = 0; z < terrain.getLength() - 1; z++)
		{
			Vec3 normal;
			//Makes OpenGL draw a triangle at every three consecutive vertices
			glBegin(GL_TRIANGLE_STRIP);
			for(int x = 0; x < terrain.getLength(); x++)
			{
				//glTexCoord2f((float)(x % 2), (float)(z % 2));
				normal = terrain.getNormal(x, z);
				glNormal3f(normal.x, normal.y, normal.z);
				glColor3f(normal.x * normal.x, normal.y * normal.y, normal.z * normal.z);
				glVertex3f(x, terrain.getHeight(x, z), z);

				//glTexCoord2f((float)(x % 2), (float)((z+1) % 2));
				normal = terrain.getNormal(x, z+1);
				glNormal3f(normal.x, normal.y, normal.z);
				glColor3f(normal.x * normal.x, normal.y * normal.y, normal.z * normal.z);
				glVertex3f(x, terrain.getHeight(x, z + 1), z + 1);

				polyCount+=2;
			}
			glEnd();
		}
	}
	else
	{
		terrain.Render(0, 0, terrain.getLength() - 1);
		terrain.computedStats = true;
	}

	/*glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(50.0f, -1.0f, 50.0f);
	glVertex3f(100.0f, -1.0f, 50.0f);
	glVertex3f(100.0f, -1.0f, 512.0f);
	glVertex3f(50.0f, -1.0f, 512.0f);
	glEnd();*/

	if(drawNormals)
	{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glColor3f(0.0f, 0.0f, 0.0f);
		Vec3 normal;

		for(int z = 0; z < terrain.getLength(); z++)
			for(int x = 0; x < terrain.getLength(); x++)
			{
				glBegin(GL_LINES);
				normal = terrain.getNormal(x,z) * 2.5;
				glVertex3f(x, terrain.getHeight(x, z), z);
				glVertex3f(x + normal.x, terrain.getHeight(x, z) + normal.y, z + normal.z);
				glEnd();
			}
	}

	//Render the GLConsole, and render it last so it's on top of everything
    if(console.IsOpen())
        console.RenderConsole();

	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR)
	{
		errString = gluErrorString(errCode);
		printf("OpenGL Error: %s\n", errString);
	}

    SDL_GL_SwapWindow(screen);
}

void SDLGame::Cleanup()
{
	SDL_Quit();
}

void dirtColorage(Vec3 normal)
{
	if(normal.y * normal.y < .4)
		glColor3f(.4, .3, .3);
	else if(normal.y * normal.y < .7)
		glColor3f((1 - normal.y * normal.y), (normal.x * normal.x + normal.z * normal.z) * .75, (normal.x * normal.x + normal.z * normal.z) * .35);
	else
		glColor3f(.2, normal.y * normal.y, .2);
}

void SDLGame::TestBox()
{
	//TESTBOX
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(1.5f, -2.0f, 1.5f);
	glVertex3f(-1.5f, -2.0f, 1.5f);
	glVertex3f(-1.5f, -2.0f, -1.5f);
	glVertex3f(1.5f, -2.0f, -1.5f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);				//Front
    glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.5f, -1.0f, 1.5f);
	glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.5f, -1.0f, 1.5f);
	glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.5f, 1.0f, 1.5f);
	glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.5f, 1.0f, 1.5f);
	glEnd();						//End Front
    
	glBindTexture(GL_TEXTURE_2D, textureIDs[1]);
	glBegin(GL_QUADS);				//Right
    glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.5f, -1.0f, -1.5f);
	glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.5f, 1.0f, -1.5f);
	glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.5f, 1.0f, 1.5f);
	glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.5f, -1.0f, 1.5f);
	glEnd();						//End Right
    
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);				//Back
    glNormal3f(0.0f, 0.0f, -1.0f);
	glColor3f(.5, .1, .8);
    glVertex3f(-1.5f, -1.0f, -1.5f);
	glColor3f(.46, .6, .2);
    glVertex3f(-1.5f, 1.0f, -1.5f);
	glColor3f(.9, .7, .1);
    glVertex3f(1.5f, 1.0f, -1.5f);
	glColor3f(.4, .3, .1);
    glVertex3f(1.5f, -1.0f, -1.5f);
    
									//Left
    glNormal3f(-1.0f, 0.0f, 0.0f);
	glColor3f(1, 1, 0);
    glVertex3f(-1.5f, -1.0f, -1.5f);
    glVertex3f(-1.5f, -1.0f, 1.5f);
    glVertex3f(-1.5f, 1.0f, 1.5f);
    glVertex3f(-1.5f, 1.0f, -1.5f);
    
    glEnd();
}

void SDLGame::GlutFakeInit() {
    char *fake_argv[] = { "fake", NULL };
    int fake_argc = 1;
    glutInit(&fake_argc, fake_argv);
}