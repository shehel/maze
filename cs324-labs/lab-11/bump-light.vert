
varying vec4 pos_eye;
varying vec3 normal_eye;
varying vec4 colour;
varying mat3 normal_matrix;


void main(void)
{
	// transform the vertex using the modelview and projection matrix
	gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;

	
	// vertex position in eye coordinates
	pos_eye = gl_ModelViewMatrix * gl_Vertex;

	// normal in eye coordinates
	normal_eye = gl_NormalMatrix * gl_Normal;
	
	colour = gl_Color; // pass out the input colour

   	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

}
