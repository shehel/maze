#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H

float ipdt(const float* v1, const float* v2)
{
	return v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2];
}

void normalize(float* v)
{
	float len = ipdt(v, v);
	if (len>0)
	{
		v[0] /= sqrt(len); v[1] /= sqrt(len); v[2] /= sqrt(len);
	}

}

void cross_product(const float* u, const float* v, float* s)
{
    s[0] = (u[1] * v[2]) - (u[2] * v[1]);
    s[1] = (u[2] * v[0]) - (u[0] * v[2]);
    s[2] = (u[0] * v[1]) - (u[1] * v[0]);
}

#endif
