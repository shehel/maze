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

bool g_spinning = false;
int g_angle = 0;
//float eyex = 1;
//float near = -3.0f;

float camera_x = 0.0f;
float camera_y = 0.0f;
float camera_z = 0.0f;
float pitch = 0.0f; // The rotation along the x axis
float yaw = 0.0f; // The rotation along the y axis
float roll = 0.0f;

unsigned int g_program_obj = 0;
unsigned int g_vertex_obj = 0;
unsigned int g_fragment_obj = 0;

int g_ticks = 0; // keeps couting
int v_ticks_loc = 0; // location of ticker in vertex shader
int v_toggle_loc = 0; // location of action toggle flag

void ground(float x1, float x2, float y1, float z1, float z2){
	glBegin(GL_QUADS);

	glColor3f(0.0, 0.0, 0.2);
	glVertex3f(x2, y1, z1);
	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y1, z2);
	glVertex3f(x2, y1, z2);

	glEnd();
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


void idle()
{
	bool post = false;

    	usleep(10000); // in microseconds

	// restrict to range 0 to 999
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

void keyboard(unsigned char key, int, int)
{

	//std::cerr << "\t you pressed the " << key << " key" << std::endl;
	//std::cerr << "\t x coordinate: " << x << " y coordinate: " << y << std::endl;

	switch (key)
	{
		//case 's': eyey = eyey - 0.10f; break;

		case 'a': eyex = eyex - 0.10f; break;

		case 'd': eyex = eyex + 0.10f; break;

		// clockwise rotate
		//case 'w': eyey = eyey + 0.10f; break;

		case 'w': near = near - 0.10f; break;

		case 's': near = near + 0.10f; break;
	}

	glutPostRedisplay(); // force a redraw
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();



	//TODO Player help when stuck by moving camera?
	/* position and orient camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1, 1, 3, // eye position
			  eyex, near, 0, // reference point
			  0, 1, 0  // up vector
		);
	*/

	//glRotatef(g_angle, 0, 1, 0);
	glLoadIdentity();
	glRotatef(pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(yaw, 0.0f, 1.0f, 0.0f);
	glRotatef(roll, 0.0f, 0.0f, 1.0f);
	glTranslatef(-x, -y, -z);


	if (glIsProgram(g_program_obj))
		glUniform1i(v_toggle_loc, true); // turn on vertex rotation

	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
      // Top face (y = 1.0f)
      // Define vertices in counter-clockwise (CCW) order with normal pointing out
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f( 1.0f, 1.0f, -1.0f);
      glVertex3f(-1.0f, 1.0f, -1.0f);
      glVertex3f(-1.0f, 1.0f,  1.0f);
      glVertex3f( 1.0f, 1.0f,  1.0f);

      // Bottom face (y = -1.0f)
      glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      glVertex3f( 1.0f, -1.0f,  1.0f);
      glVertex3f(-1.0f, -1.0f,  1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f( 1.0f, -1.0f, -1.0f);

      // Front face  (z = 1.0f)
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f( 1.0f,  1.0f, 1.0f);
      glVertex3f(-1.0f,  1.0f, 1.0f);
      glVertex3f(-1.0f, -1.0f, 1.0f);
      glVertex3f( 1.0f, -1.0f, 1.0f);

      // Back face (z = -1.0f)
      glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
      glVertex3f( 1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f,  1.0f, -1.0f);
      glVertex3f( 1.0f,  1.0f, -1.0f);

      // Left face (x = -1.0f)
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(-1.0f,  1.0f,  1.0f);
      glVertex3f(-1.0f,  1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f,  1.0f);

      // Right face (x = 1.0f)
      glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
      glVertex3f(1.0f,  1.0f, -1.0f);
      glVertex3f(1.0f,  1.0f,  1.0f);
      glVertex3f(1.0f, -1.0f,  1.0f);
      glVertex3f(1.0f, -1.0f, -1.0f);
   glEnd();  // End of drawing color-cube

	if(glIsProgram(g_program_obj))
		glUniform1i(v_toggle_loc, false); // turn off vertex rotation

	//glColor3f(0.8f, 0.8f, 0.8f);
	//glutWireCube(1.0f);

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



char* read_shader_source(const char* filename)
{
	char* buffer = NULL;

	FILE* fp = fopen(filename, "r");
	if (fp!=NULL)
	{
		struct stat status_buf;
		stat(filename, &status_buf); // find out how big it is

		buffer = new char[status_buf.st_size+1];
		// read in the file
		fread(buffer, 1, status_buf.st_size, fp);
		buffer[status_buf.st_size] = '\0'; // null terminate it

		fclose(fp);
	}
	else
	{
		fprintf(stderr, "Failed to open shader file %s for reading\n", filename);
		exit(1);
	}

	return buffer;
}

void print_shader_info_log(unsigned int shader_obj)
{
	int len = 0;
	glGetShaderiv(shader_obj, GL_INFO_LOG_LENGTH, &len);
	if (len>1)
	{
		char* log = new char[len];
		int s_len = 0;
		glGetShaderInfoLog(shader_obj, len, &s_len, log);
		fprintf(stderr, "%s", log);
		delete[] log;
	}
}

void print_program_info_log(unsigned int shader_obj)
{
	int len = 0;
	glGetProgramiv(shader_obj, GL_INFO_LOG_LENGTH, &len);
	if (len>1)
	{
		char* log = new char[len];
		int s_len = 0;
		glGetProgramInfoLog(shader_obj, len, &s_len, log);
		fprintf(stderr, "%s", log);
		delete[] log;
	}
}

void create_and_compile_shaders(
		const char* vertex_shader_filename,
		const char* fragment_shader_filename
	)
{
	fprintf(stderr, "Shading Language version %s\n",
			glGetString(GL_SHADING_LANGUAGE_VERSION));

	// read the shader source files
	char* vertex_source = read_shader_source(vertex_shader_filename);
	char* frag_source = read_shader_source(fragment_shader_filename);

	if (vertex_source && frag_source)
	{
		// create shader program object and shader objects
		g_vertex_obj = glCreateShader(GL_VERTEX_SHADER);
		g_fragment_obj = glCreateShader(GL_FRAGMENT_SHADER);


		// put sources into shader objects
		glShaderSource(g_vertex_obj, 1, (const char**)&vertex_source, NULL);
		glShaderSource(g_fragment_obj, 1, (const char**)&frag_source, NULL);

		// attempt to compile and link
		glCompileShader(g_vertex_obj);

		// check if it has compile ok
		int compiled = 0;
		glGetShaderiv(g_vertex_obj, GL_COMPILE_STATUS, &compiled);
		if (compiled==0)
		{
			// failed to compile vertex shader
			fprintf(stderr, "Failed to compile vertex shader\n");
			print_shader_info_log(g_vertex_obj);

			exit(1);
		}

		glCompileShader(g_fragment_obj);
		glGetShaderiv(g_fragment_obj, GL_COMPILE_STATUS, &compiled);
		if (compiled==0)
		{
			// failed to compile fragment shader
			fprintf(stderr, "Failed to compile fragment shader\n");
			print_shader_info_log(g_fragment_obj);
			exit(1);
		}

		// attach shaders to the program object
		g_program_obj = glCreateProgram();
		glAttachShader(g_program_obj, g_vertex_obj);
		glAttachShader(g_program_obj, g_fragment_obj);

		// try to link the program
		glLinkProgram(g_program_obj);

		int linked = 0;
		glGetProgramiv(g_program_obj, GL_LINK_STATUS, &linked);
		if (linked==0)
		{
			// failed to link program
			fprintf(stderr, "Failed to link shader program\n");
			print_program_info_log(g_program_obj);
			exit(1);
		}

		delete[] vertex_source;
		delete[] frag_source;
	}

	if (glIsProgram(g_program_obj))
		glUseProgram(g_program_obj);
}

void init(int argc, char* argv[])
{
	if (argc>2)
	{
		create_and_compile_shaders(argv[1], argv[2]);

		// get the location of the ticks variable
		v_ticks_loc = glGetUniformLocation(g_program_obj, "v_ticks");

		// get the location of an action toggle
		v_toggle_loc = glGetUniformLocation(g_program_obj, "v_toggle");

	}

	glMatrixMode (GL_PROJECTION);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity ();

	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);

	glutInitWindowSize(512, 512);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Uniform Test");

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
