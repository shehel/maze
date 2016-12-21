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
#include "draw_text.h"
#include "create_and_compile_shaders.h"

bool g_spinning = false;
int g_angle = 0;
int g_angle_x = 0;
float g_width = 2.5f;

unsigned int g_program_obj = 0;
unsigned int g_vertex_obj = 0;
unsigned int g_fragment_obj = 0;

int g_ticks = 0; // keeps couting
int v_ticks_loc = 0; // location of ticker in vertex shader

void idle()
{
	bool post = false;

    usleep(10000); // in microseconds
	
	g_ticks = (g_ticks+1)%360; // like an angle

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
	glRotatef(g_angle_x, 1, 0, 0);

	glLineWidth(g_width);

	srand48(1L);

	// a field of points
	const int num_in_x = 30;
	const int num_in_z = 30;
	const float step_x = 1.5f/num_in_x;
	const float step_z = 1.5f/num_in_z;
	glBegin(GL_LINES);
		float x = -1.5f/2;
		for (int i=0;i<=num_in_z;i++,x+=step_x)
		{
			float z = -1.5f/2;
			for (int j=0;j<=num_in_x;j++,z+=step_z)
			{
				float rand_x = 2*float(drand48())-1.0f;
				float rand_z = 2*float(drand48())-1.0f;
;
				glColor4f(1, 1, 1, 0.5);
				glVertex3f(rand_x, -0.2f, rand_z);
				glNormal3f(0, 1, 0);
				glVertex3f(rand_x, -0.25f, rand_z);
				glNormal3f(0, 1, 0);
			}
		}
	glEnd();

    // disable shader while we are writing text
    glUseProgram(0);

	glLineWidth(1.0);
    glDisable(GL_LIGHTING);
        // put some help on the screen
        draw_text(20, 960, "Use ' ' to spin the model");
        draw_text(20, 920, "Use 'x'/'X' to rotate the model in X axis");
        draw_text(20, 880, "Use '+'/'-' to change line width");
        draw_text(20, 840, "Use 'q' to quit");
    glEnable(GL_LIGHTING);

    // re-enable shader
    if (glIsProgram(g_program_obj))
        glUseProgram(g_program_obj);

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
		case 'x': g_angle_x += 3; break;
		case 'X': g_angle_x -= 3; break;
		case '+': g_width += 0.5f; break;
		case '-': g_width -= 0.5f; if (g_width<0.5f) g_width = 0.5f; break;
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
        set_geometry_shader_params(g_program_obj, GL_LINES, GL_LINE_STRIP, 128);
    }

	
/*
	init_lights(GL_SMOOTH);
	init_material();
*/
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.1f, 0.1f, 0.44f, 1.0f);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH); 

	glutInitWindowSize(512, 512); 
	glutInitWindowPosition(50, 50); 

	glutCreateWindow("Grass"); 

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
