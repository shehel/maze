#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <iostream>



#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>

bool g_spinning = false;
int g_angle = 0;
float eyex = 1;
float near = -3.0f;

// angle of rotation for the camera direction
float angle=0.0;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
// XZ position of the camera
float x=0.0f,z=5.0f, y = 0;
float tilt = 0;

// Tracking the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;

int xOrigin=256;

unsigned int g_program_obj = 0;
unsigned int g_vertex_obj = 0;
unsigned int g_fragment_obj = 0;

int g_ticks = 0; // keeps couting
int v_ticks_loc = 0; // location of ticker in vertex shader
int v_toggle_loc = 0; // location of action toggle flag

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0f, 0.1f, 100.0f);

	glutPostRedisplay();
}


void wall(float x1, float x2, float y1, float y2, float z1, float z2){
	glBegin(GL_QUADS);

	glColor3f(8.3, 0.2, 0.0);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y1, z1);
	glColor3f(1.3, 7.2, 0.0);
	glVertex3f(x2, y2, z1);
	glVertex3f(x1, y2, z1);

	glColor3f(0.3, 9.2, 2.0);
	glVertex3f(x1, y1, z2);
	glVertex3f(x2, y1, z2);
	glColor3f(1.3, 0.2, 9.0);
	glVertex3f(x2, y2, z2);
	glVertex3f(x1, y2, z2);

	glColor3f(0.3, 7.2, 0.0);
	glVertex3f(x1, y2, z1);
	glVertex3f(x2, y2, z1);
	glColor3f(0.3, 0.2, 9.0);
	glVertex3f(x2, y2, z2);
	glVertex3f(x1, y2, z2);

	glColor3f(0.3, 0.2, 8.0);
	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y2, z1);
	glColor3f(8.3, 0.2, 2.0);
	glVertex3f(x1, y2, z2);
	glVertex3f(x1, y1, z2);

	glColor3f(0.3, 0.2, 8.0);
	glVertex3f(x2, y1, z1);
	glVertex3f(x2, y2, z1);
	glColor3f(9.3, 0.2, 0.0);
	glVertex3f(x2, y2, z2);
	glVertex3f(x2, y1, z2);

	glEnd();
}

void drawFloor(GLfloat x1, GLfloat x2, GLfloat z1, GLfloat z2)
{
    //glBindTexture ( GL_TEXTURE_2D, texture );
    glBegin(GL_POLYGON);
	glColor3f(0.9f, 0.9f, 0.9f);
        glNormal3f( 0.0, 1.0, 0.0);
        glTexCoord2f(0,0);
        glVertex3f( x1, -1, z2 );
        glTexCoord2f(1,0);
        glVertex3f( x2, -1, z2 );
        glTexCoord2f(1,1);
        glVertex3f( x2, -1, z1 );
        glTexCoord2f(0,1);
        glVertex3f( x1, -1, z1 );
    glEnd();

}

void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

/*void computeDir(float deltaAngle) {

	angle += deltaAngle;
	lx = sin(angle);
	lz = -cos(angle);
}*/

void idle()
{
 bool post = false;

}



void display(void)
{
	if (deltaMove)
		computePos(deltaMove);



	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(x, tilt, z, // eye position
			  x+lx, y, z+lz, // reference point
			  0, 1, 0  // up vector
		);



	//TODO Player help when stuck by moving camera?
	// position and orient camera






	//TODO Draw the maze here
	int a[6][6] = {
	   {0, 0, 0, 2, 0, 0} ,   /*  initializers for row indexed by 0 */
	   {0, 2, 0, 2, 0, 0} ,   /*  initializers for row indexed by 1 */
	   {0, 0, 2, 2, 2, 0} ,  /*  initializers for row indexed by 2 */
	   {0, 2, 2, 2, 2, 2} ,
	   {0, 0, 2, 0, 0, 2},
	   {0, 0, 2, 0, 0, 0}
	};

	//int xcomponent;
	int count = 0;
	int icorrect;
	int jcorrect;
	for(int i = -3; i < 3; i++) {
		//xcomponent = 1;
		icorrect = i + 3;
		for(int j=-3; j < 3; j++) {
			jcorrect = j + 3;
			//std::cerr << a[icorrect][jcorrect] <<std::endl;
			if (a[icorrect][jcorrect] == 0) {
				glPushMatrix();
				glTranslatef(2*i, 0, j*2);
				wall(1, -1, 1, -1, 1, -1);

				//glPopMatrix();
				//glPushMatrix();
				//wall(0.1f, -0.1f, 1, -1, 1, -1);
				glPopMatrix();
							count++;
			} else {
				glPushMatrix();
				glTranslatef(2*i, 0, j*2);
				drawFloor(1, -1, 1, -1);
				glPopMatrix();
			}
		}
	}
	glutSwapBuffers();
}




void pressKey(unsigned char key, int xx, int yy) {
				std::cerr << key <<std::endl;
	switch (key) {

		case 'w' : deltaMove = 0.5f; break;
		case 's' : deltaMove = -0.5f; break;

	}
}

void releaseKey(unsigned char key, int x, int y) {

	switch (key) {

		case 'w' :
		case 's' : deltaMove = 0;break;
	}
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			angle -= deltaAngle;
			xOrigin = -1;
		}
		else {// state = GLUT_DOWN
			xOrigin = x;
		}
	}
}

void mouseMove(int x, int y) {



	if (xOrigin >= 0) {
		std::cerr << "ALl Eyes" << xOrigin <<std::endl;
		deltaAngle = (x - xOrigin) * 0.005f;

		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
}
int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(512,512);
	glutCreateWindow("3D Maze");

	//Callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);


	//Keyboard callbacks
	glutKeyboardFunc(pressKey);
	glutIgnoreKeyRepeat(1);
	glutKeyboardUpFunc(releaseKey);

	//Mouse Callbacks
	glutMouseFunc(mouseButton);
	glutPassiveMotionFunc(mouseMove);
	 glutSetCursor(GLUT_CURSOR_NONE);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}
