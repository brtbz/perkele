#version 330 core
out vec4 color;
in vec3 points_color_frag;
uniform int black;

void main()
{
	vec3 point_color;
	if (black == 1)
	{
		point_color = vec3( 0.0f, 0.0f, 0.0f);
	}
	else
	{
		point_color = points_color_frag;
	}
    color = vec4(point_color.r, point_color.g, point_color.b, 1.0f);
}
