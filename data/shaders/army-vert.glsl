#version 330 core

in vec2 position;
in vec2 uv_coord;

out vec2 unit_base_uv;
//out vec2 unit_type_uv;
//out vec2 unit_size_uv;

in ivec4 unit_data; // hex_index (location)
					// unit_base (wehrmacht, luftwaffe, red army)
					// unit_type (infantry, tanks)
					// unit_size (division, corps, regiment)
					// the three last are small enough that they can be baked in just one int

uniform vec2 sprite_sheet_size;
uniform vec2 screen_size;

uniform vec4 camera; // x1, y1, x2, y2

//uniform ivec2 map_bitmap_size;
uniform ivec2 map_grid_size; // in hexes

uniform int time;

void main()
{
	ivec2 map_size = map_grid_size;

	int hex_index = unit_data.x;
	int unit_base = unit_data.y;
	int unit_type = unit_data.z;
	int unit_size = unit_data.w;

	vec2 view_size = vec2( camera.z - camera.x, camera.w - camera.y );

	float common_y_offset = 12.0f;
	float odd_row_offset = 0.0f;
	float odd_column_offset = 0.0f;
	//if ( ( hex_index / map_size.x) % 2 == 1 ) { odd_row_offset = 27.0f; }
	if ( hex_index % 2 == 1 ) { odd_column_offset = 14.0f; }

	float target_x = ( hex_index % map_size.x ) * 24.0f + odd_row_offset - camera.x;
	float target_y = ( hex_index / map_size.x ) * 28.0f + odd_column_offset + common_y_offset - camera.y;

	float target_margin = 4.0f;
	float target_y_offset = 0.0f;
	vec4 target_position = vec4( target_x + target_margin, target_y + target_margin + target_y_offset, 32.0f - 2.0f * target_margin, 32.0f - 2.0f * target_margin ); // 55.0f, 64.0f






	// SPRITE SHEET RECTANGLE
	vec2 sprite_size = vec2(32.0f, 32.0f);

	float margin = 0.0f;

	float x_offset_base = 0.0f + (unit_base % 64) * ( 32.0f + 2 * margin );
	float y_offset_base = 0.0f + (unit_base / 64) * ( 32.0f + 2 * margin );

	float x_offset_type = 0.0f + unit_type * 62.0f;
	float y_offset_type = 51.0f;

	float x_offset_size = 0.0f + unit_size * 62.0f;
	float y_offset_size = 2 * 51.0f;
	
	vec4 sprite_rectangle_base = vec4(x_offset_base, y_offset_base, sprite_size.x, sprite_size.y );
	vec4 sprite_rectangle_type = vec4(x_offset_type, y_offset_type, sprite_size.x, sprite_size.y );
	vec4 sprite_rectangle_size = vec4(x_offset_size, y_offset_size, sprite_size.x, sprite_size.y );





	// POSITION
	vec2 pos = position;
	pos.x = pos.x * ( target_position.z / view_size.x );
	pos.y = -pos.y * ( target_position.w / view_size.y );

	pos.x = pos.x - ( ( (view_size.x - target_position.z) / view_size.x ) );
	pos.y = pos.y + ( ( (view_size.y - target_position.w) / view_size.y ) );

	pos.x = pos.x + 2.0f * ( target_position.x / view_size.x );
	pos.y = pos.y - 2.0f * ( target_position.y / view_size.y );

	float z = 0.99f - float(hex_index / map_grid_size.x) / float(map_grid_size.y);
	if ( hex_index % 2 == 0 ) { z += 0.0000001f; }

	gl_Position = vec4(pos, z, 1.0f);





	// UV COORDS

	unit_base_uv.x = (
		uv_coord.x / (sprite_sheet_size.x / sprite_rectangle_base.z) ) +
		( sprite_rectangle_base.x / sprite_sheet_size.x );

	unit_base_uv.y = (
		uv_coord.y / (sprite_sheet_size.y / sprite_rectangle_base.w) ) +
		( sprite_rectangle_base.y / sprite_sheet_size.y );
/*
	unit_type_uv.x = (
		uv_coord.x / (sprite_sheet_size.x / sprite_rectangle_type.z) ) +
		( sprite_rectangle_type.x / sprite_sheet_size.x );

	unit_type_uv.y = (
		uv_coord.y / (sprite_sheet_size.y / sprite_rectangle_type.w) ) +
		( sprite_rectangle_type.y / sprite_sheet_size.y );

	unit_size_uv.x = (
		uv_coord.x / (sprite_sheet_size.x / sprite_rectangle_size.z) ) +
		( sprite_rectangle_size.x / sprite_sheet_size.x );

	unit_size_uv.y = (
		uv_coord.y / (sprite_sheet_size.y / sprite_rectangle_size.w) ) +
		( sprite_rectangle_size.y / sprite_sheet_size.y );
*/
}