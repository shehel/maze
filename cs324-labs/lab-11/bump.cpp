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

#include "load_and_bind_texture.h"
#include "create_and_compile_shaders.h"
#include "lights_material.h"

bool g_spinning = false;
int g_angle = 0;

unsigned int g_program_obj = 0;
unsigned int g_vertex_obj = 0;
unsigned int g_fragment_obj = 0;

int g_ticks = 0; // keeps couting

unsigned int g_ceramic = 0; // texture handle
unsigned int g_bump_map = 0; // texture handle
int f_tex0_loc = 0; // location of texture in fragment shader
int f_tex1_loc = 0; // location of texture in fragment shader

float g_cube_vertices[8][3] = 
{
	{-0.5f, -0.5f, -0.5f}, // front
	{0.5f, -0.5f, -0.5f},
	{0.5f, 0.5f, -0.5f},
	{-0.5f, 0.5f, -0.5f},
	{-0.5f, -0.5f, 0.5f}, // back 
	{0.5f, -0.5f, 0.5f},
	{0.5f, 0.5f, 0.5f},
	{-0.5f, 0.5f, 0.5f}
};

// indices into verices
size_t g_cube_faces[6][4] =
{
	{0, 1, 2, 3},  // front
	{5, 4, 7, 6},  // back
	{4, 0, 3, 7},  // left
	{1, 5, 6, 2},  // right
	{4, 5, 1, 0},  // bottom
	{3, 2, 6, 7}   // top
};

void idle()
{
	bool post = false;

    usleep(10000); // in microseconds
	
	g_ticks++;

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
	glRotatef(-60, 1, 0, 0);

	// if shader program is installed
	if (glIsProgram(g_program_obj))
	{
		glUseProgram(g_program_obj);

		// send the texture unit ID to fragment shader
		// it should be 0 for GL_TEXTURE0
		// it should be 1 for GL_TEXTURE1
		// it should be 2 for GL_TEXTURE2 and so on
		glUniform1i(f_tex0_loc, 0); 
		glUniform1i(f_tex1_loc, 1); 
	}
	else
		glEnable(GL_TEXTURE_2D); // not needed with shader

   	glActiveTexture(GL_TEXTURE0);
   	glBindTexture(GL_TEXTURE_2D, g_ceramic);

   	glActiveTexture(GL_TEXTURE1);
   	glBindTexture(GL_TEXTURE_2D, g_bump_map);

	glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate
	glEnable(GL_TEXTURE_GEN_T); //enable texture coordinate

           //glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
           //glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

    		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glutSolidTeapot(0.7);
	//glutSolidTorus(0.35, 0.7, 32, 32);

   	glActiveTexture(GL_TEXTURE2);


	if (glIsProgram(g_program_obj))
		glUseProgram(0);
	else
		glDisable(GL_TEXTURE_2D); // not needed with shader

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


struct materials_t
{
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float shininess;
};

const materials_t g_red_plastic = {
    {0.3f, 0.0f, 0.0f, 1.0f},
    {0.6f, 0.0f, 0.0f, 1.0f},
    {0.8f, 0.6f, 0.6f, 1.0f},
    32.0f
};

// properties of a given material
void set_material(const materials_t& mat)
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat.specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat.shininess);
}

void init(int argc, char* argv[])
{
    g_ceramic = load_and_bind_texture("../images/tile.png");
    //g_bump_map = load_and_bind_texture("../images/normal.png");
    g_bump_map = load_and_bind_texture("../images/chesterfield.png");

    int max_texture_units = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);
    fprintf(stderr, "Max texture units is %d\n", max_texture_units);

	if (argc>2)
	{
		g_program_obj = create_and_compile_shaders(argv[1], NULL, argv[2]);

		// get the location of the fragment shader texture variable
		f_tex0_loc = glGetUniformLocation(g_program_obj, "f_tex0");
		f_tex1_loc = glGetUniformLocation(g_program_obj, "f_tex1");
	}

	init_lights();

	set_material(g_red_plastic);
	
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH); 

	glutInitWindowSize(512, 512); 
	glutInitWindowPosition(50, 50); 

	glutCreateWindow("Bump Mapping"); 
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
