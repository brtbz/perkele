#version 330

in vec2 position;
in vec2 uv_coord;
//in int terrain_type;
in int hex_index;
out vec2 frag_uv_coord;
//out vec2 highlight_frag_uv_coord;
uniform vec2 sprite_sheet_size;
uniform vec2 screen_size;
uniform vec4 camera; // x1, y1, x2, y2
// uniform int highlighted_hex;
uniform ivec2 map_grid_size; // in hexes

uniform int time; // ms

// could maybe have constant here and not use uniforms?
// const vec2 sprite_sheet_size = vec2(256.0f, 256.0f);
// const vec2 sprite_size = vec2(55.0f, 64.0f);

void main()
{
	vec2 view_size = vec2( camera.z - camera.x, camera.w - camera.y );

	float odd_row_offset = 0.0f;
	float odd_column_offset = 0.0f;
	if ( hex_index % 2 == 1 ) { odd_column_offset = 14.0f; }

	float target_x = ( hex_index % map_grid_size.x ) * 24.0f + odd_row_offset - camera.x;
	float target_y = ( hex_index / map_grid_size.x ) * 28.0f + odd_column_offset - camera.y;

	vec4 target_position = vec4( target_x, target_y, 32.0f, 48.0f );

	float y_offset = 0.0f;
	float x_offset = 0.0f;

	float margin = 0.0f;

	int overlay_id = 91;

	x_offset = 32.0f * float( overlay_id % 8 ) + 0.0f + margin;
	y_offset = 48.0f * float( overlay_id / 8 ) + 0.0f + margin;

	vec4 sprite_rectangle = vec4( x_offset, y_offset, 32.0f - 0.0f * margin, 48.0f - 0.0f * margin);




	vec2 pos = position;
	pos.x = pos.x * ( target_position.z / view_size.x );
	pos.y = -pos.y * ( target_position.w / view_size.y );

	pos.x = pos.x - ( ( (view_size.x - target_position.z) / view_size.x ) );
	pos.y = pos.y + ( ( (view_size.y - target_position.w) / view_size.y ) );

	pos.x = pos.x + 2.0f * ( target_position.x / view_size.x );
	pos.y = pos.y - 2.0f * ( target_position.y / view_size.y );

	// kind of an ugly hack I guess? but it works for now
	// float z = 0.99f - float(hex_index / map_grid_size.x) / float(map_grid_size.y);
	// if ( hex_index % 2 == 0 ) { z += 0.0000001f; }

	gl_Position = vec4(pos, 0.0f, 1.0f);

	frag_uv_coord.x = ( uv_coord.x / (sprite_sheet_size.x / sprite_rectangle.z) ) + ( sprite_rectangle.x / sprite_sheet_size.x );
	frag_uv_coord.y = ( uv_coord.y / (sprite_sheet_size.y / sprite_rectangle.w) ) + ( sprite_rectangle.y / sprite_sheet_size.y );
}
