#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glew.h> 
#include <GL/glut.h> 
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

#include "draw_text.h"
#include "lights_material.h"
#include "create_and_compile_shaders.h"

bool g_spinning = false;
int g_angle = 0;

unsigned int g_program_obj = 0;

float g_light0_position[] = {1.0f, 1.0f, 2.0f, 0.0f};

void idle()
{
	bool post = false;

    usleep(10000); // in microseconds
	
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

	// move light 0 position
    glLightfv(GL_LIGHT0, GL_POSITION, g_light0_position);

	// move the object
	glRotatef(g_angle, 0, 1, 0);



	glutSolidTeapot(0.6f);


	// disable shader while we are writing text
    glUseProgram(0);

	glPointSize(1.0f);
    glDisable(GL_LIGHTING);
        // put some help on the screen
        draw_text(20, 960, "Use 'x'/'X' to move light in X");
        draw_text(20, 920, "Use 'y'/'Y' to move light in Y");
        draw_text(20, 880, "Use 'q' to quit");
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

		case '0': 
				g_light0_position[0] = 1.0f; 
				g_light0_position[1] = 1.0f;  break;

		case 'x': g_light0_position[0] += 0.1f; break;
		case 'X': g_light0_position[0] -= 0.1f; break;
		case 'y': g_light0_position[1] += 0.1f; break;
		case 'Y': g_light0_position[1] -= 0.1f; break;
	}

	glutPostRedisplay();
}

void init(int argc, char* argv[])
{
	if (argc==3)
		g_program_obj = create_and_compile_shaders(argv[1], NULL, argv[2]);
	else
	if (argc==4)
		g_program_obj = create_and_compile_shaders(argv[1], argv[2], argv[3]);

	// re-link geometry shader if change the primitive assembly type
	if (g_program_obj)
	{
		// use GL_POINTS, GL_POINTS, n for sets of points
		//set_geometry_shader_params(g_program_obj, GL_POINTS, GL_POINTS, 64);

		// use GL_TRIANGLES, GL_TRIANGLE_STRIP, 3 for solid geometry with triangles
		//set_geometry_shader_params(g_program_obj, GL_TRIANGLES, GL_TRIANGLE_STRIP, 3);

		// use GL_LINES, GL_LINES_STRIP, 4 for wire frame stuff done with quads
		set_geometry_shader_params(g_program_obj, GL_LINES, GL_LINE_STRIP, 4);
	}

	init_lights(GL_SMOOTH);

	init_material();

	glClearColor(0.1f, 0.1f, 0.44f, 1.0f);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 

	glutInitDisplayMode(
			// GLUT_3_2_CORE_PROFILE| // this will give you GLSL 1.5 on -- be careful what you wish for
			GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH); 

	glutInitWindowSize(512, 512); 
	glutInitWindowPosition(50, 50); 

	glutCreateWindow("Phong Lighting with Shaders"); 

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

	fprintf(stderr, "Open GL Engine = %s\nVersion =  %s\n", 
				glGetString(GL_RENDERER), 
				glGetString(GL_VERSION)
			);

	init(argc, argv); 


	glutMainLoop(); 

	return 0; 
}
