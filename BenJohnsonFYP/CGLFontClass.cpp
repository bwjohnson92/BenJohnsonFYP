////////////////////////////////////////////////////////////////////////////
//	File: "CGLFontClass.cpp"
//
//	Purpose: Implimentation of the CGLFontClass class.
////////////////////////////////////////////////////////////////////////////


#include "CGLFontClass.h"



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
bool CGLFontClass::load(char *filename, char* datFileName)
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Load the texture.
	MGL::TargaImage image;
	if (!image.Load(filename))
		return false;


	// Check to make sure the image has an alpha channel.
	if (image.GetImageFormat() != 1)
		return false;

	image.FlipVertical();

	// Get and save the width and height.
	width = image.GetWidth();
	height = image.GetHeight();

	// Create the texture.
	if (texture > 0)
		glDeleteTextures(1, &texture);
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, 
								4, 
								width, 
								height, 
								0, 
								GL_RGBA, 
								GL_UNSIGNED_BYTE, 
								image.GetImage());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set up the wrap.
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);



	// Release the image memory.
	image.Release();
	

	// Open the data file.
	FILE* pFile = fopen(datFileName, "rb");

	if (!pFile)
		return false;

	// Read the data.
	for (int i = 0; i < 256; i++)
	{
		char data;
		fread(&data, 1, sizeof(char), pFile);
		charWidth[i] = data + 2;
	}

	// Close the file.
	fclose(pFile);

	// The texture should be 16 characters by 16 characters.  When we divide
	// the width and the height by 16, we get the width and height of a single
	// character.
	cWidth = width/16.0f;
	cHeight = height/16.0f;

	// When converting from pixel space on the texture to texture space, we
	// multiply the number in pixels by one divided by the image width.
	float p = 1.0f/width;

	// Calculate the texture coordinates for each character.
	for (int i = 0; i < 256; i++)
	{
		// The starting x (in pixels) for this character.  i%16 * cWidth finds
		// the general location, and the rest calculates the variable width.
		int startx = ((i % 16) * cWidth) + ((cWidth-charWidth[i])/2);
	
		textureCoords[i].ulx = startx * p;
		textureCoords[i].uly = i / 16 * cHeight * p;
		textureCoords[i].lrx = (startx + charWidth[i])*p;
		textureCoords[i].lry = (i / 16 + 1) * cHeight * p;
	}


	fontLoaded = true;
	return true;  
}




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
void CGLFontClass::display(char *output, int x, int y, float alpha)
{
	int stringlength = strlen(output);
	
	int xoffset = x;

	// Set up ortho mode.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, screenWidth, screenHeight, 0.0f, 1.0f, -1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture);
	glColor4f(1.0f, 1.0f, 1.0f, alpha);

	// Go through the string a character at a time and display the output.
	for (int i = 0; i < stringlength; i++)
	{
		int index = output[i];

		// Check to see if this character is on the screen.
		if (xoffset > 0 && 
			xoffset + charWidth[index] < screenWidth &&
			y > 0 &&
			y + cHeight < screenHeight)
		{
			// Display a letter.
			glBegin(GL_TRIANGLE_STRIP);
				glTexCoord2f(textureCoords[index].ulx, textureCoords[index].uly);
				glVertex2f(xoffset, y);

				glTexCoord2f(textureCoords[index].ulx, textureCoords[index].lry);			
				glVertex2f(xoffset, y+cHeight);

				glTexCoord2f(textureCoords[index].lrx, textureCoords[index].uly);
				glVertex2f(xoffset+charWidth[index], y);

				glTexCoord2f(textureCoords[index].lrx, textureCoords[index].lry);
				glVertex2f(xoffset+charWidth[index], y+cHeight);	
			glEnd();
		}

		xoffset += charWidth[index];
	}
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, screenWidth/screenHeight, 1.0f, 1000.0f);
}