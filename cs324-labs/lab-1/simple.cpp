//
// Here are the required include files
// note that glut.h itself includes glu.h and gl.h
#ifdef __APPLE__
#include <GLUT/glut.h> // if we are on a Mac
#else
#include <GL/glut.h> // on linux
#endif

// ...define what OpenGL must do when asked to redraw the window
// 	note we can call this function anything we want but remember to
//  set your chosen name in the glutDisplayCallbackFunc() in main()
//
void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clear the window to default background colour
	
	glColor3f(1.0f, 0.0f, 0.0f); // set RGB values of colour to draw
	glBegin(GL_LINES); // draw lines between pairs of points
		glVertex2f(-0.5f, -0.5f); // from endpoint
		glVertex2f(0.5f, 0.5f); // to endpoint
	glEnd();

	glFlush(); // force the output of any draw operations 
}


//
// ...every C/C++ program needs a main()
// 	argc is the number of command line arguments
//  argv[] is an array of null terminated strings of command line arguments
//      note that argv[0] is the program name
//
int main(int argc, char* argv[])
{
	glutInit(&argc, argv); // we can pass certain X windows attributes to GLUT
	
	glutCreateWindow("Simple"); // a named window of default size and position
	glutDisplayFunc(display); // set a "display" or "redraw" callback
	
	glutMainLoop(); // go into the main loop and wait for window events...

	return 0; // safe to return 0 to shell unless error
}
