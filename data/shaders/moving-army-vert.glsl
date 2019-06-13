#version 330 core

in vec2 position;
in vec2 uv_coord;

out vec2 unit_base_uv;

uniform int start_hex; // location; start hex index and end hex index; start and end are supposed to be neighbours
uniform int end_hex;
uniform float transition; // percent of transition complete between start and end hex; 0.0f ... 1.0f
uniform int base_sprite;

uniform vec2 sprite_sheet_size;
uniform vec2 screen_size;

uniform vec4 camera; // x1, y1, x2, y2

uniform ivec2 map_grid_size; // in hexes

uniform int time;

void main()
{
	ivec2 map_size = map_grid_size;

	int hex_index_start = start_hex;  // unit_data.x;
	int hex_index_end = end_hex;
	int unit_base = base_sprite; // unit_data.y;

	vec2 view_size = vec2( camera.z - camera.x, camera.w - camera.y );

	float common_y_offset = 12.0f;
	float odd_row_offset = 0.0f;
	float odd_column_offset_start = 0.0f;
	float odd_column_offset_end = 0.0f;

	if ( hex_index_start % 2 == 1 ) { odd_column_offset_start = 14.0f; }
	if ( hex_index_end % 2 == 1 ) { odd_column_offset_end = 14.0f; }

	float target_x_start = ( hex_index_start % map_size.x ) * 24.0f + odd_row_offset - camera.x;
	float target_y_start = ( hex_index_start / map_size.x ) * 28.0f + odd_column_offset_start + common_y_offset - camera.y;

	float target_x_end = ( hex_index_end % map_size.x ) * 24.0f + odd_row_offset - camera.x;
	float target_y_end = ( hex_index_end / map_size.x ) * 28.0f + odd_column_offset_end + common_y_offset - camera.y;

	float target_x = target_x_start + transition * ( target_x_end - target_x_start );
	float target_y = target_y_start + transition * ( target_y_end - target_y_start );

	float target_margin = 4.0f;
	float target_y_offset = 0.0f;
	vec4 target_position = vec4( target_x + target_margin, target_y + target_margin + target_y_offset, 32.0f - 2.0f * target_margin, 32.0f - 2.0f * target_margin ); // 55.0f, 64.0f






	// SPRITE SHEET RECTANGLE
	vec2 sprite_size = vec2(32.0f, 32.0f);

	float margin = 0.0f;

	float x_offset_base = 0.0f + (unit_base % 64) * ( 32.0f + 2 * margin );
	float y_offset_base = 0.0f + (unit_base / 64) * ( 32.0f + 2 * margin );

	vec4 sprite_rectangle_base = vec4(x_offset_base, y_offset_base, sprite_size.x, sprite_size.y );



	// POSITION
	vec2 pos = position;
	pos.x = pos.x * ( target_position.z / view_size.x );
	pos.y = -pos.y * ( target_position.w / view_size.y );

	pos.x = pos.x - ( ( (view_size.x - target_position.z) / view_size.x ) );
	pos.y = pos.y + ( ( (view_size.y - target_position.w) / view_size.y ) );

	pos.x = pos.x + 2.0f * ( target_position.x / view_size.x );
	pos.y = pos.y - 2.0f * ( target_position.y / view_size.y );

	// using only hex_index_start here and not some average of hex_index_start and hex_index_end
	// let's see what happens
	float z = 0.99f - float(hex_index_start / map_grid_size.x) / float(map_grid_size.y);
	if ( hex_index_start % 2 == 0 ) { z += 0.0000001f; }

	gl_Position = vec4(pos, z, 1.0f);





	// UV COORDS
	unit_base_uv.x = (
		uv_coord.x / (sprite_sheet_size.x / sprite_rectangle_base.z) ) +
		( sprite_rectangle_base.x / sprite_sheet_size.x );

	unit_base_uv.y = (
		uv_coord.y / (sprite_sheet_size.y / sprite_rectangle_base.w) ) +
		( sprite_rectangle_base.y / sprite_sheet_size.y );
}