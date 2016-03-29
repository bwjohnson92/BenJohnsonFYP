// height_class.cpp: implementation of the height_class class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <math.h>
#include "CHeightClass.h"
#include <stdio.h>
#include <assert.h>
#include "MapInfo.h"


//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
void CHeightClass::addHill(void)
{

	// Get a random radius between radmin and radmax.
	int radius = rand()%(radmax - radmin + 1) + radmin;


	// Get a random x and y for the center point.
	int randX = (rand()%(Width +(radius*2))) -radius;
	int randY = (rand()%(Height+(radius*2))) -radius;


	// radius squared.
	int rad_sqr = radius * radius;


	// We can speed up building the hill by creating a bounding box around the
	// hill and just looping through those variables.  Also, our center point
	// can be outside of the heightmap itself, so this will keep us from going
	// outside of the bounds of the heightmap.
	int startX = randX - radius;
	if( startX < 0 )
		startX = 0;

	int endX = randX + radius;
	if( endX > Width)
		endX = Width;


	int startY = randY - radius;
	if( startY < 0 )
		startY = 0;

	int endY = randY + radius;
	if( endY > Height)
		endY = Height;


	// Loop through the values inside the bounding box and raise our hill.
	int xside, yside;
	for (int y = startY; y < endY; y++)
	{
		for ( int x = startX; x < endX; x++)
		{
			xside = randX - x;
			xside = xside * xside;
			yside = randY - y;
			yside = yside * yside;

			if (heights[x][y].y < ((rad_sqr) - (xside + yside)))
				heights[x][y].y = heights[x][y].y + float(sqrt((float)rad_sqr - (xside + yside)));
		}
	}
}





//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
void CHeightClass::normalize(void)
{
	// Holds the max and min height in the heightmap.
	float min = heights[0][0].y;
	float max = heights[0][0].y;

	// Find the min and max heights in the heightmap.
	for ( int y = 0; y < Height; y++)
	{
		for ( int x = 0; x < Width; x++)
		{
			if(heights[x][y].y < min)
				min = heights[x][y].y;

			if(heights[x][y].y > max)
				max = heights[x][y].y;
		}
	}


	// Normalize the heightmap.
	for(int y = 0; y < Height; y++)
	{
		for( int x = 0; x < Width; x++)
		{
			heights[x][y].y = (heights[x][y].y - min) / (max - min);
		}
	}
}




//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
void CHeightClass::flaten(void)
{
	// Flaten the heightmap.
	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			// Square the normalized height value.
			heights[x][y].y = heights[x][y].y * heights[x][y].y;
		}
	}
}




//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
void CHeightClass::scale(int value)
{
	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			heights[x][y].y = heights[x][y].y * value;
		}
	}
}




