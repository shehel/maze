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

// last cursor click
int g_cursor_x = 0;
int g_cursor_y = 0;

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

// handles mouse click events
// button will say which button is presed, e.g. GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON
// state will say if the button is GLUT_UP or GLUT_DOWN
// x and y are the poitner position 
void mouse_click(int button, int state, int x, int y)
{
	if (button==GLUT_LEFT_BUTTON)
	{
		std::cerr << "\t left mouse button pressed!" << std::endl;
		
		if (state==GLUT_UP)
		{
			std::cerr << "\t button released...click finished" << std::endl;

			g_cursor_x = x;
			g_cursor_y = y;

			std::cerr << "\t cursor at (" << g_cursor_x << ", " << 
                                             g_cursor_y << ")" << std::endl;
		}

	}
	else
	if (button==GLUT_RIGHT_BUTTON)
	{
		std::cerr << "\t right mouse button pressed!" << std::endl;
	}
	glutPostRedisplay();
}

void mouse_motion(int x, int y)
{
	std::cerr << "\t mouse is at (" << x << ", " << y << ")" << std::endl;
	//glutPostRedisplay();
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

	g_cursor_x = g_cursor_y = 500; // middle of window
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA); 
	glutInitWindowSize(512, 512); 
	glutInitWindowPosition(50, 50); 
	glutCreateWindow("Mouse Test"); 
	glutDisplayFunc(display); 

	// handlers for keyboard input
	glutKeyboardFunc(keyboard); 
	glutSpecialFunc(special); 
	
	// mouse event handlers
	glutMouseFunc(mouse_click); 
	glutPassiveMotionFunc(mouse_motion); 

	init(); 
	glutMainLoop(); 

	return 0; 
}
