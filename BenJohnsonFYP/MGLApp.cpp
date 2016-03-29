/*************************************************************************

*************************************************************************/

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#define WIN32_EXTRA_LEAN
	#include <windows.h>
#endif

#include "MGLApp.h"

int seeds[MAPSIZE][MAPSIZE] = {(0)};
mapType mapTypeArray[500][500] = {(mapType::MIDPOINT)};

MGLApp::MGLApp()
{
}

MGLApp::~MGLApp()
{
}

bool MGLApp::Init(HDC g_hDC)
{	
	ghDC = g_hDC;

	if (!GLeeInit())
		return false;

	// Enable Depth Testing.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	// Enable Culling.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// Enable Fogging.
	glEnable(GL_FOG);
	glFogf(GL_FOG_START, 200); 
	glFogf(GL_FOG_END, 1500);

	//GLfloat fogcolor[4] = {.6196078f, .6588235f, .7647058f, 0.0f};
	GLfloat fogcolor[4] = {.2, .2, .8, 0.0f};
	glFogfv(GL_FOG_COLOR, fogcolor);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_DENSITY, 0.5f);
	glHint(GL_FOG_HINT, GL_NICEST);

	// Set the clear color to a nice sky blue.
	glClearColor(.7372549f, .7803921f, .9647058f, 1.0f);


	// Load our font.
	if (!font.load("font.tga", "font.dat"))
		return false;

	MapInfo::initMapArrays();
	MapInfo::initMapInfo();
	mapNode* temp = mapList::current;


	world = new mapHeightData*[MAPSIZE];
	//worldCopy = new mapHeightData*[MAPSIZE];
	for(int i = 0; i < MAPSIZE; i++)
	{
		world[i] = new mapHeightData[MAPSIZE];
		//worldCopy[i] = new mapHeightData[MAPSIZE];

	}

	// Initialize the toggle variables.
	wireframe = false;
	boundingBox = false;

	//playerPosX = 2000;
	//playerPosY = 2000;
	// Initialize the movement variables.
	rotateX = 0;
	rotateY = 0;
	translateX = 0;
	translateY = 0;
	translateZ = 0;

	timeSinceInput = 0.0;
	msgbuf = new char[100];
	//DebugString(std::string("Time for map: %d ms\n", GetTickCount()));
	return true;
}


bool MGLApp::Shutdown()
{
	return true;
}

void MGLApp::SetupProjection(int width, int height)
{
	// avoid div by zero
	if( height == 0 )
		height = 1;					

	// set up GL viewport
	glViewport( 0, 0, width, height );

	// set up a perspective projection
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 45.0f, (GLfloat)width/(GLfloat)height, 1.0f, 1000.0f );

	// Give the window dimentions to the font class.
	font.setDimentions(width, height);

	// Save the width and height for later use.
	sWidth = width;
	sHeight = height;
}