//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
void CHeightClass::createNormals(void)
{
	SVector v1, v2, n, ntmp;
	float length;


	// Loop through all points and create a normal.
	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			// Initialize the vectors.
			n.x    = n.y    = n.z    = 0.0f;
			v1.x   = v1.y   = v1.z   = 0.0f;		
			v2.x   = v2.y   = v2.z   = 0.0f;
			ntmp.x = ntmp.y = ntmp.z = 0.0f;
			length = 0.0f;

			// Add the normals from the faces around our point.
			if (x > 0)
			{
				if (y > 0)
				{
					v1.x =  0; v1.z = -1; v1.y = heights[x][y-1].y - heights[x][y].y;
					v2.x = -1; v2.z =  0; v2.y = heights[x-1][y].y   - heights[x][y].y;
					ntmp.x = v1.y * v2.z - v1.z * v2.y;
					ntmp.y = v1.z * v2.x - v1.x * v2.z;
					ntmp.z = v1.x * v2.y - v1.y * v2.x;

					n.x += ntmp.x; n.y += ntmp.y; n.z += ntmp.z;
				}

				if (y < Height - 1)
				{
					v1.x = -1; v1.z =  0; v1.y = heights[x-1][y].y   - heights[x][y].y;
					v2.x = -1; v2.z =  1; v2.y = heights[x-1][y+1].y - heights[x][y].y;
					ntmp.x = v1.y * v2.z - v1.z * v2.y;
					ntmp.y = v1.z * v2.x - v1.x * v2.z;
					ntmp.z = v1.x * v2.y - v1.y * v2.x;

					n.x += ntmp.x; n.y += ntmp.y; n.z += ntmp.z;

					v1.x = -1; v1.z =  1; v1.y = heights[x-1][y+1].y - heights[x][y].y;
					v2.x =  0; v2.z =  1; v2.y = heights[x][y+1].y - heights[x][y].y;
					ntmp.x = v1.y * v2.z - v1.z * v2.y;
					ntmp.y = v1.z * v2.x - v1.x * v2.z;
					ntmp.z = v1.x * v2.y - v1.y * v2.x;

					n.x += ntmp.x; n.y += ntmp.y; n.z += ntmp.z;
				}
			}

			if (x < Width - 1)
			{
				if (y < Height - 1)
				{
					v1.x =  0; v1.z =  1; v1.y = heights[x][y+1].y - heights[x][y].y;
					v2.x =  1; v2.z =  0; v2.y = heights[x+1][y].y   - heights[x][y].y;
					ntmp.x = v1.y * v2.z - v1.z * v2.y;
					ntmp.y = v1.z * v2.x - v1.x * v2.z;
					ntmp.z = v1.x * v2.y - v1.y * v2.x;

					n.x += ntmp.x; n.y += ntmp.y; n.z += ntmp.z;
				}

				if (y > 0)
				{
					v1.x =  1; v1.z =  0; v1.y = heights[x+1][y].y   - heights[x][y].y;
					v2.x =  1; v2.z = -1; v2.y = heights[x+1][y-1].y - heights[x][y].y;
					ntmp.x = v1.y * v2.z - v1.z * v2.y;
					ntmp.y = v1.z * v2.x - v1.x * v2.z;
					ntmp.z = v1.x * v2.y - v1.y * v2.x;

					n.x += ntmp.x; n.y += ntmp.y; n.z += ntmp.z;

					v1.x =  1; v1.z = -1; v1.y = heights[x+1][y-1].y - heights[x][y].y;
					v2.x =  0; v2.z = -1; v2.y = heights[x][y-1].y - heights[x][y].y;
					ntmp.x = v1.y * v2.z - v1.z * v2.y;
					ntmp.y = v1.z * v2.x - v1.x * v2.z;
					ntmp.z = v1.x * v2.y - v1.y * v2.x;

					n.x += ntmp.x; n.y += ntmp.y; n.z += ntmp.z;
				}
			}
			
			// Find the length of this vector.
			length = (float)sqrt((n.x * n.x) + (n.y * n.y) + (n.z * n.z));

			// Normalize this vector.
			n.x = n.x / length;
			n.y = n.y / length;
			n.z = n.z / length;

			// Set our normalized vector as this points normal.
			heights[x][y].normal.x = n.x;
			heights[x][y].normal.y = n.y;
			heights[x][y].normal.z = n.z;
		}
	}
}



	/////////////////////////////
	// Public Member Functions //
	/////////////////////////////


//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
void CHeightClass::createHeightmapHill(int rmin, int rmax, int seed)
{
	// Set the values.
	radmin = rmin;
	radmax = rmax;
	rand_seed = seed;

	// Allocate array for heightmap.
	if (heights == NULL)
	{
		heights = new heightData*[Width];
		for (int i = 0; i < Width; i++)
		{
			heights[i] = new heightData[Height];
		}
	}

	// Init the heightmap.
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			heights[j][i].y = 0;
		}
	}


	// Seed the random number generator.
	srand(rand_seed);


	// Add the hills.
	for (int i = 0; i < 3000; i++)
	{
		addHill();
	}

	// Smooth the terrain.
	for (int i = 0; i < 40; i++)
	{
		smooth();
	}


	// Normalize and flaten the heightmap.
	normalize();
	flaten();
	scale(128);

	// Create normals.
	createNormals();
}


