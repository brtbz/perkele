#version 330 core
layout (location = 0) in vec3 position;
uniform int hex_index;
uniform vec4 camera; // x1, y1, x2, y2

void main()
{
	ivec2 map_grid_size = ivec2( 128, 128 );

	vec2 view_size = vec2( camera.z - camera.x, camera.w - camera.y );
	float odd_row_offset = 0.0f;
	float odd_column_offset = 0.0f;
	if ( hex_index % 2 == 1 ) { odd_column_offset = 14.0f; }

	float target_x = ( hex_index % map_grid_size.x ) * 24.0f + odd_row_offset - camera.x;
	float target_y = ( hex_index / map_grid_size.x ) * 28.0f + odd_column_offset - camera.y;

	target_x += 8.0f;
	target_y -= 4.0f;

	vec4 target_position = vec4( target_x, target_y, 32.0f, 48.0f );

	vec2 pos = vec2(position.x, position.y);
	pos.x = pos.x * ( target_position.z / view_size.x );
	pos.y = -pos.y * ( target_position.w / view_size.y );

	pos.x = pos.x - ( ( (view_size.x - target_position.z) / view_size.x ) );
	pos.y = pos.y + ( ( (view_size.y - target_position.w) / view_size.y ) );

	pos.x = pos.x + 2.0f * ( target_position.x / view_size.x );
	pos.y = pos.y - 2.0f * ( target_position.y / view_size.y );

    gl_Position = vec4(pos.x,pos.y, 0.0f, 1.0f);
}  
