#version 330 core

out vec4 color;

uniform sampler2D input_texture;

in vec2 unit_base_uv;
//in vec2 unit_type_uv;
//in vec2 unit_size_uv;

void main()
{
	vec4 base_color = texture(input_texture, unit_base_uv);
	//vec4 type_color = texture(input_texture, unit_type_uv);
	//vec4 size_color = texture(input_texture, unit_size_uv);

#if 0
	vec4 temp_color = vec4( 
		base_color.r * (base_color.a - type_color.a) + type_color.r * ( 1.0f - ( base_color.a - type_color.a) ),
		base_color.g * (base_color.a - type_color.a) + type_color.g * ( 1.0f - ( base_color.a - type_color.a) ), 
		base_color.b * (base_color.a - type_color.a) + type_color.b * ( 1.0f - ( base_color.a - type_color.a) ),
		max(base_color.a, type_color.a) );


	color = vec4( 
		temp_color.r * (temp_color.a - size_color.a) + size_color.r * ( 1.0f - ( temp_color.a - size_color.a) ),
		temp_color.g * (temp_color.a - size_color.a) + size_color.g * ( 1.0f - ( temp_color.a - size_color.a) ), 
		temp_color.b * (temp_color.a - size_color.a) + size_color.b * ( 1.0f - ( temp_color.a - size_color.a) ),
		max(temp_color.a, size_color.a) );
	//color vec4(1.0f, 0.5f, 0.0f, 1.0f);
#endif

	if (base_color.a < 0.1f)
	{
		discard;
	}


	color = base_color;
}




/*
	vec4 map_color = texture(input_texture, frag_uv_coord);
	vec4 highlight_color = texture(input_texture, highlight_frag_uv_coord);
	color = max(map_color, highlight_color);

	*/