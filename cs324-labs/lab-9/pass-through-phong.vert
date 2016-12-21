varying vec4 pos_eye;
varying vec3 normal_eye;

void main(void)
{
	// transform the vertex using the modelview and projection matrix
	gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;

	// vertex position in eye coordinates
	pos_eye = gl_ModelViewMatrix * gl_Vertex;

	// normal in eye space
	normal_eye = gl_NormalMatrix * gl_Normal;
}
