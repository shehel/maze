#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <iostream>

#include <time.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

#include <vector>
#include <cmath>

#include "vector-math.h"

// particle information as struct
struct particle_t
{
	unsigned int tex; // texture handle
	float colour[4]; // colour and transparency
	float position[3]; // world coordinate
	float direction[3]; // direction of travel
	float size; // in world dimensions
};

std::vector<particle_t> g_particles; // a number of particles

// properties of some material
float mat_ambient[] = {0.05, 0.05, 0.05, 1.0};
float mat_diffuse[] = {0.75, 0.75, 0.75, 1.0};
float mat_specular[] = {1.0, 1.0, 1.0, 1.0};
float mat_shininess[] = {50.0};

int g_spin = 0;
bool g_moving = true;
bool g_spinning = false;

void idle()
{
	usleep(100000);

	// probably should decouple this from the particle motions
	if (g_spinning)
		g_spin += 5;

	if (g_moving)
	{
		// move all the particles -- bounce them against the sides
		for (size_t i=0;i<g_particles.size();i++)
		for (size_t j=0;j<3;j++) // for each dimension
		{
			g_particles[i].position[j] += g_particles[i].direction[j];

			// if out of bounds then reverse direction 
			if (g_particles[i].position[j]>0.5f)
			{
				g_particles[i].position[j] = 0.5f;
				g_particles[i].direction[j] = -g_particles[i].direction[j];
			}
			else
			if (g_particles[i].position[j]<-0.5f)
			{
				g_particles[i].position[j] = -0.5f;
				g_particles[i].direction[j] = -g_particles[i].direction[j];
			}
		}
	}

	glutPostRedisplay();
}

// some interesting vector algebra to turn a plane toward the eye point
void rotate_towards_viewer(
			const float* pos, // plane centre
			const float* normal, // plane normal
			const float* eye)
{
	float v[3] = {eye[0]-pos[0], eye[1]-pos[1], eye[2]-pos[2]};

	// normalize v
	normalize(v);

	// find axis of rotation
	float axis[3] = {0, 1, 0};
	cross_product(normal, v, axis);

	// roation towards eye in axis perpendiculat o 
	// plane in which vectors normal and eye lie
	const float rad_to_deg = 180.0f/(atan(1)*4);
	const float angle = rad_to_deg * acos(ipdt(v, normal));
	glRotatef(angle, axis[0], axis[1], axis[2]);
}

void display()
{
	static const float eye[3] = {1, 1, 3};
	static const float z_axis[3] = {0, 0, -1};

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
	
	// position and orient camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();	
		gluLookAt(
			  eye[0], eye[1], eye[2],
			  0, 0, 0, // reference point
			  0, 1, 0  // up vector
		);

		glRotatef(g_spin, 0, 1, 0);

		// set the surface properties
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

		// show opaque geometry
		glutSolidTeapot(0.3);

		glDisable(GL_LIGHTING);
			glColor3f(1.0f, 1.0f, 1.0f);
			glutWireCube(1.0);
		glEnable(GL_LIGHTING);

		glDepthMask(GL_FALSE);
		
    	// enable texturing
		glDisable(GL_LIGHTING);
    	glEnable(GL_TEXTURE_2D);

		// these textures are billboarded -- turned to face the viewer
		// select which texure to render
		// draw textures in front of solid geometry
		for (size_t i=0;i<g_particles.size();i++)
		{
		//	glBindTexture(GL_TEXTURE_2D, g_particles[i].tex);
			glColor4fv(g_particles[i].colour);
			glPushMatrix(); 
				glRotatef(-g_spin, 0, 1, 0);

				// position the texture
				glTranslatef(
						g_particles[i].position[0],
						g_particles[i].position[1],
						g_particles[i].position[2]);

				// turn it to face the viewer
				rotate_towards_viewer(g_particles[i].position, z_axis, eye);

				float size = g_particles[i].size;
/*
				glBegin (GL_LINE_LOOP);
					glVertex3f(-size, -size, 0);
					glVertex3f(size, -size, 0);
					glVertex3f(size, size, 0);
					glVertex3f(-size, size, 0);
				glEnd();
*/
				// post the texture
				glBegin (GL_QUADS);
					glTexCoord2f(0, 0); glVertex3f(-size, -size, 0.0f);
					glTexCoord2f(1, 0); glVertex3f(size, -size, 0.0f);
					glTexCoord2f(1, 1); glVertex3f(size, size, 0.0f);
					glTexCoord2f(0, 1); glVertex3f(-size, size, 0.0f);
				glEnd ();
			glPopMatrix();
		}
    	glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
	
		glDepthMask(GL_TRUE);

	glPopMatrix();	


	glutSwapBuffers(); 
}

