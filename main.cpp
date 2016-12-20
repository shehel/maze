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
#include <vector>
#include <cmath>
#include <math.h>

#include <string.h>

#include "load_and_bind_texture.h"

float cameraMoveSpeed = 0.1f;


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

float diffuse = 0.75;
float specular = 1.0f;
float exponent = 25.0f;
float shininess = 50;
float ambient = 0.1f;
	float light_ambient[] = {ambient, ambient, ambient, 1.0};
		float light_diffuse[] = {diffuse, diffuse, diffuse, 1.0};
bool toggle = false;
float light_position[] = {1.0, 1.0, 2.0, 0.0};
float lighty = 2.0;
float lightz = 2.0;

int xOrigin=256;

unsigned int g_program_obj = 0;
unsigned int g_vertex_obj = 0;
unsigned int g_fragment_obj = 0;

unsigned int g_wall = 0;
unsigned int g_ground = 1;

unsigned int g_bitmap_text_handle = 0;
struct materials_t
{
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float shininess;
};
struct light_t
{
	size_t name;
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float position[4];
};

const materials_t white_shiny = {
	{0.1f, 0.1f, 0.1f, 1.0f},
	{0.5f, 0.5f, 0.5f, 1.0f},
	{0.5f, 0.5f, 0.5f, 1.0f},
	100.0f
};

light_t light_0 = {
	GL_LIGHT0,
	{0.11f, 0.1f, 0.1f, 1.0f},
	{1, 0, 0, 1.0f},
	{1, 0.5f, 0.5f, 1.0f},
	{1.7, 0, 2, 1}

};

light_t light_1 = {
	GL_LIGHT1,
	{0.11f, 0.1f, 0.1f, 1.0f},
	{1, 1, 1, 1.0f},
	{0.5f, 0.5f, 0.5f, 1.0f},
	{.65, 1, .714, 1}

};

light_t light_2 = {
	GL_LIGHT2,
	{0.0f, 0.0f, 0.0f, 1.0f},
	{0.0f, 1.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f},
	{12, 0, 12, 1}

};

// properties of a given material
void set_material(const materials_t& mat)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat.ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat.diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat.specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat.shininess);
}

// set a given light
void set_light(const light_t& light, float x, float y)
{

			float position[4] = {x, 1, y, 1};
			glLightfv(light.name, GL_AMBIENT, light.ambient);
			glLightfv(light.name, GL_DIFFUSE, light.diffuse);
			glLightfv(light.name, GL_SPECULAR, light.specular);
			glLightfv(light.name, GL_POSITION, position);


			//12 make the lights spot lights here if you want
			float direction[3] = {
						-light.position[0],
						-light.position[1],
						-light.position[2]};
			glLightfv(light.name, GL_SPOT_DIRECTION, direction);
			glLightf(light.name, GL_SPOT_CUTOFF, 180.0f);

			glEnable(light.name);
	//}
}

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


struct wall
{
	unsigned int tex; // texture handle
	float position[3]; // world coordinate
};

std::vector<wall> walls; // a number of particles


