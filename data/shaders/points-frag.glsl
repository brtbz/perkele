#version 330 core
out vec4 color;
uniform vec3 point_color;

void main()
{
    color = vec4(point_color.r, point_color.g, point_color.b, 1.0f);
}
