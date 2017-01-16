#include <GL/glut.h>


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

/****************************
VARIABLE DECLARATIONS
****************************/

//Window and size variables
int mainWindow;
int SizeX=800;
int SizeY=600;

//Origin for mouse
int xOrigin=SizeX/2;
int yOrigin=SizeX/2;

float cameraMoveSpeed = 0.1f;

//Booleans for tracking map states
bool mapMode = false;
bool win = false;

//Angle of rotation, direction vector of camera and position vector of camera
float angle=0.0;
float lx=0.0f,lz=-1.0f, ly=0;
float tilt = 0;
float x=0,z=4, y = 0;

float origTilt;
float origLy;



//Angle for blip
float turnAngle = 0;

//Deltas for camera movement
float deltaAngle = 0.0f;
float deltaAngleY = 0.0f;
float deltaX = 0;
float deltaZ = 0;


//Texture variables
unsigned int g_program_obj = 0;
unsigned int g_vertex_obj = 0;
unsigned int g_fragment_obj = 0;

unsigned int g_wall = 0;
unsigned int g_ground = 1;

//Text variable
unsigned int g_bitmap_text_handle = 0;

//Datastructures for lights and material properties
struct materials_t
{
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float shininess;
};

struct light_t
{
	int id;
	size_t name;
	float ambient[4];
	float diffuse[4];
	float specular[4];
};

//Materials
const materials_t materialM = {
	{0.33f, 0.22f, 0.03f, 1.0f},
	{0.78f, 0.57f, 0.11f, 1.0f},
	{0.90f, 0.8f, 0.7f, 1.0f},
	27.8f
};

//Universal light (Shade of white)
light_t light_1 = {
	1, GL_LIGHT1,
	{ 0.01, 0.01, 0.01, 1 },
	{ 0.5, 0.5, 0.5, 1 },
	{0.3f, 0.3f, 0.3f, 1.0f}
};

//Yellowish light for teapot
light_t light_2 = {
	2, GL_LIGHT2,
	{ 0.1, 0.1, 0.1, 1 },
	{ 0.7, 0.8, 0.5, 1 },
	{0.5f, 0.5f, 0.5f, 1.0f}
};

/****************************
FUNCTIONS
****************************/

//Properties of a given material
void set_material(const materials_t& mat)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat.ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat.diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat.specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat.shininess);
}

//Set a given light
void set_light(const light_t& light, float lightx, float lightz)
{
	float lighty;
	float intensity;
	float dir;

	//Different properties for teapot light
	if (light.id == 1) {
		lighty = 5;
		intensity = 91;
		dir = 1;
	} else {
		lighty = 2;
		intensity = 50;
		dir = -1;
	}

	float position[4] = {lightx, lighty, lightz, 1};
	glLightfv(light.name, GL_AMBIENT, light.ambient);
	glLightfv(light.name, GL_DIFFUSE, light.diffuse);
	glLightfv(light.name, GL_SPECULAR, light.specular);
	glLightfv(light.name, GL_POSITION, position);

	//Spotlight adjustment calls
	float direction[3] = {0, dir, 0};
	glLightfv(light.name, GL_SPOT_DIRECTION, direction);

	glLightf(light.name, GL_SPOT_CUTOFF, intensity);


	glEnable(light.name);
}

//Function for handling menu events
void processMenuEvents(int option)
{
	//Exit cleanly
	if (option == 2) {
		glutDestroyWindow(mainWindow);
		exit(0);
	}

}

//Menu creator function for mouse RB clicks
void createGLUTMenus()
{
	int menu;

	//"processMenuEvents" will handle events
	menu = glutCreateMenu(processMenuEvents);

	//Adding entries to the menu
	glutAddMenuEntry("Find the Golden Teapot. HOLD Z for Map View.",1);
	glutAddMenuEntry("Exit Maze",2);

	//Attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//Bitmap text function
unsigned int make_bitmap_text()
{
	unsigned int handle_base = glGenLists(256);

	for (size_t i=0;i<256;i++)
	{
		// a new list for each character
		glNewList(handle_base+i, GL_COMPILE);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, i);
		glEndList();
	}
	return handle_base;
}

void draw_text(const char* text)
{
	glListBase(g_bitmap_text_handle);
	glCallLists(int(strlen(text)), GL_UNSIGNED_BYTE, text);
}

