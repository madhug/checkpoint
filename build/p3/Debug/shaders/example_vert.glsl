varying vec3 tex_coord;

void main(void)
{
    // set vertex position using fixed-functionality transform
    // gl_Position is the primary "output" of the vertex shader

	gl_Position = gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
}
