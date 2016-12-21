#version 120

void main()
{	
	gl_Position = ftransform();		
	
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

/*
	1 - u is the unit vector that goes from the camera to the current vertex: it is the position of the vertex in eye space and it is also the view vector.
	2 - n is the vertex normal in eye space.
	3 - r is the reflected vision vector against the normal n:
		r = reflect(u, n) 
		r = 2 * ( n dot u) * n - u
	4 - m is an intermediate value:
		m = sqrt( r.x^2 + r.y^2 + (r.z + 1.0)^2 )
	5 - s and t are the final texture coordinates:
		s = r.x / m + 0.5 
		t = r.y / m + 0.5
*/
	vec3 u = normalize(vec3(gl_ModelViewMatrix * gl_Vertex));
	vec3 n = normalize(gl_NormalMatrix * gl_Normal);
	vec3 r = reflect(u, n);
	float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );
	gl_TexCoord[1].s = r.x/m + 0.5;
	gl_TexCoord[1].t = r.y/m + 0.5;
}