//Array for map layout. 0 for wall cubes and 2 for ground
int a[12][12] = {
	   {2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
	   {0, 2, 0, 2, 0, 0, 0, 2, 2, 2, 2, 0} ,
	   {0, 2, 2, 2, 2, 0, 0, 2, 0, 2, 2, 0} ,
	   {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0} ,
	   {0, 0, 2, 2, 0, 2, 0, 0, 0, 2, 0, 0},
	   {0, 0, 0, 2, 0, 0, 0, 2, 0, 2, 0, 0} ,
	   {0, 0, 0, 2, 0, 0, 0, 2, 0, 2, 0, 0} ,
	   {0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0} ,
	   {0, 2, 2, 2, 0, 2, 0, 2, 0, 2, 0, 0} ,
	   {0, 0, 2, 2, 0, 0, 2, 2, 2, 2, 2, 0},
	   {0, 0, 2, 2, 0, 0, 0, 0, 2, 2, 2, 0},
	   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

//Function for window resize
void reshape(int w, int h)
{
	SizeX=w;
	SizeY=h;

	xOrigin = SizeX/2;
	yOrigin = SizeY/2;

	//Prevent divide by 0
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
}

//Function to draw wall cube
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
	glNormal3f (1,0, 0);
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

//Function to draw floor square
void drawFloor(GLfloat x1, GLfloat x2, GLfloat z1, GLfloat z2)
{
	glBindTexture ( GL_TEXTURE_2D, g_ground);
	glBegin(GL_POLYGON);
		glNormal3f( 0.0, 1.0, 0.0);
		//glColor4f(1, 1, 1, 1);
		glColor3f(0.9f, 0.9f, 0.9f);

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

//Trivial collision detection based on position of cubes in the map
//Based on what is front, if close to the wall, return true
bool checkCollision()
{
	float camWorldX = (x+lx)/2;
	float camWorldZ = (z+lz)/2;

	int truncX = static_cast<int>(camWorldX);
	int truncZ = static_cast<int>(camWorldZ);

	int roundedZ = round(camWorldZ);
	int roundedX = round(camWorldX);

	if (a[roundedX][roundedZ] == 0) {
		if (camWorldX > truncX+0.5 || truncZ > 1) {
			return true;
			}
		else return false;
	} else
	 return false;
}

//Function to compute X and Z position
void computePos(float deltaX, float deltaZ)
{
	float oldx = x;
	float oldz = z;

	//Front and back movement
	x += deltaX * lx * cameraMoveSpeed;
	z += deltaX * lz * cameraMoveSpeed;

	float rightZ = -lz;
	float rightX = lx;

	//Left and right movement
	x += deltaZ * rightZ * cameraMoveSpeed;
	z += deltaZ * rightX * cameraMoveSpeed;

	//Don't allow movement is collision is true
	if (checkCollision ()) {
		x = oldx;
		z = oldz;
	}
}

void display(void)
{
	//Check if goal is reached
	if (x > 16 & x < 19 & z > 16 & z < 19) {
		win = true;
	}

	//If there has been change in X and Z position, compute new position
	if (deltaX || deltaZ) {
		computePos(deltaX, deltaZ);
	}

	y = tilt+ly;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//Color for background
	glClearColor(0.139, 0.134, 0.130, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Stop fps view when goal is reached
	if (win) {
		mapMode = true;
	}

	//Incremental zoomin and zoomout
	if (mapMode) {
		if ( tilt != 75)
		tilt++;
		y = 5;
	} else if (!mapMode & tilt > origTilt) {
		tilt--;
		y = origLy;
	} else {
		tilt = 0;
		y = tilt+ly;
	}

	//Camera vector
	gluLookAt(x, tilt, z, // eye position
			  x+lx, y, z+lz, // reference point
			  0, 1, 0  // up vector
		);

	//Light for triangle blip
	glDisable(GL_LIGHTING);
	glPushMatrix();
		glPointSize(5.0f);
		set_light(light_1, x+lx, z+lz);
	glPopMatrix();
	glEnable(GL_LIGHTING);

	//Drawing the blip for camera position
	glPushMatrix();
		glTranslatef(x+lx, -0.99, z+lz);
		glRotatef(turnAngle, 0, 1, 0);
	 	glBegin(GL_TRIANGLES);
	 		glNormal3f (0,1, 0);
			glColor3f(1, 0, 0);
			glVertex3f(0.5f, 0, 0.5f);
			glVertex3f(-0.5f, 0, 0.5f);
			glVertex3f(0, 0, -0.5f);
		glEnd();
	glPopMatrix();

	//Changes to be made when goal is reached
	if (!win) {
		//Lighting for text
		glDisable(GL_LIGHTING);
		glPushMatrix();
			// perform any transformations of light positions here
			glPointSize(5.0f);
			set_light(light_1, -3, -4);
		glPopMatrix();
		glEnable(GL_LIGHTING);

		//Helper text
		glPushMatrix();
			glColor3f(1, 1, 1);
			glTranslatef(-2, 1, -1); // this will work
			glRasterPos2i(0, 0); // centre the text
			draw_text("Hello! The Maze starts here. Right MB for help. GL.");
		glPopMatrix();
	} else {
		//Light for text
		glPushMatrix();
			glPointSize(5.0f);
			set_light(light_1, 15, 15);
		glPopMatrix();
		glEnable(GL_LIGHTING);

		//Text when goal is reached
		glPushMatrix();
			glColor3f(1, 1, 1);
			glTranslatef(16, 1, 16); // this will work
			glRasterPos2i(0, 0); // centre the text
			draw_text("____YOU WIN____");
		glPopMatrix();
	}

	//Light for teapot
	glDisable(GL_LIGHTING);
	glPushMatrix();
		set_light(light_2, 20, 18);
	glPopMatrix();
	glEnable(GL_LIGHTING);

	//Teapot
	glPushMatrix ();
		glColor3f(.255,.215, 0);
		glTranslatef(20, -0.6, 18);
		glutSolidTeapot(0.6);
	glPopMatrix ();

	//Light for moon
	glDisable(GL_LIGHTING);
	glPushMatrix();
		set_light(light_1, 70, 70);
	glPopMatrix();
	glEnable(GL_LIGHTING);

	//Drawing moon
	glPushMatrix ();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(60.0f, 26.0f, 60.0f);
		glutSolidSphere(5.0f,20,10);
	glPopMatrix ();

	float stary;

	for(int i = 0; i < 12; i++) {
		for(int j=0; j < 12; j++) {

			//Stars at varying heights
			if (i > 6 & j > 6) {
				glPointSize(3.0f);
				stary = 3;
			} else if (i < 6 & j < 6) {
				glPointSize (4.0f);
				stary = 25;
			}

			//Draw points for stars
			glPushMatrix ();
				glBegin(GL_POINTS);
					glColor3f (1, 1, 1);
					glVertex3f(50.3*i, stary, j*37.4);
				glEnd();
			glPopMatrix();

			//Draw a wall
			if (a[i][j] == 0) {
				glPushMatrix();
					glTranslatef(2*i, 0, j*2);
					glEnable(GL_TEXTURE_2D);
					drawWall(1, -1, 1, -1, 1, -1);
					glDisable(GL_TEXTURE_2D);
				glPopMatrix();
			} else {
				//Draw a floor tile
				glPushMatrix();
					glTranslatef(2*i, 0, j*2);
					glEnable(GL_TEXTURE_2D);
					drawFloor(1, -1, 1, -1);
					glDisable(GL_TEXTURE_2D);
				glPopMatrix();

				//Add a light
				glDisable(GL_LIGHTING);
				glPushMatrix();
					// perform any transformations of light positions here
					glRotatef(0, 0, 0, 0);
					set_light(light_1, 2*i, j*2);
				glPopMatrix();
				glEnable(GL_LIGHTING);
			}
		}
	}
	glutSwapBuffers();
}

//Keyboard press handler
void pressKey(unsigned char key, int xx, int yy)
{
	switch (key) {
		//Change y to get a top down view and store current y and eye y
		//to revert back to it when key is released
		case 'z': mapMode = true;
			origTilt = tilt;
			origLy = ly;
			break;
		case 'w' :
			deltaX = 0.5f;break;
		case 's' :
			deltaX = -0.5f;break;
		case 'd' :
			deltaZ = 0.5f;break;
		case 'a' :
			deltaZ = -0.5f;break;
	}
}

//Keyboard press release handler
void releaseKey(unsigned char key, int x, int y)
{
	switch (key) {

		case 'z': mapMode = false;
			break;
		//Revert deltas to 0 when key is released
		case 'w' :
		case 's' : deltaX = 0;break;
		case 'a' :
		case 'd' : deltaZ = 0;break;

		}
}

//Passive mouse movement callback
void mouseMove(int x, int y)
{
	//turnAngle calculated for triangle blip rotation
	turnAngle = -1 * (x - xOrigin);
	if (turnAngle>0){
		turnAngle = turnAngle + 10;
	} else if (turnAngle<0)
		turnAngle = turnAngle - 10;
	else
		turnAngle = 0;

	//Change in x and y values due to mouse movement
	deltaAngle = (x - xOrigin) * 0.02f;
	deltaAngleY = (y - yOrigin) * 0.02f;

	//Calculating camera direction
	lx = sin(angle + deltaAngle);
	lz = -cos(angle + deltaAngle);
	ly =  -sin(deltaAngleY);

	//
	glutSetWindow(mainWindow);
}

//Loading textures
void load_and_bind_textures()
{
	g_wall = load_and_bind_texture("./wall.png");
	g_ground = load_and_bind_texture("./tile.png");
}

void init() {
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	set_material (materialM);
}

int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(SizeX,SizeY);
	mainWindow = glutCreateWindow("3D Maze");

	//Callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);


	//Keyboard callbacks
	glutKeyboardFunc(pressKey);
	glutIgnoreKeyRepeat(1);
	glutKeyboardUpFunc(releaseKey);

	//Mouse Callbacks
	glutPassiveMotionFunc(mouseMove);
	glutWarpPointer(SizeX/2,SizeY/2);

	glEnable(GL_DEPTH_TEST);

	int max_texture_units = 0;
    	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);
    	fprintf(stderr, "Max texture units is %d\n", max_texture_units);

	load_and_bind_textures();
	init();
	g_bitmap_text_handle = make_bitmap_text();
	createGLUTMenus ();
	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}

