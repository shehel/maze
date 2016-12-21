#version 120 
#extension GL_EXT_gpu_shader4 : enable
#extension GL_EXT_geometry_shader4 : enable

void main() 
{ 
	// make lines from given vertices out in normal direction
    for(int i = 0; i < gl_VerticesIn; ++i) 
    { 
        gl_Position = gl_PositionIn[i]; 
		gl_FrontColor = gl_FrontColorIn[i];
        EmitVertex(); 
	}
    EndPrimitive();

	// normal spike for one of the vertices only
	{
        gl_Position = gl_PositionIn[0]; 
        EmitVertex(); 

		// secodary colour is where the normal is coded
		vec3 n = gl_FrontSecondaryColorIn[0].xyz;

		gl_Position.xyz = gl_PositionIn[0].xyz + 0.25 * n;
		gl_FrontColor = vec4(1.0, 1.0, 0.0, 1.0); // yellow

        EmitVertex(); 
    }
    EndPrimitive();
}
