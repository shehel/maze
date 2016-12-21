#version 120
#
#define PI 3.1415926535897932384626433832795

uniform int v_ticks;
uniform bool v_toggle;

void main(void)
{
	// pulsing parameters
	const float amp = 0.25;
	const float freq = 2.0;

	vec4 pos = gl_Vertex;
	
	// do before projection
	if (v_toggle) // only pulse if told to
	{
		// only need to update xyz coordinates
    	pos.xyz = 
			(1.0 + amp * sin(freq * PI * float(v_ticks)/180.0)) * pos.xyz;
	}

	// transform the vertex using the modelview and projection matrix
	gl_Position =  gl_ModelViewProjectionMatrix * pos;

	gl_FrontColor = gl_Color; // pass out the input colour
}