void MGLApp::Update( double dt )
{
	timeSinceInput += dt;

	// If the '1' key is pressed, create a heightmap using the hill algorithm.
	if ((GetAsyncKeyState('1') & 0x8000))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		font.display("Creating Heightmap With:", 1, 30);
		font.display("Hill Algorithm", 1, 70);
		SwapBuffers(ghDC);
		
		long int currentTime = GetTickCount();
		heightmap.createHeightmap(CHeightClass::Hill);
		if(DEBUG_FLAG)
		{
			sprintf(msgbuf, "Time for heightmap Hill: %d ms\n", GetTickCount() - currentTime);
			OutputDebugString(msgbuf);
			currentTime = GetTickCount();
			mapList::current->terrainData->init(heightmap.getHeightData());
			sprintf(msgbuf, "Time for terrain: %d ms\n", GetTickCount() - currentTime);
			OutputDebugString(msgbuf);
			currentTime = GetTickCount();
		}
		else
			mapList::current->terrainData->init(heightmap.getHeightData());
	}

	// If the '2' key is pressed, create a heightmap using the fault line algorithm.
	if ((GetAsyncKeyState('2') & 0x8000))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		font.display("Creating Heightmap With:", 1, 30);
		font.display("Fault Line Algorithm", 1, 70);
		SwapBuffers(ghDC);

		long int currentTime = GetTickCount();

		heightmap.createHeightmap(CHeightClass::FaultLine);
		if(DEBUG_FLAG)
		{
			sprintf(msgbuf, "Time for heightmap FaultLine: %d ms\n", GetTickCount() - currentTime);
			OutputDebugString(msgbuf);
			currentTime = GetTickCount();
			mapList::current->terrainData->init(heightmap.getHeightData());
			sprintf(msgbuf, "Time for terrain: %d ms\n", GetTickCount() - currentTime);
			OutputDebugString(msgbuf);
			currentTime = GetTickCount();
		}
		else
			mapList::current->terrainData->init(heightmap.getHeightData());
	}

	// If the '3' key is pressed, create a heightmap using midpoint displacement.
	if ((GetAsyncKeyState('3') & 0x8000))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		font.display("Creating Heightmap With:", 1, 30);
		font.display("Midpoint Algorithm", 1, 70);
		SwapBuffers(ghDC);

		long int currentTime = GetTickCount();

		heightmap.createHeightmap(CHeightClass::Midpoint);
		if(DEBUG_FLAG)
		{
			sprintf(msgbuf, "Time for heightmap Midpoint: %d ms\n", GetTickCount() - currentTime);
			OutputDebugString(msgbuf);
			currentTime = GetTickCount();
			mapList::current->terrainData->init(heightmap.getHeightData());
			sprintf(msgbuf, "Time for terrain: %d ms\n", GetTickCount() - currentTime);
			OutputDebugString(msgbuf);
			currentTime = GetTickCount();
		}
		else
			mapList::current->terrainData->init(heightmap.getHeightData());
	}

	if ((GetAsyncKeyState('4') & 0x8000)&&
		timeSinceInput > .25)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		font.display("Calculating path between two points.", 1, 30);
		SwapBuffers(ghDC);
	}

	if((GetAsyncKeyState('5') & 0x8000) &&  //PATHFINDING STUFF!!!!!!!!!!
	timeSinceInput > .25)
	{
		timeSinceInput = 0;
		glClear(GL_COLOR_BUFFER_BIT);
		font.display("Calculating ray between here and the corner", 1, 30);
		SwapBuffers(ghDC);

		int playerPosX = 1500;
		int playerPosY = 3500;
		int endPosX = 2000;
		int endPosY = 5000;

		world = new mapHeightData*[MAPSIZE];
	    worldCopy = new mapHeightData*[MAPSIZE];
		for(int i = 0; i < MAPSIZE; i++)
		{
			world[i] = new mapHeightData[MAPSIZE];
			worldCopy[i] = new mapHeightData[MAPSIZE];

		}

		//CalculateMapCollisions(mapList::current->x, mapList::current->y, endX, endY);
		CalculateMapCollisions(playerPosX, playerPosY, endPosX, endPosY);
		linkedList<GraphNode> * path;
		
		if(world[100][200].mapChunk == NULL)
			int test = 1;

		int xPos, yPos = 0;
		int currentMap = 0;
		bool finishedGen = false;
		long int startTime = GetTickCount();
		long int currentTime = GetTickCount();

		/*#pragma omp parallel sections
		{
			#pragma omp section
			{
				#pragma omp parallel for shared(path, finishedGen)*/
				for(int i = 0; i < numOfCells; i++)
				{
					xPos = mapCrossovers[i][0];
					yPos = mapCrossovers[i][1];
					world[xPos][yPos].mapChunk = new chunkInfo*[32];
			
					for(int j = 0; j < 32; j++)
					{
						world[xPos][yPos].mapChunk[j] = new chunkInfo[32];
						for(int k = 0; k < 32; k++)
						{
							world[xPos][yPos].mapChunk[j][k].heightGenerated= true;
						}
					}
					sprintf(msgbuf, "Seed chunks: %d ms\n", GetTickCount() - currentTime);
					OutputDebugString(msgbuf);
					currentTime = GetTickCount();

					heightmap.createHeightmap(mapTypeArray[xPos][yPos], seeds[xPos][yPos]);

					sprintf(msgbuf, "Create heightmap: %d ms\n", GetTickCount() - currentTime);
					OutputDebugString(msgbuf);
					currentTime = GetTickCount();

					heightmap.createChunkInfo(33, 33, &world[xPos][yPos]);

					sprintf(msgbuf, "Pass in chunk info: %d ms\n", GetTickCount() - currentTime);
					OutputDebugString(msgbuf);
					currentTime = GetTickCount();

					currentMap++;
					OutputDebugString("Created map\n");
			
					sprintf(msgbuf, "Time for map %d: %d ms\n", currentMap, GetTickCount() - currentTime);
					OutputDebugString(msgbuf);
					currentTime = GetTickCount();
				}
			
		
			sprintf(msgbuf, "Total time: %d ms\n", GetTickCount() - currentTime);
			OutputDebugString(msgbuf);
			finishedGen = true;
			path = AStarPath(playerPosX, playerPosY, endPosX, endPosY, world);
			int i = 0;
			/*}
			#pragma omp section
			{
				while(!finishedGen)
				{
					//std::copy(&world[0][0], &world[0][0]+MAPSIZE*MAPSIZE*sizeof(mapHeightData), &worldCopy[0][0]);
					worldCopy = world;
					linkedList<GraphNode> * shortPath = AStarPath(playerPosX, playerPosY, endPosX, endPosY, world);

				}
			}
		//Finished calculating path
		}*/
	}


	if((GetAsyncKeyState('6') & 0x8000) &&
	timeSinceInput > .5)
	{
		timeSinceInput = 0;
		mapNode* temp = mapList::current;
		mapList::current = mapList::north;
		mapList::north = mapList::east;
		mapList::east = mapList::south;
		mapList::south = mapList::west;
		mapList::west = temp;

	}

	// If the F1 key is pressed, toggle wireframe on and off.
	if ((GetAsyncKeyState(VK_F1) & 0x8000) &&
		timeSinceInput > .25)
	{
		timeSinceInput = 0;

		wireframe = !wireframe;
		if (wireframe)
			glPolygonMode(GL_FRONT, GL_LINE);	
		else
			glPolygonMode(GL_FRONT, GL_FILL);
	}

	// If the F2 key is pressed, toggle bounding boxes.
	if ((GetAsyncKeyState(VK_F2) & 0x8000) &&
		timeSinceInput > .25)
	{
		timeSinceInput = 0;

		boundingBox = !boundingBox;
	}

	if ((GetAsyncKeyState(VK_F4) & 0x8000) &&
		timeSinceInput > .25)
	{
		timeSinceInput = 0;
		this->heightmap.printHeights();
	}

	// Movement input.
	if ((GetAsyncKeyState(VK_UP) & 0x8000))
		rotateX += dt*18;
	if ((GetAsyncKeyState(VK_DOWN) & 0x8000))
		rotateX -= dt*18;
	if ((GetAsyncKeyState(VK_LEFT) & 0x8000))
		rotateY -= dt*36;
	if ((GetAsyncKeyState(VK_RIGHT) & 0x8000))
		rotateY += dt*36;
	if ((GetAsyncKeyState('W') & 0x8000))
		translateZ -= dt*100;
	if ((GetAsyncKeyState('S') & 0x8000))
		translateZ += dt*100;
	if ((GetAsyncKeyState('A') & 0x8000))
		translateX -= dt*100;
	if ((GetAsyncKeyState('D') & 0x8000))
		translateX += dt*100;
	if ((GetAsyncKeyState('Q') & 0x8000))
		translateY -= dt*100;
	if ((GetAsyncKeyState('Z') & 0x8000))
		translateY += dt*100;

}

