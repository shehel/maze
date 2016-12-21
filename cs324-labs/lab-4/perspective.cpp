#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <iostream>

#include <stdlib.h>
#include <stddef.h>

#include "draw_text.h"
#include "lights_material.h"

float g_eye_z = 2.0f;
bool g_draw_teapot = false; // toggle

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
	
	// position and orient camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1, 1, g_eye_z, // eye position
			  0, 0, 0, // reference point
			  0, 1, 0  // up vector
		);

	glColor3f(1.0f, 1.0f, 1.0f); 

	if (!g_draw_teapot)
	{
		glDisable(GL_LIGHTING);
		glutWireCube(0.5);  
		glEnable(GL_LIGHTING);
	}
	else
		glutSolidTeapot(0.4);

	glDisable(GL_LIGHTING);
		// put some help on the screen
		draw_text(20, 20, "Use 'z'/'Z' to change eye Z position");
	glEnable(GL_LIGHTING);


	glutSwapBuffers(); 
}

void keyboard(unsigned char key, int, int)
{
	switch (key)
	{
		case 'q': exit(1); break;
		case 'z': g_eye_z += 0.5f; break;
		case 'Z': g_eye_z -= 0.5f; break;

		case 't': g_draw_teapot = !g_draw_teapot; break;
	}
	std::cerr << "\t eye z  = " << g_eye_z << std::endl;

	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	// for perspective projection use the GLU helper
	// take FOV, ASPECT RATIO, NEAR, FAR 
	gluPerspective(40.0, 1.0f, 1.0, 5.0);
}

void init()
{
	init_lights();
	init_material();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH); 
	
	glutInitWindowSize(512, 512); 
	glutInitWindowPosition(50, 50); 

	glutCreateWindow("Perspective Viewing"); 

	glutKeyboardFunc(keyboard); 
	glutReshapeFunc(reshape); 
	glutDisplayFunc(display); 

	init(); 

	glutMainLoop(); 

	return 0; 
}
