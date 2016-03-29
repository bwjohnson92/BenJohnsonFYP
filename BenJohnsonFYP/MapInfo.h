#ifndef _MAPINFO_H
#define _MAPINFO_H
#pragma 0

#include "CTerrainVBOCull.h"
#include "CHeightClass.h"
#include "ChunkHeader.h"

#define MAPSIZE 500

	extern int seeds[MAPSIZE][MAPSIZE]; 
	//static mapType mapTypeArray[MAPSIZE][MAPSIZE];


	enum mapType
	{
		HILL = 0,
		FAULTLINE = 1,
		MIDPOINT = 2,
	};

	extern mapType mapTypeArray[MAPSIZE][MAPSIZE];

	struct mapNode
	{
		mapType mType;
		CTerrainVBOCull* terrainData;
		int x, y;
		mapNode(int pX, int pY, mapType mT){x = pX; y = pY; mType = mT; terrainData = new CTerrainVBOCull();}
		~mapNode(){ delete terrainData; }
	};

	struct mapList
	{
		static mapNode* current;
		static mapNode* west;
		static mapNode* east;
		static mapNode* north;
		static mapNode* south;
		mapList(){ current = east = west = north = south = NULL; }
		~mapList(){ delete current; delete west; delete east; delete north; delete south; }
	};

	/*struct mapHeightData
	{
		chunkInfo**  mapChunk;
		mapHeightData(){ mapChunk = NULL; }
		~mapHeightData(){ delete mapChunk; } 
	};*/

	class MapInfo{
	public:
	static void moveNorth();
	static void moveSouth();
	static void moveEast();
	static void moveWest();
	static void initMapInfo();
	static void initMapArrays();
	};
#endif