#version 120 
#extension GL_EXT_gpu_shader4 : enable
#extension GL_EXT_geometry_shader4 : enable

void main() 
{ 
	// pass out the input vertex
    for(int i = 0; i < gl_VerticesIn; ++i) 
    { 
        gl_Position = gl_PositionIn[i]; 
		gl_FrontColor = gl_FrontColorIn[i];
        EmitVertex(); 
	}
    EndPrimitive();

	// duplicated and push away from original
    for(int i = 0; i < gl_VerticesIn; ++i) 
    { 
		// normal coded in secondary colour! 
		vec4 motion = gl_FrontSecondaryColorIn[i]; 
        gl_Position = gl_PositionIn[i] + motion;
		gl_FrontColor = vec4(1.0, 0.0, 0.0, 0.5);
        EmitVertex(); 
	}
    EndPrimitive();

}
