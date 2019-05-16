#version 330

in vec2 frag_uv_coord;
out vec4 color;

in vec2 highlight_frag_uv_coord;

uniform sampler2D input_texture;
uniform int time;

void main()
{
	vec4 map_color = texture(input_texture, frag_uv_coord);
	vec4 highlight_color = texture(input_texture, highlight_frag_uv_coord);
	//highlight_color.r = 1.0f - highlight_color.r;
	//highlight_color.g = 1.0f - highlight_color.g;
	//highlight_color.b = 1.0f - highlight_color.b;
	//color = max(map_color, highlight_color);
	//color = map_color;

	//highlight_color.a =  ((sin(time/100.0f) + 1.0f)/2.0f) * highlight_color.a;
	highlight_color.a = (( ((sin(time/100.0f) + 1.0f)/2.0f) * 0.5f ) + 0.5f ) * highlight_color.a;

	if (map_color.a < 0.1f)
	{
		discard;
	}

#if 1
	vec4 temp_color = vec4( 
		mix(map_color.r, highlight_color.r, highlight_color.a), 
		mix(map_color.g, highlight_color.g, highlight_color.a), 
		mix(map_color.b, highlight_color.b, highlight_color.a),
		map_color.a );
#else
	color = map_color;
#endif

	if (temp_color.a < 0.1f)
	{
		discard;
	}

	color = temp_color;

}
