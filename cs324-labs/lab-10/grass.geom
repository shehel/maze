#version 120 
#extension GL_EXT_gpu_shader4 : enable
#extension GL_EXT_geometry_shader4 : enable

void main() 
{ 
	vec4 origin = gl_PositionIn[0]; // base location of blade in model coords

	// make lines from given vertices out in normal direction
    for(int i = 0; i < gl_VerticesIn; ++i) 
    { 
        gl_Position = gl_ModelViewProjectionMatrix * gl_PositionIn[i]; 
		gl_FrontColor = gl_FrontColorIn[i];
        EmitVertex(); 
	}
    EndPrimitive();

	float rand_x = gl_FrontSecondaryColorIn[0].x;
	float rand_z = gl_FrontSecondaryColorIn[0].z;
	float phase = gl_FrontSecondaryColorIn[0].w;

	const int n = 16;
	float p_step = atan(1.0)*4.0/float(n);
    for(int i=0;i<n;i++) 
    { 
		float amp = float(i)/n * (2.0/float(n+1));
		float x = origin.x + amp*sin((float(i)*p_step) + phase + rand_x);
		float y = origin.y + float(i)/float(n);
		float z = origin.z + amp*cos((float(i)*p_step) + phase + rand_z);
    	gl_Position = gl_ModelViewProjectionMatrix * vec4(x, y, z, origin.w); // 
		gl_FrontColor = vec4(rand_z+0.35*float(i)/n, rand_x+0.65, 0.0, 0.6);
		EmitVertex();
	}

   	EndPrimitive();


}
