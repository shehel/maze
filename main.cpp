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

#include "load_and_bind_texture.h"

float cameraMoveSpeed = 0.1f;

bool first = true;
// angle of rotation for the camera direction
float angle=0.0;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
// XZ position of the camera
float x=0.0f,z=5.0f, y = 0;
float tilt = 0;

// Tracking the key states. These variables will be zero
//when no key is being presses
float turnAngle = 0;
float deltaAngle = 0.0f;
float deltaX = 0;
float deltaY = 0;

int xOrigin=256;

unsigned int g_program_obj = 0;
unsigned int g_vertex_obj = 0;
unsigned int g_fragment_obj = 0;

unsigned int g_wall = 0;
unsigned int g_ground = 1;

void reshape(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview

}


void wall(float x1, float x2, float y1, float y2, float z1, float z2){
	glBindTexture ( GL_TEXTURE_2D, g_wall);
	glBegin(GL_QUADS);
	glColor4f(1, 1, 1, 1);
	//Frontside
	glTexCoord2f(0,0);
	glVertex3f(x1, y1, z1);
        glTexCoord2f(1,0);
	glVertex3f(x2, y1, z1);
	glTexCoord2f(1,1);
	glVertex3f(x2, y2, z1);
	glTexCoord2f(0,1);
	glVertex3f(x1, y2, z1);


	//Backside
	glTexCoord2f(0,0);
	glVertex3f(x1, y1, z2);
	glTexCoord2f(1,0);
	glVertex3f(x2, y1, z2);
	glTexCoord2f(1,1);
	glVertex3f(x2, y2, z2);
	glTexCoord2f(0,1);
	glVertex3f(x1, y2, z2);


	//Top
	glTexCoord2f(0,0);
	glVertex3f(x1, y2, z1);
	glTexCoord2f(1,0);
	glVertex3f(x2, y2, z1);
	glTexCoord2f(1,1);
	glVertex3f(x2, y2, z2);
	glTexCoord2f(0,1);
	glVertex3f(x1, y2, z2);

	//Rightside
	glTexCoord2f(0,0);
	glVertex3f(x1, y1, z1);
	glTexCoord2f(1,0);
	glVertex3f(x1, y2, z1);
	glTexCoord2f(1,1);
	glVertex3f(x1, y2, z2);
	glTexCoord2f(0,1);
	glVertex3f(x1, y1, z2);


	//Leftside
	glTexCoord2f(0,0);
	glVertex3f(x2, y1, z1);
	glTexCoord2f(1,0);
	glVertex3f(x2, y2, z1);
	glTexCoord2f(1,1);
	glVertex3f(x2, y2, z2);
	glTexCoord2f(0,1);
	glVertex3f(x2, y1, z2);

	glEnd();
}

