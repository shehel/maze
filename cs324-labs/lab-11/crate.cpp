#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glew.h> 
#include <GL/glut.h> 
#endif

#include <iostream>

#include <stdlib.h>
#include <stddef.h>
#include <unistd.h> // for usleep

#include "load_and_bind_texture.h"

bool g_spinning = false;
int g_spin = 0;

unsigned int g_crate = 0;

float g_cube_vertices[8][3] = 
	{
		{-0.5f, -0.5f, -0.5f}, // front
		{0.5f, -0.5f, -0.5f},
		{0.5f, 0.5f, -0.5f},
		{-0.5f, 0.5f, -0.5f},
		{-0.5f, -0.5f, 0.5f}, // back 
		{0.5f, -0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f},
		{-0.5f, 0.5f, 0.5f}
	};

// indices into verices
size_t g_cube_faces[6][4] =
	{
		{0, 1, 2, 3},  // front
		{5, 4, 7, 6},  // back
		{4, 0, 3, 7},  // left
		{1, 5, 6, 2},  // right
		{4, 5, 1, 0},  // bottom
		{3, 2, 6, 7}   // top
	};

void load_and_bind_textures()
{
	// load all textures here
	g_crate = load_and_bind_texture("../images/crate.png");


}

void idle()
{
    usleep(10000); // in microseconds
	g_spin += 1;
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

	// position and orient camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 1.5, 3.5, // eye position
			  0, 0, 0.0, // reference point
			  0, 1, 0  // up vector
		);

	glPushMatrix();
		glRotatef(g_spin, 0, 1, 0);

		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_crate);

		glBegin(GL_QUADS);
		for (size_t f=0;f<6;f++) // for each face
		{
			glTexCoord2f(0, 0);
			glMultiTexCoord2i(GL_TEXTURE0, 0, 0);
			glVertex3fv(g_cube_vertices[g_cube_faces[f][0]]);
			glMultiTexCoord2i(GL_TEXTURE0, 1, 0);
			glVertex3fv(g_cube_vertices[g_cube_faces[f][1]]);
			glMultiTexCoord2i(GL_TEXTURE0, 1, 1);
			glVertex3fv(g_cube_vertices[g_cube_faces[f][2]]);
			glMultiTexCoord2i(GL_TEXTURE0, 0, 1);
			glVertex3fv(g_cube_vertices[g_cube_faces[f][3]]);
		}
		glEnd();

		glActiveTexture(GL_TEXTURE1);
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glutSwapBuffers(); 
}

void keyboard(unsigned char key, int, int)
{
	switch (key)
	{
		case 'q': exit(1); break;

		case ' ': 
				g_spinning = !g_spinning;
				if (g_spinning)
					glutIdleFunc(idle);
				else
					glutIdleFunc(NULL);
				break;
	}
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluPerspective(40.0, 1.0f, 1.0, 5.0);

	glutPostRedisplay();
}

void init()
{

    int max_texture_units = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);
    fprintf(stderr, "Max texture units is %d\n", max_texture_units);

	load_and_bind_textures();

	GLenum error = glGetError();
	if (error!=GL_NO_ERROR)
		fprintf(stderr, "GL error %s\n", gluErrorString(error));
	

	glEnable(GL_DEPTH_TEST);

}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH); 
	glutInitWindowSize(640, 640); 
	glutInitWindowPosition(50, 50); 
	glutCreateWindow("Crate"); 
#ifndef __APPLE__ 
	GLenum err = glewInit();
	if (GLEW_OK!=err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(1);
	}
	fprintf(stderr, "Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

	// get texture ready before we need it
	init(); 

	glutKeyboardFunc(keyboard); 
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape); 

	glutMainLoop(); 

	return 0; 
}
