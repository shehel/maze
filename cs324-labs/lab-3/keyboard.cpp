#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <stddef.h>
#include <iostream>

// values controlled by fast keys
float g_angle = 0.0f;
float g_xoffset = 0.0f;
float g_yoffset = 0.0f;

// increments
const float g_angle_step = 32.0f; // degrees
const float g_offset_step = 32.0f; // world coord units

void draw_triangle()
{
	// in model cooridnates centred at (0,0)
	static float vertex[3][2] =
		{
			{-1.0f, -1.0f},
			{1.0f, -1.0f},
			{0.0f, 1.0f}
		};

	glBegin(GL_LINE_LOOP); 
		for (size_t i=0;i<3;i++)
			glVertex2fv(vertex[i]);
	glEnd();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); 
	glColor3f(1.0f, 1.0f, 1.0f); 
	glLineWidth(2.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glTranslatef(500.0f+g_xoffset, 500.0f+g_yoffset, 0.0f);
		glScalef(100.0f, 100.0f, 1.0f);
		glRotatef(g_angle, 0.0f, 0.0f, 1.0f); 
		draw_triangle();
	glPopMatrix(); // done with stack
	glutSwapBuffers(); 
}

// will get which key was pressed and x and y positions if required
void keyboard(unsigned char key, int, int)
{
	std::cerr << "\t you pressed the " << key << " key" << std::endl;

	switch (key)
	{
		case 'q': exit(1); // quit!

		// clockwise rotate
		case 'r': g_angle += g_angle_step; break;
	}

	glutPostRedisplay(); // force a redraw
}

// any special key pressed like arrow keys
void special(int key, int, int)
{
	// handle special keys
	switch (key)
	{
		case GLUT_KEY_LEFT: g_xoffset -= g_offset_step; break;
		case GLUT_KEY_RIGHT: g_xoffset += g_offset_step; break;
		case GLUT_KEY_UP: g_yoffset += g_offset_step; break;
		case GLUT_KEY_DOWN: g_yoffset -= g_offset_step; break;
	}

	glutPostRedisplay(); // force a redraw
}

void init()
{
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluOrtho2D(0, 1000, 0, 1000);
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f); 
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA); 
	glutInitWindowSize(512, 512); 
	glutInitWindowPosition(50, 50); 
	glutCreateWindow("Keyboard Test"); 
	glutDisplayFunc(display); 

	// handlers for keyboard input
	glutKeyboardFunc(keyboard); 
	glutSpecialFunc(special); 

	init(); 
	glutMainLoop(); 

	return 0; 
}
