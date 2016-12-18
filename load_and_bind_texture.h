#ifndef LOAD_AND_BIND_TEXTURE_H
#define LOAD_AND_BIND_TEXTURE_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stddef.h>
#include "png_load.h"

char* add_alpha_channel(char* image_buffer, const int width, const int height)
{
	char* image_buffer2 = new char[4 * width * height];
	int p = 0;
	int p2 = 0;
	for (int i=0;i<height;i++)
	for (int j=0;j<width;j++,p+=3,p2+=4)
	{
		image_buffer2[p2+0] = image_buffer[p+0];
		image_buffer2[p2+1] = image_buffer[p+1];
		image_buffer2[p2+2] = image_buffer[p+2];
		image_buffer2[p2+3] = 255;
	}
	delete[] image_buffer;
	return image_buffer2;
}

unsigned int load_and_bind_texture(const char* filename, const bool add_alpha=false)
{
    char* image_buffer = NULL; // the image data
	int width = 0;
	int height = 0;

	// read in the PNG image data into image_buffer
	if (png_load(filename, &width, &height, &image_buffer)==0)
    {
        fprintf(stderr, "Failed to read image texture from %s\n", filename);
        exit(1);
    }

	// request one texture handle
	unsigned int tex_handle = 0;
	glGenTextures(1, &tex_handle); 

	// create a new texture object and bind it to tex_handle
  	glBindTexture(GL_TEXTURE_2D, tex_handle);

	// so we can blend it 
  	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (add_alpha)
	{
		image_buffer = add_alpha_channel(image_buffer, width, height);
  		glTexImage2D(GL_TEXTURE_2D, 0, 
				GL_RGBA, width, height, 0,
   		 		GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)image_buffer);
	}
	else
	{
  		glTexImage2D(GL_TEXTURE_2D, 0, 
				GL_RGB, width, height, 0,
   		 		GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)image_buffer);
	}

    glBindTexture(GL_TEXTURE_2D,0);

	delete[] image_buffer; // free the image buffer memory
	
	return tex_handle;
}

#endif
