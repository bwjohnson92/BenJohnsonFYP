// height_class.h: interface for the height_class class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CHEIGHTCLASS_H_
#define _CHEIGHTCLASS_H_

#include <ctime>
#include "glee.h"
#include <gl/glu.h>
#include <iostream>
#include <fstream>
#include "ChunkHeader.h"

/*
struct chunkInfo
{
	float minY, maxY, avgY, varY;
	chunkInfo(){ minY = maxY = avgY = varY = 0; }
};*/

// Vector struct.
struct SVector
{
	float x, y, z;
};

// Color struct.
struct SColor
{
	GLubyte r, g, b, a;
};


// Height data struct.
struct heightData
{
	float y;				// Height at this location.
	SVector normal;			// Normal at this location.
	SColor color;			// The color at this location.
};


class CHeightClass  
{
	/////////////////////
	// Private Members //
	/////////////////////

	private:

		// Seed used to create this heightmap.
		int rand_seed;

		// Max and min radius for the hills.
		int radmin;
		int radmax;
		

		// The width and height of the heightmap.
		enum {Width = 1025, Height = 1025};
	
	
		// The heightmap data.
		heightData **heights;		

		// A texture used to display the heightmap to the screen.
		GLuint heightmapTexture;


		////////////////////////////////
		//  Private Member Functions  //
		////////////////////////////////
					  
		//////////////////////////////////////////////////////////
		//	Function: "addHill"
		//
		//	Input: None
		//
		//	Output: None
		//
		//	Returns: None
		//
		//	Purpose: Raises a hill on the heightmap.
		//////////////////////////////////////////////////////////
		void addHill(void);


		//////////////////////////////////////////////////////////
		//	Function: "normalize"
		//
		//	Input: None
		//
		//	Output: None
		//
		//	Returns: None
		//
		//	Purpose: Normalizes the heights in the heightmap.
		//////////////////////////////////////////////////////////
		void normalize(void);

		void flaten(void);


		//////////////////////////////////////////////////////////
		//	Function: "scale"
		//
		//	Input: int value		Scaling value.
		//
		//	Output: None
		//
		//	Returns: None
		//
		//	Purpose: Scales the terrains height values.
		//////////////////////////////////////////////////////////
		void scale(int value);

		void smooth(void);

		void createNormals(void);

		void diamond(int ulx, int ulz, int width, int randValue);


		//////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////
		void square(int ulx, int ulz, int width, int randValue);


		//////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////
		void createImage(void);


  
	////////////////////
	// Public Members //
	////////////////////

	public:

		// These enumerations are used to decide which type of heightmap to create.
		enum {Hill = 0, FaultLine = 1, Midpoint = 2};


		/////////////////
		// Constructor //
		/////////////////

		CHeightClass(void)
		{
			heights = NULL;
			heightmapTexture = 0;
		};



		////////////////
		// Destructor //
		////////////////

		virtual ~CHeightClass()
		{
			if (heights)
			{
				for (int i = 0; i < Width; i++)
					delete [] heights[i];

				delete heights;
			}
		};



		
		/////////////////////////////
		// Public Member Functions //
		/////////////////////////////

		void createHeightmapHill(int rmin = 10, int rmax = 100, int seed = time(0));

		void createHeightmapFaultLine(int seed = time(0));

		void createHeightmapMidpoint(int seed = time(0));
	//	void createHeightmapMidpoint2(int seed = time(0));

		void createHeightmap(int type);
		void createHeightmap(int type, int seed);
		void draw(int sWidth, int sHeight);


		heightData ***getHeightData(void)
		{
			return &heights;
		}

		int getHeight()
		{
			return Height;
		}

		int getWidth()
		{
			return Width;
		}

		void printHeights(void);

		void createChunkInfo(int ChunkHeight, int ChunkWidth, mapHeightData *map);

};

#endif // _HEIGHT_CLASS_H_
