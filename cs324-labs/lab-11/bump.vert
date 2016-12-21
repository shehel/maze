
varying vec3 normal;

void main(void)
{
    // transform the vertex using the modelview and projection matrix
    gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	
	normal = gl_Normal;
}
