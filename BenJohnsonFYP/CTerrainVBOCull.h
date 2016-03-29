#ifndef _CTERRAINVBOCULL_H_
#define _CTERRAINVBOCULL_H_

#include "GLee.h"
#include "CHeightClass.h"
#include "CFrustum.h"

class CTerrainVBOCull
{
	/////////////////////
	// Private Members //
	/////////////////////

	private:

		
		GLuint bufferVertList;	// Vertex list.

		// The height and width of the heightmap mesh.
		enum{Width = 1025, Height = 1025};
		
		// The height and width of a single chunk of the mesh.
		enum{ChunkWidth = 33, ChunkHeight = 33};
		

		// The chunk structure.
		struct chunk
		{
			GLuint bufferTriList;		// Triangle index list for a chunk.
			float maxX, maxY, maxZ;		// Maximum vertex values in this chunk.
			float minX, minY, minZ;		// Minimum vertex values in this chunk.
			float avgY, varY;           //Average height within chunk, variance of height.
		};

		// A multidimentional array of mesh chunks.
		chunk **chunkArray;


		// Heightmap data (will come from the heightmap class).
		heightData **data;

		// The view Frustum.
		CFrustum frustum;


		// Data offsets into the vert list.
		int vertOffset;
		int normalOffset;
		int colorOffset;

		// The number of total triangles in the mesh.
		int numElements;

		void buildBufferObjectVertexArrays(void);

		void createColors(void);



	////////////////////
	// Public Members //
	////////////////////

	public:


	/////////////////
	// Constructor //
	/////////////////
		CTerrainVBOCull(void)
		{

			// Initialize our variables.
			data = NULL;
			bufferVertList = 0;

			vertOffset   = 0;
			normalOffset = 0;
			colorOffset  = 0;

			numElements = 0;


			// Create the chunk array and initialize it's values.
			chunkArray = new chunk*[Width/(ChunkWidth-1)];
			for (int x = 0; x < Width/(ChunkWidth-1); x++)
			{
				chunkArray[x] = new chunk[Height/(ChunkHeight-1)];
			}

			for (int z = 0; z < Height/(ChunkHeight-1); z++)
			{
				for (int x = 0; x < Width/(ChunkWidth-1); x++)
				{
					chunkArray[x][z].bufferTriList = 0;
				}
			}
		}


	////////////////
	// Destructor //
	////////////////

		virtual ~CTerrainVBOCull(void)
		{
			if (chunkArray)
			{
				for (int x = 0; x < Width / (ChunkWidth-1); x++)
					delete [] chunkArray[x];

				delete chunkArray;
			}
		}


		void init(heightData ***heights)
		{
			// Keep a copy of the heightmap data for ourselves.
			data = *heights;

			// Check to make sure we can use vertex buffer objects.
			if (!GLEE_ARB_vertex_buffer_object)
				MessageBox(0, "Your version of OpenGL does not support buffer objects", "Error", MB_OK);


			// Asign the color values.
			createColors();

			// Build the buffer objects.
			buildBufferObjectVertexArrays();
		}

		void createMap(float ** map);

		void draw(bool boundingBox, bool drawCubes, int x, int y);
};

#endif