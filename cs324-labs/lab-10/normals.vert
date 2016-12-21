
void main(void)
{
	// transform the vertex using the modelview and projection matrix
	gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;

	// pass colour 
	gl_FrontColor = gl_Color;

	// pass normal in eye coordinates to geometry shader in the secondary colour atrribute
	vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
	gl_FrontSecondaryColor = vec4(normal, 1.0);
}
