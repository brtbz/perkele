#version 330

in vec2 frag_uv_coord;
out vec4 color;

uniform sampler2D input_texture;
uniform int time;

void main()
{
	vec4 map_color = texture(input_texture, frag_uv_coord);

	color = map_color;
}
