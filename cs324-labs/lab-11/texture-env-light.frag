
uniform sampler2D f_tex0;
uniform sampler2D f_tex1;
varying vec3 normal;
varying vec3 light_dir;
varying vec3 eye_vec;

void main (void)
{

	// base material colour
	vec4 final_colour = 
		(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + 
		(gl_LightSource[0].ambient * gl_FrontMaterial.ambient);

	vec3 N = normalize(normal);
	vec3 L = normalize(light_dir);
	
	float lambert_term = dot(N,L);
	
	if (lambert_term>0.0)
	{
		// diffuse color only
		//final_colour += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambert_term;	

		// diffuse and base texture colour
		//final_colour +=  lambert_term * texture2D(f_tex0, gl_TexCoord[0].st); // base texture pattern
		
		// diffuse + specular highlights from environment map
		final_colour += 0.9 * lambert_term * texture2D(f_tex0, gl_TexCoord[0].st) + 
				0.1 * texture2D(f_tex1, gl_TexCoord[1].st);  // env term
	}
		
	gl_FragColor = final_colour;
}
