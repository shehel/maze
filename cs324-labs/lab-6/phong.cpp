#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <iostream>

#include <stdlib.h>
#include <stddef.h>

// properties of some material
float mat_ambient[] = {0.05, 0.05, 0.05, 1.0};
float mat_diffuse[] = {0.75, 0.75, 0.75, 1.0};
float mat_specular[] = {1.0, 1.0, 1.0, 1.0};
float mat_shininess[] = {50.0};

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

	glutSolidTeapot(0.4);
	glutSwapBuffers(); 
}

void keyboard(unsigned char key, int, int)
{
		switch (key)
	{
		case 't': exit(1); break;

		case '1': 	set_material(white_shiny); break;
		case '2':   set_material(red_plastic); break;
		case '3':   set_material(brass); break;
		case 'Q':	glEnable(GL_LIGHT0);break;
		case 'W':	glEnable(GL_LIGHT1);break;
		case 'E':	glEnable(GL_LIGHT2);break;
		case 'q':	glDisable(GL_LIGHT0);break;
		case 'w':	glDisable(GL_LIGHT1);break;
		case 'e':	glDisable(GL_LIGHT2);break;
 		// code to change light spin angle
		case '4': 
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
	// we can do this once if we don't intend to change the light properties
	// incident energy properties of a light
	float light_ambient[] = {0.1, 0.1, 0.1, 1.0};
	float light_diffuse[] = {0.5, 0.5, 0.5, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	// fix the light position
	float light_position[] = {1.0, 1.0, 2.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	// enable lighting and turn on the light0
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// so that hidden surfaces are removed
	glEnable(GL_DEPTH_TEST);

	// mode of shading
	glShadeModel(GL_FLAT); // can be GL_FLAT, GL_SMOOTH
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH); 

	glutInitWindowSize(512, 512); 
	glutInitWindowPosition(50, 50); 

	glutCreateWindow("Phong"); 

	glutKeyboardFunc(keyboard); 
	glutReshapeFunc(reshape); 
	glutDisplayFunc(display); 

	init(); 

	glutMainLoop(); 

	return 0; 
}
