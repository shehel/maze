#version 120

// which texture?
uniform sampler2D f_tex0;

void main(void)
{
	// vanilla
	gl_FragColor = texture2D(f_tex0, gl_TexCoord[0].st);

/*
	// make is redish
	vec4 red = vec4(0.5, 0.0, 0.0, 1.0);
	gl_FragColor = 0.5 * red +  0.5 * texture2D(f_tex0, gl_TexCoord[0].st);
*/

}
