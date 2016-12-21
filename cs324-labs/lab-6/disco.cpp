#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <iostream>

#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

// properties struct
struct materials_t 
{
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float shininess;
};

// light struct
struct light_t 
{
	size_t name;
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float position[4];
};

// predefined materils to use
const materials_t brass = {
	{0.33f, 0.22f, 0.03f, 1.0f},
	{0.78f, 0.57f, 0.11f, 1.0f},
	{0.99f, 0.91f, 0.81f, 1.0f},
	27.8f
};

const materials_t red_plastic = {
	{0.3f, 0.0f, 0.0f, 1.0f},
	{0.6f, 0.0f, 0.0f, 1.0f},
	{0.8f, 0.6f, 0.6f, 1.0f},
	32.0f
};

const materials_t white_shiny = {
	{0.1f, 0.1f, 0.1f, 1.0f},
	{0.5f, 0.5f, 0.5f, 1.0f},
	{0.5f, 0.5f, 0.5f, 1.0f},
	100.0f
};

// void some predfined lights
light_t light_0 = {
	GL_LIGHT0, 
	{0.0f, 0.0f, 0.0f, 1.0f},
	{1.0f, 0.0f, 0.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
	{0.0f, 0.75f, 0.5f, 1.0f}
};

light_t light_1 = {
	GL_LIGHT1, 
	{0.0f, 0.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},
	{0.5f, 0.75f, -0.5f, 1.0f}
};

light_t light_2 = {
	GL_LIGHT2, 
	{0.0f, 0.0f, 0.0f, 1.0f},
	{0.0f, 1.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f},
	{-0.5f, 0.75f, -0.5f, 1.0f}
};

// state variable for rotation of light gantry
bool g_spinning = false;
int g_angle = 0; 

// properties of a given material
void set_material(const materials_t& mat)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat.ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat.diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat.specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat.shininess);
}

// set a given light
void set_light(const light_t& light)
{
	glLightfv(light.name, GL_AMBIENT, light.ambient);
	glLightfv(light.name, GL_DIFFUSE, light.diffuse);
	glLightfv(light.name, GL_SPECULAR, light.specular);
	glLightfv(light.name, GL_POSITION, light.position);

/*
	// make the lights spot lights here if you want
	float direction[3] = {
				-light.position[0],
				-light.position[1],
				-light.position[2]};
	glLightfv(light.name, GL_SPOT_DIRECTION, direction);
	glLightf(light.name, GL_SPOT_CUTOFF, 60.0f);
*/
	glEnable(light.name);
}

/*
void idle()
{
	usleep(300000); // in microseconds

	g_angle += 3;

	glutPostRedisplay();
}
*/

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
	
	// position and orient camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 1, 3, // eye position
			  0, 0, 0, // reference point
			  0, 1, 0  // up vector
		);

	// set the surface properties
	set_material(white_shiny); // can try brass and white_shiny

	glutSolidTeapot(0.4);

	// show where the lights are applying current spin
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		
		// perform any transformations of light positions here

		glPointSize(3.0f);
		glBegin(GL_POINTS);
			glVertex3fv(light_0.position);
			glVertex3fv(light_1.position);
			glVertex3fv(light_2.position);
		glEnd();

	glPopMatrix();
	glEnable(GL_LIGHTING);
	

	glutSwapBuffers(); 
}

void keyboard(unsigned char key, int, int)
{
	switch (key)
	{
		case 'q': exit(1); break;

/* 		// code to change light spin angle
		case ' ': 
				g_spinning = !g_spinning;

				if (g_spinning)
					glutIdleFunc(idle);
				else
					glutIdleFunc(NULL);
				break;
*/
				
	}
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluPerspective(40.0, 1.0f, 1.0, 5.0);
}

void init()
{
	set_light(light_0);
	set_light(light_1);
	set_light(light_2);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH); 
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH); 

	glutInitWindowSize(512, 512); 
	glutInitWindowPosition(50, 50); 

	glutCreateWindow("D-I-S-C-O"); 

	glutKeyboardFunc(keyboard); 
	glutReshapeFunc(reshape); 
	glutDisplayFunc(display); 

	init(); 

	glutMainLoop(); 

	return 0; 
}
