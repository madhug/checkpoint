// set by the C program
uniform sampler2DRect tex;
uniform sampler2DRect depth;

void main(void)
{
   gl_FragColor = texture2DRect(tex,gl_TexCoord[0].st);
}

