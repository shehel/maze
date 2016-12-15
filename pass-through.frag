#version 120

void main(void)
{
	// pass through
	//gl_FragColor = vec4(1.0,0.4,0.8,1.0); // draw every thing one colour
	gl_FragColor = gl_Color; // interpolate the received colour
}