//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
void CHeightClass::createHeightmapFaultLine(int seed)
{
	// Allocate array for heightmap.
	if (heights == NULL)
	{
		heights = new heightData*[Width];
		for (int i = 0; i < Width; i++)
		{
			heights[i] = new heightData[Height];
		}
	}

	// Init the heightmap.
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			heights[j][i].y = 0;
		}
	}
	//OutputDebugString("Testing");
	// Seed the random number generator.
	srand(seed);

	// Create a random line and raise or lower points on either side of it.
	int x1, x2, z1, z2;
	for (int i = 0; i < 200; i++)
	{
		// Create a random line.
		x1 = rand() % Width;
		x2 = rand() % Width;
		z1 = rand() % Height;
		z2 = rand() % Height;

		// Loop through the points in the heightmap.
		for (int z = 0; z < Height; z++)
		{
			for (int x = 0; x < Width; x++)
			{
				// Raise the height if the current point is on one side of the
				// line, otherwise lower the height.
				if (((x2-x1) * (z-z1)) - ((z2-z1) * (x-x1)) > 0)
					heights[x][z].y += 1;
				else
					heights[x][z].y -= 1;
			}
		}
	}

	// Smooth the result.
	for (int i = 0; i < 80; i++)
	{
		smooth();
	}


	// Normalize and flaten the heightmap.
	normalize();
	flaten();
	scale(128);

	// Create normals.
	createNormals();
}



//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
void CHeightClass::diamond(int ulx, int ulz, int width, int randValue)
{
	//////////////////
	// Top of diamond
	//////////////////
	
	// Check to make sure a height hasn't already been determined for this point.
	if (heights[ulx + (width/2)][ulz].y <= 0.0001)
	{
		// Check to decide if all points around or diamond are within the heightmap.
		if (ulz - (width/2) >= 0)
		{
			// Average the four points width/2 distance from our top point.
			heights[ulx + (width/2)][ulz].y = (heights[ulx][ulz].y + 
											   heights[ulx+width][ulz].y + 
											   heights[ulx+(width/2)][ulz-(width/2)].y + 
											   heights[ulx+(width/2)][ulz+(width/2)].y) / 
											   4.0 + ((rand()%(randValue*2))-randValue);
		}
		else
		{
			// Average the three points width/2 distance from our top point.
			heights[ulx + (width/2)][ulz].y = (heights[ulx][ulz].y + 
											   heights[ulx+width][ulz].y +
											   heights[ulx+(width/2)][ulz+(width/2)].y) /
											   3.0 + ((rand()%(randValue*2))-randValue);
		}
	}

	//////////////////////
	// Bottom of diamond
	//////////////////////

	// Check to make sure a height hasn't already been determined for this point.
	if (heights[ulx + (width/2)][ulz+width].y <= 0.0001)
	{
		// Check to decide if all points around or diamond are within the heightmap.
		if (ulz+width+(width/2) <= Width-1)
		{
			// Average the four points width/2 distance from our bottom point.
			heights[ulx + (width/2)][ulz+width].y = (heights[ulx][ulz+width].y + 
													 heights[ulx+width][ulz+width].y +
													 heights[ulx+(width/2)][ulz+(width/2)].y +
													 heights[ulx+(width/2)][ulz+width+(width/2)].y) /
													 4.0 + ((rand()%(randValue*2))-randValue);
		}
		else
		{
			// Average the three points width/2 distance from our bottom point.
			heights[ulx + (width/2)][ulz+width].y = (heights[ulx][ulz+width].y + 
													 heights[ulx+width][ulz+width].y +
													 heights[ulx+(width/2)][ulz+(width/2)].y) /
													 3.0 + ((rand()%(randValue*2))-randValue);
		}
	}

	/////////////////////
	// Left of diamond
	/////////////////////

	// Check to make sure a height hasn't already been determined for this point.
	if (heights[ulx][ulz + (width/2)].y <= 0.0001)
	{
		// Check to decide if all points around or diamond are within the heightmap.
		if (ulx - (width/2) >= 0)
		{
			// Average the four points width/2 distance from our left point.
			heights[ulx][ulz + (width/2)].y = (heights[ulx][ulz].y + 
											   heights[ulx][ulz+width].y +
											   heights[ulx+(width/2)][ulz+(width/2)].y +
											   heights[ulx-(width/2)][ulz+(width/2)].y) /
											   4.0 + ((rand()%(randValue*2))-randValue);
		}
		else
		{
			// Average the three points width/2 distance from our left point.
			heights[ulx][ulz + (width/2)].y = (heights[ulx][ulz].y + 
											   heights[ulx][ulz+width].y +
											   heights[ulx+(width/2)][ulz+(width/2)].y) /
											   3.0 + ((rand()%(randValue*2))-randValue);
		}
	}

	/////////////////////
	// Right of diamond
	/////////////////////

	// Check to make sure a height hasn't already been determined for this point.
	if (heights[ulx+width][ulz + (width/2)].y <= 0.0001)
	{
		// Check to decide if all points around or diamond are within the heightmap.
		if (ulx+width + (width/2) <= Width-1)
		{
			// Average the four points width/2 distance from our right point.
			heights[ulx+width][ulz + (width/2)].y = (heights[ulx+width][ulz].y + 
													 heights[ulx+width][ulz+width].y +
													 heights[ulx+width+(width/2)][ulz+(width/2)].y +
													 heights[ulx+width-(width/2)][ulz+(width/2)].y) /
													 4.0 + ((rand()%(randValue*2))-randValue);
		}
		else
		{
			// Average the three points width/2 distance from our right point.
			heights[ulx+width][ulz + (width/2)].y = (heights[ulx+width][ulz].y + 
													 heights[ulx+width][ulz+width].y +
													 heights[ulx+width-(width/2)][ulz+(width/2)].y) /
													 3.0 + ((rand()%(randValue*2))-randValue);
		}
	}
}



