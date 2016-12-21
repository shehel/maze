#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <stddef.h> 
#include "png_load.h"

// initial window dimensions
size_t g_window_width = 512;
size_t g_window_height = 512;

// single texture
unsigned int g_the_tex = 0; // texture handle

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

	// enable texturing
  	glEnable(GL_TEXTURE_2D); 

	// select which texure to render
  	glBindTexture(GL_TEXTURE_2D, g_the_tex); 

	// specify texture coordinates
  	glBegin (GL_QUADS);
  		glTexCoord2f (0.0f,0.0f); // lower left corner
  		glVertex2i(0, 0);
  		glTexCoord2f (1.0f, 0.0f); // lower right corner
  		glVertex2i(g_window_width, 0);
  		glTexCoord2f (1.0f, 1.0f); // upper right corner
  		glVertex2i(g_window_width, g_window_height);
  		glTexCoord2f (0.0f, 1.0f); // upper left corner
  		glVertex2i(0, g_window_height);
  	glEnd ();

  	glDisable(GL_TEXTURE_2D); 
	glutSwapBuffers();
}

unsigned int load_and_bind_texture(const char* filename)
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

	unsigned int tex_handle = 0;

	// request one texture handle
	glGenTextures(1, &tex_handle); 

	// create a new texture object and bind it to tex_handle
  	glBindTexture(GL_TEXTURE_2D, tex_handle);

  	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  	glTexImage2D(GL_TEXTURE_2D, 0, 
				GL_RGB, width, height, 0,
   		 		GL_RGB, GL_UNSIGNED_BYTE, image_buffer);

	
	free(image_buffer); // free the image buffer memory
	
	return tex_handle;
}

void reshape(int w, int h)
{
	// save the new window dimensions when window is resized
	g_window_width = w;
	g_window_height = h;

  	glViewport(0, 0, w, h);

  	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
  	gluOrtho2D(0, w, 0, h);
  	glutPostRedisplay();
}

void keyboard(unsigned char key, int, int)
{
	switch(key)
	{
		case 'q': exit(0); break;
	}
	glutPostRedisplay();
}

void init()
{
	g_the_tex = load_and_bind_texture("../images/wood-grain.png");
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);

    glutInitWindowSize(g_window_width, g_window_height);
    glutInitWindowPosition(50, 50);

    glutCreateWindow("Texture");

    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    init();

    glutMainLoop();

    return 0;
}
