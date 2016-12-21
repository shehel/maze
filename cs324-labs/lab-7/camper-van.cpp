#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <iostream>

#include <stdlib.h>
#include <stddef.h>
#include <unistd.h> // for usleep

#include "load_and_bind_texture.h"

bool g_spinning = false;
int g_spin = 0;

enum van_sides_t {FRONT=0, LEFT_SIDE=1, RIGHT_SIDE=2, 
                  TOP=3, BACK=4, NUM_SIDES=5};

// van edges
const float g_l = -0.25f;
const float g_r = 0.25f;
const float g_t = 0.05f;
const float g_b = -0.45f;
const float g_f = 0.4f;
const float g_k = -0.4f;

// coordinates on the input images
const int g_van_source_coords[NUM_SIDES][4][2] = {
	{ {121, 407}, {422, 407}, {383, 64}, {151, 64} },
	{ {0, 411}, {547, 411}, {547, 0}, {0, 0} },
	{ {0, 411}, {547, 411}, {547, 0}, {0, 0} },
	{ {195, 211}, {230, 211}, {230, 170}, {195, 170} },
	{ {0, 411}, {547, 411}, {547, 0}, {0, 0} }
};

// target coordinates
const float g_van_vertices[NUM_SIDES][4][3] = {
	{ {g_l, g_b, g_f}, {g_r, g_b, g_f}, {g_r, g_t, g_f}, {g_l, g_t, g_f} }, // front
	{ {g_r, g_b, g_f}, {g_r, g_b, g_k}, {g_r, g_t, g_k}, {g_r, g_t, g_f} }, // left-side 
	{ {g_l, g_b, g_k}, {g_l, g_b, g_f}, {g_l, g_t, g_f}, {g_l, g_t, g_k} }, // right-side
	{ {g_r, g_t, g_f}, {g_r, g_t, g_k}, {g_l, g_t, g_k}, {g_l, g_t, g_f} }, // top
	{ {g_r, g_b, g_k}, {g_l, g_b, g_k}, {g_l, g_t, g_k}, {g_r, g_t, g_k} }  // back
};

// texture handles
unsigned int g_road = 0;
unsigned int g_camper_tex[NUM_SIDES];

void load_and_bind_textures()
{
	// load all textures here
	g_road = load_and_bind_texture("../images/road.png");

	// load the camper images here
	g_camper_tex[FRONT] = load_and_bind_texture("../images/camper-front.png");
	g_camper_tex[LEFT_SIDE] = load_and_bind_texture("../images/camper-left.png");
	g_camper_tex[RIGHT_SIDE] = load_and_bind_texture("../images/camper-right.png");
	g_camper_tex[TOP] = load_and_bind_texture("../images/camper-front.png");
	g_camper_tex[BACK] = load_and_bind_texture("../images/camper-back.png");

}

void idle()
{
    usleep(50000); // in microseconds
	g_spin += 1;
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

	// position and orient camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0.25, 2.5, // eye position
			  0, 0, 0.0, // reference point
			  0, 1, 0  // up vector
		);

	glPushMatrix();
		glRotatef(g_spin, 0, 1, 0);

		glEnable(GL_TEXTURE_2D);

		// map the road surface
		glBindTexture(GL_TEXTURE_2D, g_road);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-0.5f, -0.5f, 0.5f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(0.5f, -0.5f, 0.5f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(0.5f, -0.5f, -0.5f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-0.5f, -0.5f, -0.5f);
		glEnd();

		// do other textures here
		// do each side
		for (size_t s=0;s<NUM_SIDES;s++)
		{
			glBindTexture(GL_TEXTURE_2D, g_camper_tex[s]);
			glBegin(GL_QUADS);
				for (size_t i=0;i<4;i++)
				{
					glTexCoord2f(
						g_van_source_coords[s][i][0]/547.0f,
						(411-g_van_source_coords[s][i][1])/411.0f
					);
					glVertex3fv(g_van_vertices[s][i]);
				}
			glEnd();
		}

		glDisable(GL_TEXTURE_2D);

		// draw the edges in wire frame
		glPushAttrib(GL_CURRENT_BIT);
			glColor3f(1.0f, 1.0f, 0.0f);
			for (size_t s=0;s<NUM_SIDES;s++)
			{
				glBegin(GL_LINE_LOOP);
				for (size_t v=0;v<4;v++)
					glVertex3fv(g_van_vertices[s][v]);
				glEnd();
			}
		glPopAttrib();


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
	glutCreateWindow("VW Camper Van"); 

	// get texture ready before we need it
	init(); 

	glutKeyboardFunc(keyboard); 
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape); 

	glutMainLoop(); 

	return 0; 
}
