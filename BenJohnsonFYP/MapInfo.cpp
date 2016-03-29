

#include "MapInfo.h"

mapNode* mapList::current = NULL;
mapNode* mapList::east = NULL;
mapNode* mapList::west = NULL;
mapNode* mapList::north = NULL;
mapNode* mapList::south = NULL;

void moveNorth()
{
	CHeightClass heightmap;
	delete mapList::south;
	delete mapList::east;
	delete mapList::west;
	mapList::south = mapList::current;
	mapList::current = mapList::north;

	heightmap.createHeightmap((int)mapTypeArray[mapList::current->x][mapList::current->y - 1], seeds[mapList::current->x][mapList::current->y - 1]);
	mapList::north->terrainData->init(heightmap.getHeightData());

	heightmap.createHeightmap((int)mapTypeArray[mapList::current->x + 1][mapList::current->y], seeds[mapList::current->x + 1][mapList::current->y]);
	mapList::east->terrainData->init(heightmap.getHeightData());

	heightmap.createHeightmap((int)mapTypeArray[mapList::current->x - 1][mapList::current->y], seeds[mapList::current->x - 1][mapList::current->y]);
	mapList::west->terrainData->init(heightmap.getHeightData());
}

void moveSouth()
{
	delete mapList::north;
	delete mapList::east;
	delete mapList::west;
	mapList::north = mapList::current;
	mapList::current = mapList::south;

	CHeightClass heightmap;
	heightmap.createHeightmap(mapTypeArray[mapList::current->x][mapList::current->y + 1], seeds[mapList::current->x][mapList::current->y + 1]);
	mapList::north->terrainData->init(heightmap.getHeightData());

	heightmap.createHeightmap(mapTypeArray[mapList::current->x + 1][mapList::current->y], seeds[mapList::current->x + 1][mapList::current->y]);
	mapList::east->terrainData->init(heightmap.getHeightData());

	heightmap.createHeightmap(mapTypeArray[mapList::current->x - 1][mapList::current->y], seeds[mapList::current->x - 1][mapList::current->y]);
	mapList::west->terrainData->init(heightmap.getHeightData());
}

void moveEast()
{
	delete mapList::south;
	delete mapList::north;
	delete mapList::west;
	mapList::west = mapList::current;
	mapList::current = mapList::east;

	CHeightClass heightmap;
	heightmap.createHeightmap(mapTypeArray[mapList::current->x][mapList::current->y - 1], seeds[mapList::current->x][mapList::current->y - 1]);
	mapList::north->terrainData->init(heightmap.getHeightData());

	heightmap.createHeightmap(mapTypeArray[mapList::current->x + 1][mapList::current->y], seeds[mapList::current->x + 1][mapList::current->y]);
	mapList::east->terrainData->init(heightmap.getHeightData());

	heightmap.createHeightmap(mapTypeArray[mapList::current->x][mapList::current->y + 1], seeds[mapList::current->x][mapList::current->y + 1]);
	mapList::west->terrainData->init(heightmap.getHeightData());
}

static void moveWest()
{
	delete mapList::south;
	delete mapList::north;
	delete mapList::east;
	mapList::east = mapList::current;
	mapList::current = mapList::west;

	CHeightClass heightmap;
	heightmap.createHeightmap(mapTypeArray[mapList::current->x][mapList::current->y - 1], seeds[mapList::current->x][mapList::current->y - 1]);
	mapList::north->terrainData->init(heightmap.getHeightData());

	heightmap.createHeightmap(mapTypeArray[mapList::current->x - 1][mapList::current->y], seeds[mapList::current->x - 1][mapList::current->y]);
	mapList::east->terrainData->init(heightmap.getHeightData());

	heightmap.createHeightmap(mapTypeArray[mapList::current->x][mapList::current->y + 1], seeds[mapList::current->x][mapList::current->y + 1]);
	mapList::west->terrainData->init(heightmap.getHeightData());
}

void MapInfo::initMapArrays()
{
	int counter = 0;
	for (int x = 0; x < MAPSIZE; x++)
	{
		for (int y = 0; y < MAPSIZE; y++)
		{
			seeds[x][y] = counter++;
			mapTypeArray[x][y] = MIDPOINT;
		}
	}
}

void MapInfo::initMapInfo()
{
	mapList::mapList();
	/*int counter = 0;
	for (int x = 0; x < MAPSIZE; x++)
	{
		for (int y = 0; y < MAPSIZE; y++)
		{
			seeds[x][y] = counter++;
			mapTypeArray[x][y] = MIDPOINT;
		}
	}*/

	CHeightClass heightmap;


	mapList::current = new mapNode(MAPSIZE/2, MAPSIZE/2, MIDPOINT);
	//mapNode* temp = mapList::current;
	mapList::north = new mapNode(MAPSIZE/2, MAPSIZE/2 - 1, MIDPOINT);
	mapList::south = new mapNode(MAPSIZE/2, MAPSIZE/2 + 1, MIDPOINT);
	mapList::east = new mapNode(MAPSIZE/2 + 1, MAPSIZE/2, MIDPOINT);
	mapList::west = new mapNode(MAPSIZE/2 - 1, MAPSIZE/2, MIDPOINT);

	heightmap.createHeightmap(mapList::current->mType, seeds[mapList::current->x][mapList::current->y]);
	mapList::current->terrainData->init(heightmap.getHeightData());

	/*heightmap.createHeightmap(3, seeds[mapList::current->x][mapList::current->y]);
	mapList::north->terrainData->init(heightmap.getHeightData());

	float** map1; 
	float** map2;
	map1 = new float*[32];
	map2 = new float*[32];
	for (int i=0; i < 32; i++)
	{
		map1[i] = new float[32];
		map2[i] = new float[32];
		for (int j=0; j<32; j++)
		{
			map1[i][j] = 0;
			map2[i][j] = 0;
		}
	}

	mapList::current->terrainData->createMap(map1);
	mapList::north->terrainData->createMap(map2);

	bool check = false;

	for (int i=0; i < 32; i++)
	{
		for (int j=0; j<32; j++)
		{
			if(map1[i][j] != map2[i][j])
				check = true;
		}
	}
	*/

	heightmap.createHeightmap(mapList::north->mType, seeds[mapList::north->x][mapList::north->y]);
	mapList::north->terrainData->init(heightmap.getHeightData());

	heightmap.createHeightmap(mapList::south->mType, seeds[mapList::south->x][mapList::south->y]);
	mapList::south->terrainData->init(heightmap.getHeightData());

	heightmap.createHeightmap(mapList::east->mType, seeds[mapList::east->x][mapList::east->y]);
	mapList::east->terrainData->init(heightmap.getHeightData());

	heightmap.createHeightmap(mapList::west->mType, seeds[mapList::west->x][mapList::west->y]);
	mapList::west->terrainData->init(heightmap.getHeightData());
}