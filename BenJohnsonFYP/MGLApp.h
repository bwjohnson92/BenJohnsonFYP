/*************************************************************************

*************************************************************************/

#ifndef MGLAPP_H_INCLUDED
#define MGLAPP_H_INCLUDED

#ifdef WIN32
  #define WIN32_LEAN_AND_MEAN
  #define WIN32_EXTRA_LEAN
#include <windows.h>
#endif

#pragma comment(lib, "glee")
#define DEBUG_FLAG true

#include "glee.h"
#include <gl/glu.h>
#include <gl/GL.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <omp.h>

#include "CGLFontClass.h"
#include "CHeightClass.h"
#include "CTerrainVBOCull.h"

#include "AStarPathFinding.h"
#include "MapInfo.h"

// values used to set up the window and rendering context
const char MGL_APP_NAME[]	= "Ben Johnson - FYP - J004511A - Pathfinding Across Randomly Generated Terrain";

const int MGL_WINDOW_WIDTH = 800;
const int MGL_WINDOW_HEIGHT = 600;

const int MGL_COLOR_BITS = 32;
const int MGL_DEPTH_BITS = 32;
const int MGL_STENCIL_BITS = 0;

const bool MGL_FORCE_FULLSCREEN = false;


class MGLApp
{
public:
	MGLApp();
	virtual ~MGLApp();

	bool Init(HDC g_hDC);
	bool Shutdown();
	void SetupProjection( int width, int height );
	void Update( double dt );
	void Render();
	void CalculatePath();
	void CalculateMapCollisions(int startx, int starty, int endx, int endy);
	void DebugString(std::string inStr);

private:

	HDC ghDC;

	// The applications font.
	CGLFontClass font;
	
	// Our heightmap.
	CHeightClass heightmap;

	// Our terrain mesh.
	CTerrainVBOCull terrain;


	//Pathfinding data
	float ** map;
	//Graphnode map
	GraphNode*** graphNodeMap;
	//Final path

	//Data pulled from the heightmap
	heightData *** heightmapData;
	
	// 2D array (2 x num) of maps that need to be generated
	int ** mapCrossovers;
	//Number of maps that need to be generated
	int numOfCells;
	//Entire world data generation
	mapHeightData** world;
	mapHeightData** worldCopy;

	float playerPosX, playerPosY;

	// Width and Height of the window
	int sWidth, sHeight;

	// Variable to toggle wireframe and bounding boxes.
	bool wireframe;
	bool boundingBox;
	bool drawSpheres;
	char* msgbuf;

	// Movement variables.
	float translateX, translateY, translateZ, rotateX, rotateY;

	// Variable to make sure input works smoothly.
	double timeSinceInput;

};

#endif // MGLAPP_H_INCLUDED