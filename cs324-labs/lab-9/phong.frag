varying vec4 pos_eye;
varying vec3 normal_eye;

void main(void)
{
	vec4 pos = normalize(pos_eye);
	vec3 normal = normalize(normal_eye); // in eye space
	vec3 light_direction, reflected;
	vec4 ambient, diffuse, global_ambient, specular;
	float N_dot_L; // lambertian
	float N_dot_L_halfway; // reflected vector approximation
	float N_dot_R; // specular

	/* now normalize the light's direction after taking vertex pos 
		from the light away from pos */
	light_direction = normalize(vec3(gl_LightSource[0].position)-vec3(pos));

	/* compute the cos of the angle between the normal and lights direction.  
		The light is directional so the direction is constant for every vertex.  
		Since these two are normalized the cosine is the dot product. 
		We also need to clamp the result to the [0,1] range. */
	N_dot_L = max(0.0, dot(normal, light_direction));

	/* Compute the diffuse term */
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;

	/* Compute the ambient and globalAmbient terms */
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	//global_ambient = gl_LightModel.ambient * gl_FrontMaterial.ambient;

	/* Compute the specular term if N_dot_L is larger than zero */
	specular = vec4(0.0, 0.0, 0.0, 0.0); // initialize
	if (N_dot_L > 0.0) 
	{
		// half way approximation
		// normalize the half-vector, and then compute the
		// cosine (dot product) with the normal
		N_dot_L_halfway = max(0.0, dot(normal, gl_LightSource[0].halfVector.xyz));

		// do it properly with reflected vector
		//reflected = reflect(light_direction, normal);
		//N_dot_R = max(0.0, dot(-normal, reflected));

		// size of specularity in pow() bit
		specular = gl_FrontMaterial.specular * gl_LightSource[0].specular *
				pow(N_dot_L_halfway, gl_FrontMaterial.shininess);
				//pow(N_dot_R, gl_FrontMaterial.shininess);
	}

	// vertex is now lit with Phong lighting value
	vec4 colour =  
			//global_ambient + 
			ambient + // ambient
			N_dot_L * diffuse +  // diffuse 
			specular; // specular term

	// transform the vertex using the modelview and projection matrix
	gl_FragColor =  colour;
}
