varying vec2 tex_coord;

uniform sampler2DRect tex;
uniform sampler2DRect depth;
uniform float far;
uniform float near;
uniform float width;
uniform float height;

uniform float threshold;

uniform sampler2DRect normal;

float vector_depth(float x, float y)
{
	return texture2DRect(depth, vec2(x,y)).x;
}

float compute_edge(float x, float y)
{
	return (abs(near * far / (far - vector_depth(x+1,y) * (far-near)) 
		- near*far / (far - vector_depth(x-1,y) * (far-near))) 
		+ abs(near * far / (far - vector_depth(x,y+1) * (far-near))
		- near*far / (far - vector_depth(x,y-1) * (far-near))));
}

bool depth_detect(float x, float y)
{
	if (x == 0 || x == width || y == 0 || y == height)
	{
		return false;
	}
	else if (compute_edge(x,y) > threshold)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void main(void)
{
	if(depth_detect(tex_coord.x, tex_coord.y) == true)
	{
		gl_FragColor = vec4(1.0,0.0,0.0,1.0);
	}
	else
	{
		gl_FragColor = texture2DRect(tex, tex_coord.xy);
	}
}