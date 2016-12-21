#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <stdlib.h>
#include <stddef.h>

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

	// work on MODELVIEW matrix stack
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

		// translate it to where we want it
		glTranslatef(500.0f, 500.0f, 0.0f);

		// scale it up 
		glScalef(250.0f, 250.0f, 1.0f);

		// rotate anticlockwise in-plane by 60 degrees
		glRotatef(60.0f, 0.0f, 0.0f, 1.0f); 


		// render primitives
		draw_triangle();

	glPopMatrix(); // done with stack

	glutSwapBuffers(); 
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
	glutCreateWindow("Rotate"); 
	glutDisplayFunc(display); 

	init(); 
	glutMainLoop(); 

	return 0; 
}
