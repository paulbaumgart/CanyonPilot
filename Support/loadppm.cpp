//! Load a ppm file from disk.
// @input filename The location of the PPM file.  If the file is not found, an error message
//		will be printed and this function will return 0
// @input width This will be modified to contain the width of the loaded image, or 0 if file not found
// @input height This will be modified to contain the height of the loaded image, or 0 if file not found
//
// @return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured

#include "loadppm.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

unsigned char* loadPPM(const char* filename, int& width, int& height)
{
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ( (fp=fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return 0;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);
	
    retval_fgets = fgets(buf[0], BUFSIZE, fp);
    retval_sscanf=sscanf(buf[0], "%d", &width);
    retval_fgets = fgets(buf[0], BUFSIZE, fp);
    retval_sscanf=sscanf(buf[0], "%d", &height);

	// Read maxval:
	do
	{
	retval_fgets=fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;

		return 0;
	}

	return rawData;
}
