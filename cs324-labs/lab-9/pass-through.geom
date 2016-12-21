#version 120
#extension GL_EXT_gpu_shader4 : enable
#extension GL_EXT_geometry_shader4 : enable

void main() 
{ 
    for(int i = 0; i < gl_VerticesIn; ++i) 
    { 
        gl_Position = gl_PositionIn[i]; 
		gl_FrontColor = gl_FrontColorIn[i];
        EmitVertex(); 
    }
    EndPrimitive();
}