//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
void CHeightClass::square(int ulx, int ulz, int width, int randValue)
{
	// Calculate the average of the four points width distance around or center point.
	heights[ulx+(width/2)][ulz+(width/2)].y = (heights[ulx][ulz].y +
											   heights[ulx][ulz+width].y +
											   heights[ulx+width][ulz].y +
											   heights[ulx+width][ulz+width].y) /
											   4.0+ ((rand()%(randValue*2))-randValue);
}





//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
void CHeightClass::createHeightmapMidpoint(int seed)
{
	// Allocate array for heightmap.
	if (heights == NULL)
	{
		heights = new heightData*[Width];
		for (int i = 0; i < Width; i++)
		{
			heights[i] = new heightData[Height];
		}
	}

	// Init the heightmap.
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			heights[j][i].y = 0;
		}
	}

	// Seed the random number generator.
	srand(seed);

	// Set the corners to random heights.
	heights[0][0].y = rand() % Width;
	heights[Width-1][0].y = rand() % Width;
	heights[0][Height-1].y = rand() % Width;
	heights[Width-1][Height-1].y = rand() % Width;

	// Calculate the center point.
	heights[(Width-1)/2][(Width-1)/2].y = ((heights[0][0].y +
											   heights[0][Height-1].y +
											   heights[Width-1][0].y +
											   heights[Width-1][Height-1].y) /
											   4.0 + (rand()%Width) / 2);

	

	// We want to loop through our diamond and square functions untill all
	// heights have a value.  This will be acomplished when the width of the
	// diamond/square step is less than 2, leaving no space for new points.
	for (int width = (Width-1)/2; width >= 2; width /= 2)
	{
	
		// For each iteration of the above for loop, we will need to do an
		// iteration of the square and diamond steps for each width across
		// and down the heightmap.
		for (int x = 0; x < Width-1; x+= width)
		{
			for (int y = 0; y < Width-1; y+= width)
			{
				square(x, y, width, width);
			}
		}

		for (int x = 0; x < Width-1; x+= width)
		{
			for (int y = 0; y < Width-1; y+= width)
			{
				diamond(x, y, width, width);
			}
		}
	}

	// Smooth the result.
	for (int i = 0; i < 5; i++)
	{
		smooth();
	}


	// Normalize and flaten the heightmap.
	normalize();
	flaten();
	scale(128);

	// Create normals.
	createNormals();
}

void CHeightClass::printHeights(void)
{
	std::ofstream testFile;
	testFile.open("testfile.txt");

	for(int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Height; j++)
		{
			testFile << heights[i][j].y;
			testFile << " ";
			testFile << j;
			testFile << " ";
		}
		testFile << "End of line\n";
	}
	testFile.close();
	testFile.open("testfinished.txt");
	testFile.close();
}

