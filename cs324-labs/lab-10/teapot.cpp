#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glew.h> 
#include <GL/glut.h> 
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/stat.h>
#include <unistd.h>

//#include "lights_material.h"
#include "create_and_compile_shaders.h"

bool g_spinning = false;
int g_angle = 0;

unsigned int g_program_obj = 0;
unsigned int g_vertex_obj = 0;
unsigned int g_fragment_obj = 0;

int g_ticks = 0; // keeps couting
int v_ticks_loc = 0; // location of ticker in vertex shader

void idle()
{
	bool post = false;

    usleep(2000); // in microseconds
	
	g_ticks = (g_ticks+1)%1000;

	// if shader program is installed
	if (glIsProgram(g_program_obj))
	{
		glUniform1i(v_ticks_loc, g_ticks); // send to vertex shader
		post = true;
	}

	if (g_spinning)
	{
    	g_angle += 1;
		post = true;
	}

	// only redraw if we need to
	if (post)
    	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
	
	// position and orient camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1, 1, 3, // eye position
			  0, 0, 0, // reference point
			  0, 1, 0  // up vector
		);

	glRotatef(g_angle, 0, 1, 0);

	glColor3f(0.8f, 0.8f, 0.8f);
	glutWireTeapot(0.5f);

	glutSwapBuffers(); 
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluPerspective(40.0, 1.0f, 1.0, 5.0);

	glutPostRedisplay();
}

void keyboard(unsigned char key, int, int)
{
	switch (key)
	{
		case 'q': exit(0); break;
		case ' ': g_spinning = !g_spinning; break;
	}
	glutPostRedisplay();
}

void init(int argc, char* argv[])
{
	if (argc>3)
		g_program_obj = create_and_compile_shaders(argv[1], argv[2], argv[3]);

    if (g_program_obj)
    {
		// get the location of the ticks variable
		v_ticks_loc = glGetUniformLocation(g_program_obj, "v_ticks");

    	// re-link geometry shader if change the primitive assembly type
        // use GL_POINTS, GL_POINTS, n for sets of points
        //set_geometry_shader_params(g_program_obj, GL_POINTS, GL_POINTS, 64);

        // use GL_TRIANGLES, GL_TRIANGLE_STRIP, 3 for solid geometry with triangles
        //set_geometry_shader_params(g_program_obj, GL_TRIANGLES, GL_TRIANGLE_STRIP, 3);

        // use GL_LINES, GL_LINES_STRIP, 4 for wire frame stuff done with quads
        set_geometry_shader_params(g_program_obj, GL_LINES, GL_LINE_STRIP, 4);
    }

	
/*
	init_lights(GL_SMOOTH);
	init_material();
*/
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH); 

	glutInitWindowSize(512, 512); 
	glutInitWindowPosition(50, 50); 

	glutCreateWindow("Teapot Geometry Shader"); 
#ifndef __APPLE__ 
	GLenum err = glewInit();
	if (GLEW_OK!=err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(1);
	}
	fprintf(stderr, "Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

	glutKeyboardFunc(keyboard); 
	glutReshapeFunc(reshape); 
	glutDisplayFunc(display); 
	glutIdleFunc(idle); 

	fprintf(stderr, "Open GL version %s\n", glGetString(GL_VERSION));
	init(argc, argv); 


	glutMainLoop(); 

	return 0; 
}
