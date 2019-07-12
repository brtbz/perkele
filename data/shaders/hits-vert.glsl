#version 330 core

in vec2 position;
in vec2 uv_coord;

out vec2 hits_uv;

uniform int hex;
uniform int hits;

uniform vec2 sprite_sheet_size;
uniform vec2 screen_size;

uniform vec4 camera; // x1, y1, x2, y2

uniform ivec2 map_grid_size; // in hexes

// uniform int time;

void main()
{
	ivec2 map_size = map_grid_size;

	vec2 view_size = vec2( camera.z - camera.x, camera.w - camera.y );

	float common_y_offset = 12.0f;
	float odd_row_offset = 0.0f;
	float odd_column_offset = 0.0f;

	if ( hex % 2 == 1 ) { odd_column_offset = 14.0f; }

	float target_x = ( hex % map_size.x ) * 24.0f + odd_row_offset - camera.x;
	float target_y = ( hex / map_size.x ) * 28.0f + odd_column_offset + common_y_offset - camera.y;


	float target_y_offset = 29.0f;
	float target_x_offset = 13.0f;
	float mult = 0.5f;
	vec4 target_position = vec4( target_x + target_x_offset, target_y + target_y_offset, 12.0f * mult, 8.0f * mult );



	// SPRITE SHEET RECTANGLE
	vec2 sprite_size = vec2(12.0f, 8.0f);

	float margin = 1.0f;

	float x_offset_base = 1.0f + (hits % 4) * ( 12.0f + 2 * margin );
	float y_offset_base = 1.0f + (hits / 4) * ( 8.0f + 2 * margin );

	vec4 sprite_rectangle_base = vec4(x_offset_base, y_offset_base, sprite_size.x, sprite_size.y );



	// POSITION
	vec2 pos = position;
	pos.x = pos.x * ( target_position.z / view_size.x );
	pos.y = -pos.y * ( target_position.w / view_size.y );

	pos.x = pos.x - ( ( (view_size.x - target_position.z) / view_size.x ) );
	pos.y = pos.y + ( ( (view_size.y - target_position.w) / view_size.y ) );

	pos.x = pos.x + 2.0f * ( target_position.x / view_size.x );
	pos.y = pos.y - 2.0f * ( target_position.y / view_size.y );


	float z = 0.99f - float(hex / map_grid_size.x) / float(map_grid_size.y);
	if ( hex % 2 == 0 ) { z += 0.0000001f; }

	gl_Position = vec4(pos, z, 1.0f);



	// UV COORDS
	hits_uv.x = (
		uv_coord.x / (sprite_sheet_size.x / sprite_rectangle_base.z) ) +
		( sprite_rectangle_base.x / sprite_sheet_size.x );

	hits_uv.y = (
		uv_coord.y / (sprite_sheet_size.y / sprite_rectangle_base.w) ) +
		( sprite_rectangle_base.y / sprite_sheet_size.y );
}
