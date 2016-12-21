#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <stddef.h>
#include <stdlib.h>

size_t g_state = 0;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); 
	
	if (g_state==0)
	{
		glPointSize(2.0f);
		glBegin(GL_POINTS); 
		glColor3f(1.0f, 1.0f, 1.0f); // white
		glBegin(GL_TRIANGLES); // filled triangle
			glVertex2f(-0.5f, -0.5f); 
			glVertex2f(0.5f, -0.5f); 
			glVertex2f(0.0f, 0.47f); 
		glEnd();
	}
	else if(g_state==1)
	{
		glLineWidth(2.0f);
		glColor3f(1.0f, 1.0f, 1.0f); // white
		glBegin(GL_LINE_LOOP); 
			glVertex2f(-0.5f, -0.5f); 
			glVertex2f(0.5f, -0.5f); 
			glVertex2f(0.0f, 0.47f); 
		glEnd();
	}
	else if (g_state==2)
	{
		// constant colour
		glBegin(GL_TRIANGLES); 
			glColor3f(1.0f, 1.0f, 1.0f); 
			glVertex2f(-0.5f, -0.5f); 
			glColor3f(1.0f, 1.0f, 1.0f); 
			glVertex2f(0.5f, -0.5f); 
			glColor3f(1.0f, 1.0f, 1.0f); 
			glVertex2f(0.0f, 0.47f); 
		glEnd();
	}
	else if (g_state==3)
	{
		// interpolate colours along lines
		glLineWidth(2.0f);
		glBegin(GL_LINE_LOOP); 
			glColor3f(1.0f, 0.0f, 0.0f); // red
			glVertex2f(-0.5f, -0.5f); 
			glColor3f(0.0f, 1.0f, 0.0f); // green
			glVertex2f(0.5f, -0.5f); 
			glColor3f(0.0f, 0.0f, 1.0f); // blue
			glVertex2f(0.0f, 0.47f); 
		glEnd();
	}
	else if (g_state==4)
	{
		// interpolate colours across surface
		glBegin(GL_TRIANGLES); // filled triangle
			glColor3f(1.0f, 0.0f, 0.0f); // red
			glVertex2f(-0.5f, -0.5f); 
			glColor3f(0.0f, 1.0f, 0.0f); // green
			glVertex2f(0.5f, -0.5f); 
			glColor3f(0.0f, 0.0f, 1.0f); // blue
			glVertex2f(0.0f, 0.47f); 
		glEnd();
	}

	glutSwapBuffers(); // swap the backbuffer with the front
}

void keyboard(unsigned char key, int, int)
{
	switch (key)
	{
		case 'q': exit(0); break;

		case ' ':	
				g_state = (g_state+1)%5;
				break;
	}
	glutPostRedisplay();
}
	

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA); 
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Fixed Pipeline"); 
	glutDisplayFunc(display); 
	glutKeyboardFunc(keyboard);
	glutMainLoop(); 

	return 0; 
}
