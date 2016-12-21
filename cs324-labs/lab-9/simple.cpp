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

bool g_spinning = false;
int g_angle = 0;

unsigned int g_program_obj = 0;
unsigned int g_vertex_obj = 0;
unsigned int g_fragment_obj = 0;

void idle()
{
    usleep(10000); // in microseconds

	if (g_spinning)
	{
    	g_angle += 1;
    	glutPostRedisplay();
	}
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

	glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.25f, -0.25f, 0.25f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.25f, -0.25f, 0.25f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.25f, -0.25f);
	glEnd();

	glColor3f(0.8f, 0.8f, 0.8f);
	glutWireCube(1.0f);

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
	fprintf(stderr, "create_and_compile shaders called: vertex shader = %s, fragment shader = %s\n",
						vertex_shader_filename, fragment_shader_filename );
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
		create_and_compile_shaders(argv[1], argv[2]);

	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH); 

	glutInitWindowSize(512, 512); 
	glutInitWindowPosition(50, 50); 

	glutCreateWindow("Simple Shader"); 

#ifndef __APPLE__ 
	GLenum err = glewInit();
	if (GLEW_OK!=err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(1);
	}

	fprintf(stderr, "Using GLEW %s\n", glewGetString(GLEW_VERSION));


	if (glewIsSupported("GL_VERSION_2_0"))
		printf("Ready for OpenGL 2.0\n");
	else 
	{
		printf("OpenGL 2.0 not supported\n");
		exit(1);
	}
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
