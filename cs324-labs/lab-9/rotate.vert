

uniform int v_ticks;
uniform bool v_toggle;

float deg_to_rad(in float deg)
{
	const float pi = 3.1415926535897932384626433832795;
	return pi*deg/180.0;
}

mat4 rotation_matrix(in vec3 axis, in float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
	// homogeneous matrix in row major form
    return mat4(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,  0.0,
                0.0, 0.0, 0.0, 1.0);
}

void main(void)
{
	vec4 pos = gl_Vertex; // input vertex

	if (v_toggle) // only rotate if told to
	{
		// set up a rotation matrix about X axis
		mat4 mr_x = rotation_matrix(vec3(1, 0, 0), deg_to_rad(float(v_ticks)));

		// apply it before model view
    	pos = mr_x * gl_Vertex;
	}

	// transform the vertex using the modelview and projection matrix
	gl_Position =  gl_ModelViewProjectionMatrix * pos;

	gl_FrontColor = gl_Color; // pass out the input colour
}
