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
const float g_angle_step = 10.0f; // degrees
const float g_offset_step = 10.0f; // world coord units

// so we can install and de-install an idle handler
bool g_idle_installed = false;

// handle for display list
unsigned int g_the_triangle = 0;

unsigned int make_triangle()
{
	static float vertex[3][2] =
		{
			{-1.0f, -1.0f},
			{1.0f, -1.0f},
			{0.0f, 1.0f}
		};

	// request a single display list handle
	unsigned int handle = glGenLists(1); 

	glNewList(handle, GL_COMPILE);

	glBegin(GL_LINE_LOOP); 
		for (size_t i=0;i<3;i++)
			glVertex2fv(vertex[i]);
	glEnd();
	glEndList();

	return handle;
}

// our idle handler
void idle()
{
	std::cerr << "\t idle handler called..." << std::endl;
	//glutPostRedisplay(); // uncomment if you change any drawing state
}

void visibility(int vis)
{
	if (vis==GLUT_VISIBLE)
	{
		std::cerr << "\t visible" << std::endl;
		if (g_idle_installed)
		{
			glutIdleFunc(idle);
		}
	}
	else
	{
		std::cerr << "\t NOT visible" << std::endl;
		glutIdleFunc(NULL);
	}
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
		
		// execute a pre-compiled display list
		glCallList(g_the_triangle);

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

		case 'a':
				if (!g_idle_installed)
				{
					glutIdleFunc(idle);
					g_idle_installed = true;
				}
				else
				{
					glutIdleFunc(NULL);
					g_idle_installed = false;
				}
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

	// make triangle display list
	g_the_triangle = make_triangle();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA); 
	glutInitWindowSize(512, 512); 
	glutInitWindowPosition(50, 50); 
	glutCreateWindow("Idle Test"); 
	glutDisplayFunc(display); 

	// handlers for keyboard input
	glutKeyboardFunc(keyboard); 
	glutSpecialFunc(special); 

	// visibility
	glutVisibilityFunc(visibility); 

	init(); 
	glutMainLoop(); 

	return 0; 
}
