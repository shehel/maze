
uniform int v_ticks;

void main(void)
{
	// transform the vertex using the modelview and projection matrix
	gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;

	gl_FrontColor = gl_Color;

	// push the normal displacement into the back colour
	vec3 normal = gl_NormalMatrix * gl_Normal;
	float scale = float(v_ticks)/2000.0;
	gl_FrontSecondaryColor = vec4(scale * normal, 0.0);
}
