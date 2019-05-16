#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv_coord;
layout (location = 2) in vec4 char_src_rect; ///x1 y1 x2 y2
layout (location = 3) in vec2 char_pos;
// oiskohan näissä vähän turhaa ja päällekkäistä dataa

out vec2 frag_uv_coord;

uniform vec2 screen_size;
vec2 font_bitmap_size = vec2( 512.0f, 512.0f );  // uniformiks

void main()
{
	vec2 pos = vec2( position.x, -position.y );
	vec4 dst_rect = vec4(char_pos.x, char_pos.y, char_src_rect.z - char_src_rect.x, char_src_rect.w - char_src_rect.y);

	pos.x = pos.x * ( dst_rect.z / screen_size.x );
	pos.y = pos.y * ( dst_rect.w / screen_size.y );

	pos.x = pos.x - ( ( ( screen_size.x - dst_rect.z ) / screen_size.x ) );
	pos.y = pos.y + ( ( ( screen_size.y - dst_rect.w ) / screen_size.y ) );

	pos.x = pos.x + 2.0f * ( dst_rect.x / screen_size.x );
	pos.y = pos.y - 2.0f * ( dst_rect.y / screen_size.y );

	gl_Position = vec4(pos, 0.0f, 1.0f);

	frag_uv_coord.x = ( uv_coord.x / ( font_bitmap_size.x / dst_rect.z ) ) + ( char_src_rect.x / font_bitmap_size.x  ) ;
	frag_uv_coord.y = ( uv_coord.y / ( font_bitmap_size.y / dst_rect.w ) ) + ( char_src_rect.y / font_bitmap_size.y  );
}  
