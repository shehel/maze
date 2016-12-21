#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <iostream>

#include <stdlib.h>
#include <stddef.h>
#include <unistd.h> // for usleep

#include "draw_text.h"
#include "load_and_bind_texture.h"

bool g_spinning = false;
bool g_texture = false;
int g_spin = 0;

int g_curr = 0;
unsigned int g_tex[3];

unsigned int g_gen_mode = GL_OBJECT_LINEAR;

// properties of some material
float mat_ambient[] = {0.05, 0.05, 0.05, 1.0};
float mat_diffuse[] = {0.75, 0.75, 0.75, 1.0};
float mat_specular[] = {1.0, 1.0, 1.0, 1.0};
float mat_shininess[] = {50.0};

void idle()
{
    usleep(25000); // in microseconds
	g_spin += 1;
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

	// position and orient camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1, 1, 3, // eye position
			  0, 0, 0, // reference point
			  0, 1, 0  // up vector
		);

    // set the surface properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glPushMatrix();
		glRotatef(g_spin, 0, 1, 0);

		// auto generate texture coordinates 
		if (g_texture)
		{
			glBindTexture(GL_TEXTURE_2D, g_tex[g_curr]);

			// g_gen_mode sets type of O-mapping
	    	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, g_gen_mode);
   	 		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, g_gen_mode);

    		glEnable(GL_TEXTURE_GEN_S);
    		glEnable(GL_TEXTURE_GEN_T);
    		glEnable(GL_TEXTURE_2D);

    		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		glutSolidTeapot(0.6);

		if (g_texture)
		{
    		glDisable(GL_TEXTURE_2D);
  			glDisable(GL_TEXTURE_GEN_S);
  			glDisable(GL_TEXTURE_GEN_T);
		}

	glPopMatrix();

    // fast-key help on the screen
    glDisable(GL_LIGHTING);
        // put some help on the screen
        draw_text(20, 960, "Use 't' to toggle texturing");
        draw_text(20, 920, "Use ' ' to toggle spin");
        draw_text(20, 880, "Use '['/']' to cycle through textures");
        draw_text(20, 840, "Use 's' for sphere map coordinates");
        draw_text(20, 800, "Use 'o' for object linear coordinates");
        draw_text(20, 760, "Use 'e' for eye linear coordinates");
        draw_text(20, 700, "Use 'q' to quit");

		if (g_texture)
		{
			switch (g_gen_mode)
			{
        		case GL_SPHERE_MAP: draw_text(20, 40, "SPHERE MAP"); break;
        		case GL_EYE_LINEAR: draw_text(20, 40, "EYE LINEAR"); break;
        		case GL_OBJECT_LINEAR: draw_text(20, 40, "OBJECT LINEAR"); break;
			}
		}
    glEnable(GL_LIGHTING);

	glutSwapBuffers(); 
}

void keyboard(unsigned char key, int, int)
{
	switch (key)
	{
		case 'q': exit(1); break;

		case 't': g_texture = !g_texture; break;

		case ']':  g_curr = (g_curr+1)%3; break;
		case '[':  g_curr = (g_curr+2-1)%3; break;

		case ' ': 
				g_spinning = !g_spinning;
				if (g_spinning)
					glutIdleFunc(idle);
				else
					glutIdleFunc(NULL);
				break;

		case 's': 
				g_gen_mode = GL_SPHERE_MAP;
				break;
		case 'e': 
				g_gen_mode = GL_EYE_LINEAR;
				break;
		case 'o': 
				g_gen_mode = GL_OBJECT_LINEAR;
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
    float light_ambient[] = {0.1, 0.1, 0.1, 1.0};
    float light_diffuse[] = {0.5, 0.5, 0.5, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    float light_position[] = {1.0, 1.0, 2.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH); 

	// load texture
	g_tex[0] = load_and_bind_texture("../images/wood-grain.png");
	g_tex[1] = load_and_bind_texture("../images/clouds.png");
	g_tex[2] = load_and_bind_texture("../images/sphere-map.png");
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH); 
	glutInitWindowSize(640, 640); 
	glutInitWindowPosition(50, 50); 
	glutCreateWindow("Texture Coord Generation");
	glutKeyboardFunc(keyboard); 
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape); 

	init();

	glutMainLoop(); 

	return 0; 
}
