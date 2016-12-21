
varying vec3 normal;
varying vec3 light_dir;
varying vec3 eye_vec;

void main()
{	
	gl_Position = ftransform();		
	
	vec3 view_vertex = vec3(gl_ModelViewMatrix * gl_Vertex);

	light_dir = vec3(gl_LightSource[0].position.xyz - view_vertex);
	eye_vec = -view_vertex;
	
	// base texture
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

	// for specular (environment) lighting
	vec3 u = normalize(view_vertex);
	normal = normalize(gl_NormalMatrix * gl_Normal);
	vec3 r = reflect(u, normal);
	float m = 2.0 * sqrt(r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0));
	gl_TexCoord[1].s = r.x/m + 0.5;
	gl_TexCoord[1].t = r.y/m + 0.5;
}
