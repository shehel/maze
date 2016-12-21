#ifdef __APPLE__
#include <GLUT/glut.h> // if we are on a Mac
#else
#include <GL/glut.h> // on linux
#endif

#include <stdio.h> 
#include <stdlib.h> 

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clear the window to default background colour
	
	glColor3f(1.0f, 0.0f, 0.0f); // set RGB values of colour to draw
	glBegin(GL_LINES); // draw lines between pairs of points
		glVertex2f(-0.5f, -0.5f); // from endpoint
		glVertex2f(0.5f, 0.5f); // to endpoint
	glEnd();

	glutSwapBuffers(); // force the output of any draw operations 
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv); // we can pass certain X windows attributes to GLUT

    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE|GLUT_DOUBLE|GLUT_RGBA);

    glutCreateWindow("Test Version 3.2");

    fprintf(stderr, "Open GL version %s\n", glGetString(GL_VERSION));
    fprintf(stderr, "Shading Language version %s\n",
            glGetString(GL_SHADING_LANGUAGE_VERSION));


	glutDisplayFunc(display); // set a "display" or "redraw" callback
	
	glutMainLoop(); // go into the main loop and wait for window events...

	return 0; // safe to return 0 to shell unless error
}
