
uniform sampler2D f_tex0;
uniform sampler2D f_tex1; // assume this is the environment map

void main (void)
{
	vec4 colour = texture2D(f_tex0, gl_TexCoord[0].st);
	vec4 env = texture2D(f_tex1, gl_TexCoord[1].st);

	//gl_FragColor = colour*0.9 + env*0.1;
	gl_FragColor = env;
}
