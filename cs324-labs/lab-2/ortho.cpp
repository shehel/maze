#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

void display()
{
	// clears to current background colour
	glClear(GL_COLOR_BUFFER_BIT); 
	
	// drawing colour
	glColor3f(1.0f, 1.0f, 1.0f); 

	// we should see this in our window
	glBegin(GL_LINE_LOOP); 
		glVertex2i(250, 250);
		glVertex2i(750, 250);
		glVertex2i(500, 750);
	glEnd();

	glutSwapBuffers(); 
}

void init()
{
	// select viewing transformation matrix to modify
	glMatrixMode(GL_PROJECTION); 

	// set identity on matrix
	glLoadIdentity();

	// specify bottom-left and top-right corners
	//     of window in world coordinates
	// takes LEFT, RIGHT, BOTTOM, TOP
	gluOrtho2D(0, 1000, 0, 1000);

	// set background colour and transparency
	// takes RGBA values
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f); // blue
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA); // flags bitwise OR'd together
	
	// display window size is in pixels on screen
	glutInitWindowSize(512, 512); 
	glutInitWindowPosition(50, 50); 

	glutCreateWindow("Ortho2D "); 

	glutDisplayFunc(display); 
	init(); // initialise the viewing 
	glutMainLoop(); 

	return 0; 
}
