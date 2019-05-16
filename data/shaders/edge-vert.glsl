#version 330

in vec3 position;
in vec2 uv_coord;
uniform ivec2 edge_nodes;
uniform vec2 screen_size;
uniform vec4 camera; // x1, y1, x2, y2
uniform float angle;

out vec2 frag_uv_coord;

void main()
{
	frag_uv_coord = uv_coord;


	int map_width = 128;
	float o1_odd_row_offset = 0.0f;
	float o1_odd_column_offset = 0.0f;

	float o2_odd_row_offset = 0.0f;
	float o2_odd_column_offset = 0.0f;

	if ( edge_nodes.x % 2 == 1 ) { o1_odd_column_offset = 14.0f; }
	if ( edge_nodes.y % 2 == 1 ) { o2_odd_column_offset = 14.0f; }

	vec2 origo1;
	vec2 origo2;

	origo1.x = ( edge_nodes.x % map_width ) * 24.0f + o1_odd_row_offset + 16.0f; // - camera.x + 12.0f;
	origo1.y = ( edge_nodes.x / map_width ) * 28.0f + o1_odd_column_offset + 34.0f; // - camera.y + 34.0f;

	origo2.x = ( edge_nodes.y % map_width ) * 24.0f + o2_odd_row_offset + 16.0f; // - camera.x + 12.0f;
	origo2.y = ( edge_nodes.y / map_width ) * 28.0f + o2_odd_column_offset + 34.0f; // - camera.y + 34.0f;


	//ivec2 map_grid_size = ivec2(128, 128);
	//int hex_index = edge_nodes.x;

	vec2 view_size = vec2( camera.z - camera.x, camera.w - camera.y );

	//if ( hex_index % 2 == 1 ) { odd_column_offset = 14.0f; }

	//float target_x = ( hex_index % map_grid_size.x ) * 24.0f + odd_row_offset - camera.x;
	//float target_y = ( hex_index / map_grid_size.x ) * 28.0f + odd_column_offset - camera.y;

	vec2 master_origo;
	//master_origo.x = abs(origo1.x - origo2.x) - camera.x;
	//master_origo.y = abs(origo1.y - origo2.y) - camera.y;

	master_origo.x = (origo1.x + origo2.x) / 2.0f - camera.x;
	master_origo.y = (origo1.y + origo2.y) / 2.0f - camera.y; 


	vec4 target_position = vec4( master_origo.x - 11.0f, master_origo.y - 14.0f, 22.0f, 28.0f );

	//float angle = 60.0f;
	float angle_rad = radians(angle);
	mat4 rotation_matrix = mat4(
	  cos(angle_rad), sin(angle_rad), 0.0f, 0.0f,
	 -sin(angle_rad), cos(angle_rad), 0.0f, 0.0f,
	  0.0f, 0.0f, 1.0f, 0.0f,
	  0.0f, 0.0f, 0.0f, 1.0f);


	vec4 pos = rotation_matrix * vec4(position, 1.0f);
	pos.x = pos.x * ( target_position.z / view_size.x );
	pos.y = -pos.y * ( target_position.w / view_size.y );

	pos.x = pos.x - ( ( (view_size.x - target_position.z) / view_size.x ) );
	pos.y = pos.y + ( ( (view_size.y - target_position.w) / view_size.y ) );

	pos.x = pos.x + 2.0f * ( target_position.x / view_size.x );
	pos.y = pos.y - 2.0f * ( target_position.y / view_size.y );




	gl_Position = pos; // vec4(pos, 0.0f, 1.0f);

	//gl_Position = vec4(position, 1.0f);
}