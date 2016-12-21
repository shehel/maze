#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <stddef.h>
#include <string.h>
#include <iostream>

unsigned int g_bitmap_text_handle = 0;

unsigned int make_bitmap_text()
{
	unsigned int handle_base = glGenLists(256); 

	for (size_t i=0;i<256;i++)
	{
		// a new list for each character
		glNewList(handle_base+i, GL_COMPILE);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, i);
		glEndList();
	}
	return handle_base;
}

void draw_text(const char* text)
{
	glListBase(g_bitmap_text_handle);
	glCallLists(int(strlen(text)), GL_UNSIGNED_BYTE, text);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); 

	glColor3f(1.0f, 1.0f, 1.0f); 

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glTranslatef(500.0f, 500.0f, 0.0f); // this will work
		glScalef(10.0f, 10.0f, 1.0f); // this has no effect
		glRotatef(30.0f, 0.0f, 0.0f, 1.0f); // this has no effect

		glRasterPos2i(0, 0); // centre the text
		draw_text("Hello, World!");
	glPopMatrix();

	glutSwapBuffers(); 
}

void keyboard(unsigned char key, int, int)
{
	switch (key)
	{
		case 'q': exit(1); 
	}
	glutPostRedisplay(); 
}

void init()
{
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluOrtho2D(0, 1000, 0, 1000);
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f); 

	// make all bitmap text characters 
	g_bitmap_text_handle = make_bitmap_text();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA); 
	glutInitWindowSize(512, 512); 
	glutInitWindowPosition(50, 50); 
	glutCreateWindow("Bitmap Text"); 
	glutDisplayFunc(display); 
	glutKeyboardFunc(keyboard); 
	init(); 
	glutMainLoop(); 

	return 0; 
}