void MGLApp::Render()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	// set up the camera
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	// Rotate to face the heightmap.
	glRotatef(180, 0, 1, 0);

	// Add the rotation we got from input.
	glRotatef(rotateX, 1, 0, 0);
	glRotatef(rotateY, 0, 1, 0);

	// Start in a position to see the heightmap.
	glTranslatef(-512, -256, 0);

	// Add the movement we got from input.
	glTranslatef(translateX, translateY, translateZ);

	int currentXNode = 0;
	int currentYNode = 0;
	// Display the terrain mesh.
	mapList::current->terrainData->draw(boundingBox, drawSpheres, currentXNode, currentYNode);

}



void MGLApp::CalculateMapCollisions(int playerPosX, int playerPosY, int endPosX, int endPosY)
{

	int startX = std::floor(playerPosX / 32.0f); //Calculate map to start from
	int startY = std::floor(playerPosY / 32.0f); //Calculate map to finish from
	int endX = std::floor(endPosX / 32.0f);
	int endY = std::floor(endPosY / 32.0f);

	mapCrossovers = new int *[(3 * (MAPSIZE - 1) + 1)]; //Works for square maps, maximum number of maps that need to be generated. 
	//mapCrossovers = new int*[(5 * (MAPSIZE - 4) + 4)]; //Works for square maps that generate 8 cardinal directions instead of 4
	for(int i = 0; i < 2000; i++)
	{
		mapCrossovers[i] = new int[2]; //[n][0] is x, [n][1] is y
	}
	int currentVectorPos = 0;
	mapCrossovers[currentVectorPos][0] = startX;
	mapCrossovers[currentVectorPos][1] = startY;
	
	if(DEBUG_FLAG)
	{

		sprintf(msgbuf, "%d %d \n", mapCrossovers[currentVectorPos][0], mapCrossovers[currentVectorPos][1]);
		OutputDebugString(msgbuf);
	}

	int xDist = endX - startX;
	int yDist = endY - startY;
	float length = sqrt(pow((float)xDist, 2) + pow((float)yDist, 2));
	for(int i = 0; i < 4000; i++) //poll line 1000 times (0.1% per check)
	{
		float currX = (startX + 0.5); //Centre of current map
		float currY = (startY + 0.5); //Centre of current map
		currX += 0.00025 * i * xDist;
		currY += 0.00025 * i * yDist;

		currX = floor(currX);
		currY = floor(currY);
		if(currX != mapCrossovers[currentVectorPos][0] || currY != mapCrossovers[currentVectorPos][1])
		{
			if(currX != mapCrossovers[currentVectorPos][0] && currY != mapCrossovers[currentVectorPos][1]) //Check to see if a map grid has been skipped, currently unable to travel diagonally
			{
				if(xDist > 0)
				{
					if(yDist > 0) // South east, add south and east
					{
						currentVectorPos++;
						mapCrossovers[currentVectorPos][0] = (int)currX;
						mapCrossovers[currentVectorPos][1] = (int)currY - 1;
						currentVectorPos++;
						mapCrossovers[currentVectorPos][0] = (int)currX - 1;
						mapCrossovers[currentVectorPos][1] = (int)currY;
						if(DEBUG_FLAG) //Debug
						{

							sprintf(msgbuf, "Added adjacent cells\n");
							OutputDebugString(msgbuf);
						}
					}
					else if (yDist < 0) //North east, add north and east
					{
						currentVectorPos++;
						mapCrossovers[currentVectorPos][0] = (int)currX;
						mapCrossovers[currentVectorPos][1] = (int)currY + 1;
						currentVectorPos++;
						mapCrossovers[currentVectorPos][0] = (int)currX - 1;
						mapCrossovers[currentVectorPos][1] = (int)currY;
						if(DEBUG_FLAG) //Debug
						{

							sprintf(msgbuf, "Added adjacent cells\n");
							OutputDebugString(msgbuf);
						}
					}
				}
				else if (xDist < 0)
				{
					if(yDist > 0) // South west, add south and west
					{
						currentVectorPos++;
						mapCrossovers[currentVectorPos][0] = (int)currX;
						mapCrossovers[currentVectorPos][1] = (int)currY - 1;
						currentVectorPos++;
						mapCrossovers[currentVectorPos][0] = (int)currX + 1;
						mapCrossovers[currentVectorPos][1] = (int)currY;
						if(DEBUG_FLAG) //Debug
						{

							sprintf(msgbuf, "Added adjacent cells\n");
							OutputDebugString(msgbuf);
						}
					}
					else if (yDist < 0) //North west, add north and west
					{
						currentVectorPos++;
						mapCrossovers[currentVectorPos][0] = (int)currX;
						mapCrossovers[currentVectorPos][1] = (int)currY + 1;
						currentVectorPos++;
						mapCrossovers[currentVectorPos][0] = (int)currX + 1;
						mapCrossovers[currentVectorPos][1] = (int)currY;
						if(DEBUG_FLAG) //Debug
						{
							sprintf(msgbuf, "Added adjacent cells\n");
							OutputDebugString(msgbuf);
						}
					}
				}
			}
			currentVectorPos++;
			mapCrossovers[currentVectorPos][0] = (int)currX;
			mapCrossovers[currentVectorPos][1] = (int)currY;
			numOfCells = currentVectorPos + 1;

			if(DEBUG_FLAG) //Debug
			{

				sprintf(msgbuf, "%d %d \n", mapCrossovers[currentVectorPos][0], mapCrossovers[currentVectorPos][1]);
				OutputDebugString(msgbuf);
			}

		}
	}
	

	if(DEBUG_FLAG) //Debug
	{
		OutputDebugString("Finished potential calculations!\n");
		std::ofstream testFile;
		testFile.open("ListofMaps.txt");
		int j = 0;
		while(mapCrossovers[j][0] != endX || mapCrossovers[j][1] != endY)
		{
			testFile << mapCrossovers[j][0];
			testFile << " ";
			testFile << mapCrossovers[j][1];
			testFile << "\n";
			j++;
		}
		testFile << "Number of maps in total: ";
		testFile << j;
		testFile.close();
	}

}

void MGLApp::DebugString(std::string inStr)
{
	if(DEBUG_FLAG)
	{
		strcpy(msgbuf, inStr.c_str());
		OutputDebugString(msgbuf);
	}
}