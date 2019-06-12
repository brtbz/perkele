#version 330 core

out vec4 color;

uniform sampler2D input_texture;

in vec2 unit_base_uv;

void main()
{
	vec4 base_color = texture(input_texture, unit_base_uv);
	if (base_color.a < 0.1f)
	{
		discard;
	}
	color = base_color;
}