void CHeightClass::createChunkInfo(int ChunkHeight, int ChunkWidth, mapHeightData *map)
{
	for (int chunkZ = 0; chunkZ < Height/(ChunkHeight-1); chunkZ++)
	{
		for (int chunkX = 0; chunkX < Width/(ChunkWidth-1); chunkX++)
		{
			int startx = chunkX * (ChunkWidth-1);
			int startz = chunkZ * (ChunkHeight-1);
			int endx = startx + ChunkWidth;
			int endz = startz + ChunkHeight;
		
			int maxY = heights[startx][startz].y;
			int minY = heights[startx][startz].y;
			int avgCount = 0;
			int numElements = 0;

			for (int z = startz; z < endz-1; z++)
			{
				for (int x = startx; x < endx; x++)
				{
					maxY = maxY > heights[x][z].y ? maxY : heights[x][z].y;
					minY = minY < heights[x][z].y ? minY : heights[x][z].y;
					avgCount += heights[x][z].y;
					numElements++;
				}
			}
			map->mapChunk[chunkX][chunkZ].maxY = maxY;
			map->mapChunk[chunkX][chunkZ].minY = minY;
			map->mapChunk[chunkX][chunkZ].varY = (maxY - minY);
			map->mapChunk[chunkX][chunkZ].avgY = avgCount / numElements;
			map->mapChunk[chunkX][chunkZ].heightGenerated = true;
		}
	}
}



//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
void CHeightClass::smooth(void)
{
	// The median of a point and it's surounding points.
	float median;

	// Smooth a point by averaging the points around it.
	for (int y = 0; y < Width; y++){
		for (int x = 0; x < Height; x++){
			int num_div = 1;
			median = heights[x][y].y;
			if (x > 0)
			{
				median += heights[x-1][y].y;
				num_div++;
			}
			else
			{
				median += heights[Width-1][y].y;
				num_div++;
			}
			if (x < Width - 1)
			{
				median += heights[x+1][y].y;
				num_div++;
			}
			else
			{
				median += heights[0][y].y;
				num_div++;
			}
			if (y > 0)
			{
				median += heights[x][y-1].y;
				num_div++;
			}
			else
			{
				median += heights[x][Height-1].y;
				num_div++;
			}
			if (y < Height - 1)
			{
				median += heights[x][y+1].y;
				num_div++;
			}
			else
			{
				median += heights[x][0].y;
				num_div++;
			}

			median = median / num_div;
			heights[x][y].y = median;
		}
	}
}




//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
void CHeightClass::createHeightmap(int type)
{
	// Create the heightmap using the given method.
	if (type == CHeightClass::FaultLine)
		createHeightmapFaultLine();
	else if (type == CHeightClass::Midpoint)
		createHeightmapMidpoint();
	else
		createHeightmapHill();
}

void CHeightClass::createHeightmap(int type, int seed)
{
	// Create the heightmap using the given method.
	if (type == 1)
		createHeightmapFaultLine(seed);
	else if (type == 2)
		createHeightmapMidpoint(seed);
	//else if (type == 3)
		//createHeightmapMidpoint2(seed);
	else
		createHeightmapHill(seed);
}



//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
void CHeightClass::createImage(void)
{
	// Create an array to hold the texture data.
	GLubyte *heightmap;
	heightmap = new GLubyte[(Width-1)*(Height-1)];

	// Normalize the heightmap so we can keep the values between 0 and 255.
	normalize();

	// Fill in our GLubyte array.
	for (int y = 0; y < Height-1; y++)
	{		
		for (int x = 0; x < Width-1; x++)
		{
			heightmap[(y*(Width-1))+x] = heights[x][y].y * 255;								 												
		}
	}


	// Create our texture object.
	if (heightmapTexture > 0)
		glDeleteTextures(1, &heightmapTexture);
	glGenTextures(1, &heightmapTexture);

	glBindTexture(GL_TEXTURE_2D, heightmapTexture);

	// Fill in out texture with our GLubyte array.
	glTexImage2D(GL_TEXTURE_2D, 0,  1, 
									Width-1, 
									Height-1,
									0,
									GL_LUMINANCE,
									GL_UNSIGNED_BYTE,
									heightmap);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set up the wrap.
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);

	// Free our GLubyte array.
	delete [] heightmap;
}





//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
void CHeightClass::draw(int sWidth, int sHeight)
{
	// Set up ortho mode.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, sWidth, sHeight, 0.0f, 1.0f, -1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	

	// Bind our heightmap image.
	glBindTexture(GL_TEXTURE_2D, heightmapTexture);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);


	// Display the texture.
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0, 0);
		glVertex2f(0, 0);

		glTexCoord2f(0, 1);			
		glVertex2f(0, sHeight);

		glTexCoord2f(1, 0);
		glVertex2f(sWidth, 0);

		glTexCoord2f(1, 1);
		glVertex2f(sWidth, sHeight);	
	glEnd();


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, sWidth/sHeight, 1.0f, 1000.0f);
}

