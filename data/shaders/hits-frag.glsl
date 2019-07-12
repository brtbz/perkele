#version 330 core

out vec4 color;

uniform sampler2D input_texture;

in vec2 hits_uv;
uniform vec3 faction_color;

void main()
{
	vec4 base_color = texture(input_texture, hits_uv);
	if (base_color.a < 0.1f)
	{
		discard;
	}
	color = base_color;

	color.r = min(color.r, faction_color.r);
	color.g = min(color.g, faction_color.g);
	color.b = min(color.b, faction_color.b);

	//color.a = 0.5f;
}