void keyboard(unsigned char key, int, int)
{
	switch (key)
	{
		case 'q': exit(1); break;

		case ' ': g_spinning = !g_spinning; break;
		case 'm': g_moving = !g_moving; break;
	}
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluPerspective(40.0, 1.0f, 1.0, 5.0);

	glutPostRedisplay();
}

// a gaussian blob texture
unsigned char* blob_image(size_t size)
{
	unsigned char* im = new unsigned char[4*size*size];
	const float sd2 = (float(size)/3.0f)*(float(size)/3.0f);

    //unsigned char m = (unsigned char)(size/8); // mask for checker board
	size_t p = 0;
	for (size_t i=0;i<size;i++)
	for (size_t j=0;j<size;j++,p+=4)
	{
		const float x = float(j) - float(size/2);
		const float y = float(i) - float(size/2);

		const float g = exp(-2*(x*x+y*y)/sd2);
		//const float g = float(((i&m)==0)^((j&m)==0)); // checker board
		im[p+0] = 0;
		im[p+1] = 0;
		im[p+2] = 0;
		im[p+3] = (unsigned char)(255 * g); // alpha value
	}

	return im;
}

void make_particles(const size_t n)
{
	const size_t size = 64;
	unsigned char* im = blob_image(size);

	const float env_colour[4] = {0.0f, 0.0f, 0.0f, 1.0f};

	srand48(time(NULL));

	for (size_t i=0;i<n;i++)
	{
		particle_t p;

		p.size = 0.15f; // we could make them different sizes

		float rand_r = float(drand48());
		float rand_g = float(drand48());
		float rand_b = float(drand48());
		p.colour[0] = rand_r;
		p.colour[1] = rand_g;
		p.colour[2] = rand_b;
		p.colour[3] = 0.75f;

		float rand_x = float(drand48()) - 0.5f;
		float rand_y = float(drand48()) - 0.5f;
		float rand_z = float(drand48()) - 0.5f;
		p.position[0] = rand_x;
		p.position[1] = rand_y;
		p.position[2] = rand_z;

		// random direction of movement
		float rand_dx = (float(drand48()) - 0.5f)/20;
		float rand_dy = (float(drand48()) - 0.5f)/20;
		float rand_dz = (float(drand48()) - 0.5f)/20;

		// debug on the initial particle state
		//printf("\tparticle %d: dx = %f, dy = %f, dz = %f\n", int(i), rand_dx, rand_dy, rand_dz);
		p.direction[0] = rand_dx;
		p.direction[1] = rand_dy;
		p.direction[2] = rand_dz;

		glGenTextures(1, &(p.tex));
		glBindTexture(GL_TEXTURE_2D, p.tex);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, env_colour);

		glTexImage2D(GL_TEXTURE_2D, 0,
					GL_RGBA8, size, size, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, (void*)im);

		g_particles.push_back(p);
	}

	delete[] im; // free up the image
}

void init()
{
	glClearColor(0.1f, 0.1f, 0.44f, 1.0f); // midnight blue

	float light_ambient[] = {0.1, 0.1, 0.1, 1.0};
	float light_diffuse[] = {0.5, 0.5, 0.5, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	float light_position[] = {1.0, 1.0, 2.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH); 

	// turn on blending and set a blending function
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	make_particles(8);
	
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH); 

	glutInitWindowSize(640, 640); 
	glutInitWindowPosition(50,50); 

	glutCreateWindow("Particles"); 

	glutKeyboardFunc(keyboard); 
	glutReshapeFunc(reshape); 
	glutDisplayFunc(display); 
	glutIdleFunc(idle);

	init(); 

	glutMainLoop(); 

	return 0; 
}
