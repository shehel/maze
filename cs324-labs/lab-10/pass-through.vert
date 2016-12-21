
void main(void)
{
	// transform the vertex using the modelview and projection matrix
	gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;

	gl_FrontColor = gl_Color; // pass out the input colour to geom/frag shader
}