int a[12][12] = {
	   {2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0} ,   /*  initializers for row indexed by 0 */
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


void drawWall(float x1, float x2, float y1, float y2, float z1, float z2){
	glBindTexture ( GL_TEXTURE_2D, g_wall);
	glBegin(GL_QUADS);
	glColor4f(1, 1, 1, 1);

	//Rightside
	glNormal3f(0, 0, 1);
	glTexCoord2f(0,0);
	glVertex3f(x1, y1, z1);
        glTexCoord2f(1,0);
	glVertex3f(x2, y1, z1);
	glTexCoord2f(1,1);
	glVertex3f(x2, y2, z1);
	glTexCoord2f(0,1);
	glVertex3f(x1, y2, z1);


	//Leftside
	glNormal3f(0, 0, -1);
	glTexCoord2f(0,0);
	glVertex3f(x1, y1, z2);
	glTexCoord2f(1,0);
	glVertex3f(x2, y1, z2);
	glTexCoord2f(1,1);
	glVertex3f(x2, y2, z2);
	glTexCoord2f(0,1);
	glVertex3f(x1, y2, z2);


	//Top
	glNormal3f (0,1, 0);
	glTexCoord2f(0,0);
	glVertex3f(x1, y2, z1);
	glTexCoord2f(1,0);
	glVertex3f(x2, y2, z1);
	glTexCoord2f(1,1);
	glVertex3f(x2, y2, z2);
	glTexCoord2f(0,1);
	glVertex3f(x1, y2, z2);

	//Backside
	glNormal3f (-1,0, 0);
	glTexCoord2f(0,0);
	glVertex3f(x1, y1, z1);
	glTexCoord2f(0,1);
	glVertex3f(x1, y2, z1);
	glTexCoord2f(1,1);
	glVertex3f(x1, y2, z2);
	glTexCoord2f(1,0);
	glVertex3f(x1, y1, z2);


	//Frontside
	glNormal3f (-1,0, 0);
	glTexCoord2f(0,0);
	glVertex3f(x2, y1, z1);
	glTexCoord2f(0,1);
	glVertex3f(x2, y2, z1);
	glTexCoord2f(1,1);
	glVertex3f(x2, y2, z2);
	glTexCoord2f(1,0);
	glVertex3f(x2, y1, z2);

	glEnd();
}

void drawFloor(GLfloat x1, GLfloat x2, GLfloat z1, GLfloat z2)
{
	glBindTexture ( GL_TEXTURE_2D, g_ground);
	glBegin(GL_POLYGON);
		glNormal3f( 0.0, 1.0, 0.0);
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

bool checkCollision() {

	float camWorldX = (x+lx)/2;
	float camWorldY = (z+lz)/2;

	int truncX = static_cast<int>(camWorldX);
	int truncY = static_cast<int>(camWorldY);

	int roundedY = round(camWorldY);
	int roundedX = round(camWorldX);
	std::cerr << camWorldX << " CamWorld " << camWorldY <<std::endl;
	std::cerr << "Is it a wall? "  << a[roundedX][roundedY] <<std::endl;
	if (a[roundedX][roundedY] == 0) {
		if (camWorldX > truncX+0.5 || truncY > 1) {
				std::cerr << "Worlds collide" <<std::endl;
			return true;
			}
		else return false;
	} else
	 return false;



}

void computePos(float deltaX, float deltaY) {

	float oldx = x;
	float oldz = z;

	x += deltaX * lx * cameraMoveSpeed;
	z += deltaX * lz * cameraMoveSpeed;

	float rightX = -lz;
	float rightZ = lx;

	x += deltaY * rightX * cameraMoveSpeed;
	z += deltaY * rightZ * cameraMoveSpeed;

	if (checkCollision ()) {
		x = oldx;
		z = oldz;
	}
}

void display(void)
{
	if (deltaX || deltaY) {
		computePos(deltaX, deltaY);

	}

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);


	glLoadIdentity();


	gluLookAt(x, tilt, z, // eye position
			  x+lx, y, z+lz, // reference point
			  0, 1, 0  // up vector
		);
	//Map Blip Triangle
	glPushMatrix();
		glTranslatef(x+lx, -0.99, z+lz);
		glRotatef(turnAngle, 0, 1, 0);

	 	glBegin(GL_TRIANGLES);
	 		glNormal3f (0,1, 0);
			glColor3f(0, 1, 0);
			glVertex3f(0.5f, 0, 0.5f);
			glVertex3f(-0.5f, 0, 0.5f);
			glVertex3f(0, 0, -0.5f);
		glEnd();
	glPopMatrix();
	glDisable(GL_LIGHTING);
	glPushMatrix();

		// perform any transformations of light positions here
		glPointSize(5.0f);
		set_light(light_1, -2, -5);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	//Text
	glPushMatrix();
		glColor3f(1.0f, 1, 1);

		glTranslatef(-2, 0, -5); // this will work
		glRasterPos2i(0, 0); // centre the text
		draw_text("Hello! The Maze starts here. Find the teapot.");
	glPopMatrix();


	glPushMatrix ();
		glColor4f(1, 1, 1, 1);
		glTranslatef(20, -0.5, 18);
		glutSolidTeapot(0.8);
	glPopMatrix ();


	/*glDisable(GL_LIGHTING);
					glPushMatrix();

					// perform any transformations of light positions here
					glRotatef(20, 0, 1, 0);
					glPointSize(3.0f);
					set_light(light_1);

				glBegin(GL_POINTS);
					glColor3f (1, 0, 0);
					glVertex3fv(light_1.position);


				glEnd();

	glPopMatrix();
	glEnable(GL_LIGHTING);*/

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
					drawWall(1, -1, 1, -1, 1, -1);
					glDisable(GL_TEXTURE_2D);
				glPopMatrix();



			} else {
				glPushMatrix();
					glTranslatef(2*i, 0, j*2);
					glEnable(GL_TEXTURE_2D);
					drawFloor(1, -1, 1, -1);
					glDisable(GL_TEXTURE_2D);
				glPopMatrix();
				glDisable(GL_LIGHTING);
				glPushMatrix();

					// perform any transformations of light positions here
					glRotatef(0, 0, 1, 0);
					glPointSize(3.0f);
					set_light(light_1, 2*i, j*2);

				glBegin(GL_POINTS);
					glColor3f (1, 1, 1);
					glVertex3f(2*i, 1, j*2);
				glEnd();
				glPopMatrix();
				glEnable(GL_LIGHTING);
			}
		}
	}
	glutSwapBuffers();
}




void pressKey(unsigned char key, int xx, int yy) {

	switch (key) {

		case 'z': tilt = tilt + 25;
			break;
		case 'x': tilt = 0;
			break;
		case 'w' :
				deltaX = 0.5f;
				break;

		case 's' : //
				deltaX = -0.5f;
				break;

		case 'd' : //
				deltaY = 0.5f; break;
				break;

		case 'a' : //
				deltaY = -0.5f; break;



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

void initWalls() {



}


void load_and_bind_textures()
{
	// load all textures here
	g_wall = load_and_bind_texture("./help.png");
	g_ground = load_and_bind_texture("./tile.png");
}

void makeLightSource() {
	float light_ambient[] = {0.1, 0.1, 0.1, 1.0};
	float light_diffue[] = {0.5, 0.5, 0.5, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	// fix the light position

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	// enable lighting and turn on the light0
	//glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// so that hidden surfaces are removed
	glEnable(GL_DEPTH_TEST);

	// mode of shading
	glShadeModel(GL_SMOOTH); // can be GL_FLAT, GL_SMOOTH
}

void init() {


	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

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
	initWalls();
	init();
	g_bitmap_text_handle = make_bitmap_text();

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}

