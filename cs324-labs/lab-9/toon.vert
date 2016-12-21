

varying vec3 light_dir;
varying vec3 normal;

void main()
{
	vec4 pos = gl_ModelViewMatrix * gl_Vertex;

	light_dir = normalize(vec3(gl_LightSource[0].position)-vec3(pos));
	normal = normalize(gl_NormalMatrix * gl_Normal);

	gl_Position = ftransform();
} 
