
#ifndef LIGHTS_MATERIAL_H
#define LIGHTS_MATERIAL_H

// some magic to set up lights and smooth shading
void init_lights(const GLenum shade_model=GL_FLAT)
{
	float light0_position[] = {1.0, 1.0, 2.0, 0.0};
	float light1_position[] = {-2.0, 0.0, 2.0, 0.0};
	float light_ambient[] = {0.1, 0.1, 0.1, 1.0};
	float light_diffuse[] = {0.5, 0.5, 0.5, 1.0};
	float light_specular[] = {0.8, 0.8, 0.8, 1.0};

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);

	glFrontFace(GL_CW);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);

    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);


	glShadeModel(shade_model); // GL_FLAT, GL_SMOOTH

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
}

void init_material()
{
	float mat_ambient[] = {0.05, 0.05, 0.05, 1.0};
	float mat_diffuse[] = {0.85, 0.85, 0.85, 1.0};
	float mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	float mat_shininess[] = {50.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

#endif
