#version 120

uniform sampler2D f_tex0;
uniform sampler2D f_tex1;

varying vec3 normal;

void main() 
{  
	// Extract the normal from the normal map  
	vec3 p_normal = normalize(texture2D(f_tex1, gl_TexCoord[0].st).rgb * 2.0 - 1.0);  

	// apply as a pertubation to actual normal
	vec3 normal2 = normalize(gl_NormalMatrix * (normal + p_normal));
	  
	// Determine where the light is positioned (this can be set however you like)  
	vec3 light_pos = normalize(vec3(1.0, 1.0, 1.5));  
	  
	// Calculate the lighting diffuse value  
	float diffuse = max(dot(normal2, light_pos), 0.0);  
	  
	vec3 color = diffuse * texture2D(f_tex0, gl_TexCoord[0].st).rgb;  
	  
	// Set the output color of our current pixel  
	gl_FragColor = vec4(color, 1.0);  
} 
