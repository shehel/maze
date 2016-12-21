#ifndef CREATE_AND_COMPILE_SHADERS_H
#define CREATE_AND_COMPILE_SHADERS_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

char* read_shader_source(const char* filename)
{
	char* buffer = NULL;

	if (filename==NULL)
		return buffer;

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

void set_geometry_shader_params(
			unsigned int program_obj,
			unsigned int in_type,
			unsigned int out_type,
			int max_vertices)
{
	// maximum vertices in glBegin() glEnd() bracketing
	glProgramParameteriEXT(program_obj, GL_GEOMETRY_VERTICES_OUT_EXT, max_vertices);

	// specify the type of geometry we are going to use in GS
	// unfortunately this has to be done *prior* to linking the geoemtry shader
	// in GL 1.2
	// can be GL_TRIANGLES -> GL_TRIANGLE_STRIP
	// can be GL_LINES -> GL_LINE_STRIP
	// can be GL_POINTS -> GL_POINTS
	glProgramParameteriEXT(program_obj, GL_GEOMETRY_INPUT_TYPE_EXT, in_type);
	glProgramParameteriEXT(program_obj, GL_GEOMETRY_OUTPUT_TYPE_EXT, out_type);

	// re-link the program
	glLinkProgram(program_obj);

	int linked = 0;
	glGetProgramiv(program_obj, GL_LINK_STATUS, &linked);
	if (linked==0)
	{
		// failed to link program 
		fprintf(stderr, "Failed to link shader program\n");
		print_program_info_log(program_obj);
		exit(1);
	}
}


unsigned int create_and_compile_shaders(
		const char* vertex_shader_filename,
		const char* geometry_shader_filename,
		const char* fragment_shader_filename
	)
{
	unsigned int program_obj = 0;

	fprintf(stderr, "Shading Language version %s\n", 
			glGetString(GL_SHADING_LANGUAGE_VERSION));

	// read the shader source files
	char* vertex_source = read_shader_source(vertex_shader_filename);
	char* geometry_source = read_shader_source(geometry_shader_filename);
	char* frag_source = read_shader_source(fragment_shader_filename);

	if (vertex_source && frag_source)
	{
		// create shader program object and shader objects
		unsigned int vertex_obj = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragment_obj = glCreateShader(GL_FRAGMENT_SHADER);

		unsigned int geometry_obj = 0;

		// put sources into shader objects
		glShaderSource(vertex_obj, 1, (const char**)&vertex_source, NULL);
		glShaderSource(fragment_obj, 1, (const char**)&frag_source, NULL);

		// optional
		if (geometry_source)
		{
			// note the type has "EXT" in GLSL 1.2
			geometry_obj = glCreateShader(GL_GEOMETRY_SHADER_EXT);
			glShaderSource(geometry_obj, 1, (const char**)&geometry_source, NULL);
		}
		
		// attempt to compile and link
		glCompileShader(vertex_obj);

		// check if it has compile ok
		int compiled = 0;
		glGetShaderiv(vertex_obj, GL_COMPILE_STATUS, &compiled);
		if (compiled==0)
		{
			// failed to compile vertex shader
			fprintf(stderr, "Failed to compile vertex shader\n");
			print_shader_info_log(vertex_obj);

			exit(1);
		}

		glCompileShader(fragment_obj);
		glGetShaderiv(fragment_obj, GL_COMPILE_STATUS, &compiled);
		if (compiled==0)
		{
			// failed to compile fragment shader
			fprintf(stderr, "Failed to compile fragment shader\n");
			print_shader_info_log(fragment_obj);
			exit(1);
		}

		// optional
		if (geometry_source)
		{
			glCompileShader(geometry_obj);
			glGetShaderiv(geometry_obj, GL_COMPILE_STATUS, &compiled);
			if (compiled==0)
			{
				// failed to compile geometry shader
				fprintf(stderr, "Failed to compile geometry shader\n");
				print_shader_info_log(geometry_obj);
				exit(1);
			}

		}

		// attach shaders to the program object
		program_obj = glCreateProgram();
		glAttachShader(program_obj, vertex_obj);
		glAttachShader(program_obj, fragment_obj);

		// optional
		if (geometry_obj)
		{
			glAttachShader(program_obj, geometry_obj);

/*
			// find out what is maxium GL will output
			int max_vertices = 0;
			// might be inefficient to do this so should reduce to
			// maximum actuall used
			glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &max_vertices);
			fprintf(stderr, "\t maximum vertices to geometry shader is %d\n", 
					max_vertices);
*/
			const int max_vertices = 3;
			// specify the max vertices out to GPU
			glProgramParameteriEXT(program_obj, GL_GEOMETRY_VERTICES_OUT_EXT, max_vertices);

			// specify the type of geometry we are going to use in GS
			// unfortunately this has to be done *prior* to linking the geoemtry shader
			// in GL 1.2
			// can be GL_TRIANGLES -> GL_TRIANGLE_STRIP
			// can be GL_LINES -> GL_LINE_STRIP
			// can be GL_POINTS -> GL_POINTS
    		glProgramParameteriEXT(program_obj, GL_GEOMETRY_INPUT_TYPE_EXT, GL_TRIANGLES);
    		glProgramParameteriEXT(program_obj, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLE_STRIP);
		}

		// try to link the program
		glLinkProgram(program_obj);

		int linked = 0;
		glGetProgramiv(program_obj, GL_LINK_STATUS, &linked);
		if (linked==0)
		{
			// failed to link program 
			fprintf(stderr, "Failed to link shader program\n");
			print_program_info_log(program_obj);
			exit(1);
		}

		delete[] vertex_source;
		delete[] frag_source;
	}

	if (glIsProgram(program_obj))
		glUseProgram(program_obj);

	return program_obj;
}

#endif

