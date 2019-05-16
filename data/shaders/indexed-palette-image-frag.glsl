#version 330

in vec2 frag_uv_coord;

uniform usampler2D input_texture;
//uniform sampler2D input_texture;
uniform usampler2D palette_texture;
uniform uint time;
uniform bool phase_shift;

out vec4 color;

void main()
{
	uint phase = 0u;
	if (phase_shift)
	{
		phase = (time/50u) % 256u;
	}
	uvec4 temp_color = texture(input_texture, frag_uv_coord);

	phase += temp_color.r;
	phase %= 256u;

	//uvec4 second_stage_color = texelFetch(palette_texture, ivec2(temp_color.r, 0), 0);
	uvec4 second_stage_color = texelFetch(palette_texture, ivec2( int(phase), 0), 0);

	color.r = float(second_stage_color.b) / 255.9f;
	color.g = float(second_stage_color.g) / 255.9f;
	color.b = float(second_stage_color.r) / 255.9f;
	color.a = 1.0f;

	//color = texture(input_texture, frag_uv_coord);

	//color = vec4(1.0f, 0.5f, 1.0f, 1.0f);

}
