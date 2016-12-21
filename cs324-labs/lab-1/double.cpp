#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

// redraw callback
void display()
{
	glClear(GL_COLOR_BUFFER_BIT); 
	
	// draw stuff -- into the backbuffer

	glColor3f(1.0f, 1.0f, 1.0f); // white

	glBegin(GL_POLYGON); // filled polygon
		glVertex2f(-0.5f, -0.5f); 
		glVertex2f(0.5f, -0.5f); 
		glVertex2f(0.5f, 0.5f); 
		glVertex2f(-0.5f, 0.5f); 
	glEnd();

	glutSwapBuffers(); // swap the backbuffer with the front
}


//
int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	
	// set the GL display mode: double buffering and RGBA colour mode
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA); // flags bitwise OR'd together
	
	glutInitWindowSize(512, 512); // window size
	glutInitWindowPosition(50, 50); // where on screen

	glutCreateWindow("Double Buffer"); 

	glutDisplayFunc(display); // set display callback

	glClearColor(0.0f, 1.0f, 0.0f, 0.0f); // clear colour -- green!
	
	glutMainLoop(); // go into the main loop and wait for window events...

	return 0; 
}
