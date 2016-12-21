#ifndef CAPPED_CYLINDER_H
#define CAPPED_CYLINDER

// this uses gluQuadrics to make a capped cylinder with a given number
//    of slices and stacks
void capped_cylinder(
			const float radius,
			const float height,
			const int slices=32,
			const int stacks=32)
{
	GLUquadricObj* q_obj = gluNewQuadric();

	gluQuadricNormals(q_obj, GLU_SMOOTH);

	// draw the sides
	gluCylinder(q_obj, radius, radius, height, slices, stacks);

	// draw the top cap
	glPushMatrix();
		glTranslated(0.0, 0.0, height);
		gluDisk(q_obj, 0.0, radius, slices, stacks);
	glPopMatrix();

	// draw the bottom cap
	glPushMatrix();
		glRotated(180.0, 1.0, 0.0, 0.0);
		gluDisk(q_obj, 0.0, radius, slices, stacks);
	glPopMatrix();
}

#endif