void drawFloor(GLfloat x1, GLfloat x2, GLfloat z1, GLfloat z2)
{
	glBindTexture ( GL_TEXTURE_2D, g_ground);
	glBegin(GL_POLYGON);
		//glColor4f(1, 1, 1, 1);
		glColor3f(0.9f, 0.9f, 0.9f);
		glNormal3f( 1.0, 1.0, 0.0);

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

void computePos(float deltaX, float deltaY) {



	x += deltaX * lx * cameraMoveSpeed;
	z += deltaX * lz * cameraMoveSpeed;

	float rightX = -lz;
	float rightZ = lx;

	x += deltaY * rightX * cameraMoveSpeed;
	z += deltaY * rightZ * cameraMoveSpeed;
}

void display(void)
{
	if (deltaX || deltaY)
		computePos(deltaX, deltaY);


	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);


	glLoadIdentity();
	gluLookAt(x, tilt, z, // eye position
			  x+lx, y, z+lz, // reference point
			  0, 1, 0  // up vector
		);

	glPushMatrix();
		glTranslatef(x+lx, -0.99, z+lz);
		glRotatef(turnAngle, 0, 1, 0);

	 	glBegin(GL_TRIANGLES);
			glColor3f(0, 1, 0);
			glVertex3f(0.5f, 0, 0.5f);
			glVertex3f(-0.5f, 0, 0.5f);
			glVertex3f(0, 0, -0.5f);
		glEnd();
	glPopMatrix();

	std::cerr << "x vava voom " << x+lx << " z for vava voom " << z+lz-1 <<std::endl;


	int a[12][12] = {
	   {0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,   /*  initializers for row indexed by 0 */
	   {2, 2, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0} ,   /*  initializers for row indexed by 1 */
	   {2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 0, 0} ,  /*  initializers for row indexed by 2 */
	   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0} ,
	   {0, 0, 2, 2, 0, 2, 0, 0, 0, 2, 0, 0},
	   {0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0} ,   /*  initializers for row indexed by 0 */
	   {0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0} ,   /*  initializers for row indexed by 1 */
	   {0, 0, 2, 2, 2, 0, 0, 0, 0, 2, 0, 0} ,  /*  initializers for row indexed by 2 */
	   {0, 2, 2, 2, 2, 2, 0, 0, 0, 2, 0, 0} ,
	   {0, 0, 2, 0, 0, 2, 0, 0, 0, 2, 0, 0},
	   {0, 0, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0},
	   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};
	//int xcomponent;
	int count=0;
	int icorrect;
	int jcorrect;
	for(int i = 0; i < 12; i++) {

		icorrect = 12;
		for(int j=0; j < 12; j++) {

			//jcorrect = j + 6;

			//std::cerr << a[icorrect][jcorrect] <<std::endl;
			if (a[i][j] == 0) {
				glPushMatrix();
					glTranslatef(2*i, 0, j*2);
					glEnable(GL_TEXTURE_2D);
					wall(1, -1, 1, -1, 1, -1);
					glDisable(GL_TEXTURE_2D);
				glPopMatrix();

			} else {
				glPushMatrix();
					glTranslatef(2*i, 0, j*2);
					glEnable(GL_TEXTURE_2D);
					drawFloor(1, -1, 1, -1);
					glDisable(GL_TEXTURE_2D);
				glPopMatrix();
			}
			count++;
		}
	}
	glutSwapBuffers();
}




void pressKey(unsigned char key, int xx, int yy) {
				std::cerr << key <<std::endl;
	switch (key) {

		case 'z': tilt = tilt + 25;
			break;
		case 'x': tilt = 0;
			break;
		case 'w' : deltaX = 0.5f; first = false; break;
		case 's' : deltaX = -0.5f; break;
		case 'd' : deltaY = 0.5f; break;
		case 'a' : deltaY = -0.5f; break;


	}
}

void releaseKey(unsigned char key, int x, int y) {

	switch (key) {

		case 'w' :
		case 's' : deltaX = 0;break;
		case 'a' :
		case 'd' : deltaY = 0;break;

	}
}

void mouseButton(int button, int state, int x, int y) {


}


void mouseMove(int x, int y) {


		turnAngle = -1 * (x - xOrigin);
		if (turnAngle>0){
			turnAngle = turnAngle + 10;
		} else if (turnAngle<0)
			turnAngle = turnAngle - 10;
		else
			turnAngle = 0;
		//std::cerr << "xOrigin " << xOrigin << " x "<< x << std::endl;
		deltaAngle = (x - xOrigin) * 0.02f;
		//std::cerr << "Angle " << (x-xOrigin)*1 << std::endl;
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);

}


void load_and_bind_textures()
{
	// load all textures here
	g_wall = load_and_bind_texture("./help.png");
	g_ground = load_and_bind_texture("./tile.png");
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
	//glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(256,256);
	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	int max_texture_units = 0;
    	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);
    	fprintf(stderr, "Max texture units is %d\n", max_texture_units);

	load_and_bind_textures();

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}
