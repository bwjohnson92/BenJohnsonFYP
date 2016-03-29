#ifndef _CHUNKHEADER_H
#define _CHUNKHEADER_H

#pragma 0

struct chunkInfo
{
	float minY, maxY, avgY, varY;
	bool heightGenerated, haveVisited;
	chunkInfo(){ minY = maxY = avgY = varY = 0; heightGenerated = haveVisited = false; }
};

struct mapHeightData
	{
		chunkInfo**  mapChunk;
		mapHeightData(){ mapChunk = NULL; }
		~mapHeightData(){ delete mapChunk; } 
	};

#endif