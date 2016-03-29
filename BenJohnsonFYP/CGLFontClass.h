////////////////////////////////////////////////////////////////////////////
//	File: "CGLFontClass.h"
//
//	Purpose: Definition of the CGLFontClass class.
////////////////////////////////////////////////////////////////////////////

#ifndef _CGLFONTCLASS_H_
#define _CGLFONTCLASS_H_


#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <gl/glu.h>

#include <iostream>
#include "MGLTargaImage.h"


class CGLFontClass
{
	private:

		
		// Texture object for the font texture.
		GLuint texture;

		// Width and height of the texture.
		int width, height;

		// Array to hold the width of each character.
		int charWidth[256];

		// The amount of space used for each letter on the texture (in pixels)
		int cWidth, cHeight;	

		// Array to hold the precalculated texture coordinates for each character.
		struct texCoord
		{
			float ulx, uly;		// Upper left hand corner.
			float lrx, lry;		// Lower right hand corner.
		} textureCoords[256];
				 

		// Width and height of the OpenGL window.
		int screenWidth, screenHeight;

		bool fontLoaded;

	public:


	/////////////////
	// Constructor //
	/////////////////

		//////////////////////////////////////////////////////////
		//	Function: CGLFontClass
		//
		//	Input:	None
		//
		//	Output:	None
		//
		//	Returns:None
		//
		//	Purpose: Constructor for the class
		//////////////////////////////////////////////////////////
		CGLFontClass(void)
		{
			texture = 0;
			width = 0;
			height =0;
			screenWidth = 0;
			screenHeight = 0;

			fontLoaded = false;
		};


	////////////////
	// Destructor //
	////////////////

		//////////////////////////////////////////////////////////
		//	Function: ~CGLFontClass
		//
		//	Input:	None
		//
		//	Output:	None
		//
		//	Returns:None
		//
		//	Purpose: Destructor for the class
		//////////////////////////////////////////////////////////
		virtual ~CGLFontClass(void){ };


	/////////////////////////////
	// Public Member Functions //
	/////////////////////////////

		//////////////////////////////////////////////////////////
		//	Function: "Load"
		//
		//	Input:	char* filename		The name of the texture file.
		//			char* datFileName	The file holding the individual character widths.
		//
		//	Output: None
		//
		//	Returns: True if the texture loaded correctly.
		//
		//	Purpose: Sets up a font for use.
		//////////////////////////////////////////////////////////
		bool load(char *filename, char* datFileName);



		//////////////////////////////////////////////////////////
		//	Function: "Display"
		//
		//	Input:	char* output		The string to display.
		//			int x				The x position to start displaying to.
		//			int y				The y position to start displaying to.
		//			float alpha			The alpha value.
		//
		//	Output: None
		//
		//	Returns: None
		//
		//	Purpose: Displays the string to the screen.
		//////////////////////////////////////////////////////////
		void display(char *output, int x, int y, float alpha = 1.0f);



		//////////////////////////////////////////////////////////
		//	Function: "setDimentions"
		//
		//	Input:	int sWidth			The current screen width.
		//			int sHeight			The current screen height.
		//
		//	Output: None
		//
		//	Returns: None
		//
		//	Purpose: Updates the internal screen width and heigth variables.
		//////////////////////////////////////////////////////////
		void setDimentions(int sWidth, int sHeight)
		{
			screenWidth = sWidth;
			screenHeight = sHeight;
		}
};

#endif