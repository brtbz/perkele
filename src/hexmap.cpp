void DrawHexes()
{
	glBindVertexArray(hex_vao);
	glUseProgram(hex_sp);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hex_map_texture);

	GLint uloc_input_texture = glGetUniformLocation( hex_sp, "input_texture" );
	glUniform1i( uloc_input_texture, 0 );

	GLint uloc_sprite_sheet_size = glGetUniformLocation( hex_sp, "sprite_sheet_size" );
	glUniform2f(uloc_sprite_sheet_size, 256.0f, 576.0f );

	GLint uloc_screen_size = glGetUniformLocation( hex_sp, "screen_size" );
	glUniform2f( uloc_screen_size, viewport_size.x, viewport_size.y );

	GLint uloc_camera = glGetUniformLocation( hex_sp, "camera" );
	glUniform4f( uloc_camera, camera.Min().x, camera.Min().y, camera.Max().x, camera.Max().y);

	GLint uloc_highlighted = glGetUniformLocation( hex_sp, "highlighted_hex" );
	glUniform1i( uloc_highlighted, highlighted_hex );

	GLint uloc_map_grid_size = glGetUniformLocation( hex_sp, "map_grid_size" );
	glUniform2i( uloc_map_grid_size, map_width, map_height );

	GLint uloc_time = glGetUniformLocation( hex_sp, "time");
	glUniform1i(uloc_time, master_timer);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, hexes_to_draw_count );

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glBindVertexArray(0);
}

void DrawHexDebugOverlay()
{
	glBindVertexArray(hex_debug_overlay_vao);
	glUseProgram(hex_debug_overlay_sp);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hex_map_texture);

	GLint uloc_input_texture = glGetUniformLocation( hex_debug_overlay_sp, "input_texture" );
	glUniform1i( uloc_input_texture, 0 );

	GLint uloc_sprite_sheet_size = glGetUniformLocation( hex_debug_overlay_sp, "sprite_sheet_size" );
	glUniform2f(uloc_sprite_sheet_size, 256.0f, 576.0f );

	GLint uloc_screen_size = glGetUniformLocation( hex_debug_overlay_sp, "screen_size" );
	glUniform2f( uloc_screen_size, viewport_size.x, viewport_size.y );

	GLint uloc_camera = glGetUniformLocation( hex_debug_overlay_sp, "camera" );
	glUniform4f( uloc_camera, camera.Min().x, camera.Min().y, camera.Max().x, camera.Max().y);

	GLint uloc_map_grid_size = glGetUniformLocation( hex_debug_overlay_sp, "map_grid_size" );
	glUniform2i( uloc_map_grid_size, map_width, map_height );

	GLint uloc_time = glGetUniformLocation( hex_debug_overlay_sp, "time");
	glUniform1i(uloc_time, master_timer);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, number_of_nodes_that_were_in_open_set_debug );

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glBindVertexArray(0);
}

void DrawEdgeAsArrow(int start_node_idx, int end_node_idx, float angle)
{
	glBindVertexArray(edge_vao);
	glUseProgram(edge_sp);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, edge_texture);

	GLint input_texture_loc = glGetUniformLocation( edge_sp, "input_texture" );
	glUniform1i(input_texture_loc, 0);

	GLint edge_nodes_loc = glGetUniformLocation( edge_sp, "edge_nodes" );
	glUniform2i(edge_nodes_loc, start_node_idx, end_node_idx);

	GLint uloc_screen_size = glGetUniformLocation( edge_sp, "screen_size" );
	glUniform2f( uloc_screen_size, viewport_size.x, viewport_size.y );

	GLint uloc_camera = glGetUniformLocation( edge_sp, "camera" );
	glUniform4f( uloc_camera, camera.Min().x, camera.Min().y, camera.Max().x, camera.Max().y);

	GLint angle_loc = glGetUniformLocation( edge_sp, "angle" );
	glUniform1f( angle_loc, angle );

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glBindVertexArray(0);
}

void UpdateHexMapBuffers()
{
	//glBindVertexArray(hex_vao); // ???

	glBindBuffer(GL_ARRAY_BUFFER, hex_indices_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, hexes_to_draw_count * sizeof(int), (const GLvoid*)&(hexes_to_draw_indices[0]) );
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for (int i = 0; i < hexes_to_draw_count; i++)
	{
		hexes_to_draw_terrain_types[i] = map_data[ hexes_to_draw_indices[i] ];
	}

	glBindBuffer(GL_ARRAY_BUFFER, hex_terrain_types_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, hexes_to_draw_count * sizeof(uint32_t), (const GLvoid*)&hexes_to_draw_terrain_types[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindVertexArray(0);
}

void UpdateHexMapBuffersForDebugOverlay()
{
	glBindBuffer(GL_ARRAY_BUFFER, hex_indices_debug_overlay_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, number_of_nodes_that_were_in_open_set_debug * sizeof(int32_t), (const GLvoid*)&nodes_that_were_in_open_set_debug[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void HexesWithinCameraBounds()
{
	ivec2 view_top_left = { (int)camera.Min().x, (int)camera.Min().y };
	ivec2 view_bottom_right = { (int)camera.Max().x, (int)camera.Max().y };

	int tile_row_min;
	int tile_row_max;
	int tile_column_min;
	int tile_column_max;

	int tile_columns_in_view;
	int tile_rows_in_view;

	int tile_width = 24; // stride 24(?), width 32
	int tile_height = 28; 

	tile_column_min = ( view_top_left.x - 20 ) / tile_width;
	tile_column_max = ( view_bottom_right.x ) / tile_width;

	tile_row_min = (view_top_left.y - 48) / tile_height;
	tile_row_max = ( view_bottom_right.y ) / tile_height; 

	tile_column_min = ClampTo( 0, map_width - 1, tile_column_min);
	tile_column_max = ClampTo( 0, map_width - 1, tile_column_max);
	tile_row_min = ClampTo( 0, map_height - 1, tile_row_min);
	tile_row_max = ClampTo( 0, map_height - 1, tile_row_max);

	tile_rows_in_view = tile_row_max - tile_row_min + 1;
	tile_columns_in_view = tile_column_max - tile_column_min + 1;

	hexes_to_draw_count = tile_rows_in_view * tile_columns_in_view;

	int i = 0;
	for ( int x = tile_column_min; x <= tile_column_max; x++)
	{
		for (int y = tile_row_min; y <= tile_row_max; y++ )
		{
			int tile_index = map_width * y + x;
			hexes_to_draw_indices[i] = tile_index;
			i++;
		}
	}

	dumb_dbg.tile_column_min = tile_column_min;
	dumb_dbg.tile_column_max = tile_column_max;
	dumb_dbg.tile_row_min = tile_row_min;
	dumb_dbg.tile_row_max = tile_row_max;

	UpdateHexMapBuffers();
}


int GetMouseOveredHex(vec2 m)
{
	// find grid coordinates with both left aligned and right aligned rectangles
	// results are called ville and kalle, because left and right are way too overloaded words
	// if ville and kalle disagree, one whose origo is closer to mouse gets to decide
	// some early exits exist
	// slightly broken near zeroes of each axis

	if ( m.x < 0.0f || m.y < 0.0f ) { return -13; }

	int hex_number = -99;
	//float odd_row_offset = 0.0f;
	float odd_column_offset = 14.0f;
	float xsize = 32.0f;
	float ysize = 48.0f;
	float xstride = 24.0f;
	float ystride = 28.0f;
	int grid_width = map_width;
	int align_shift = 8; // difference between left and right aligned bounding boxes
	float common_offset = 18.0f; // vertical offset for hex sets with empty space above hexes

	if ( m.x > xstride * grid_width + ( xsize - xstride ) || m.y > ystride * grid_width + odd_column_offset ) { return -17; }

	ivec2 ville;
	ivec2 kalle;
	int ville_hex;
	int kalle_hex;

	float ville_offset = 0.0f + common_offset;
	ville.x = (int)m.x / (int)xstride;
	if (ville.x % 2 == 1) { ville_offset += odd_column_offset; }
	ville.y = (int)(m.y - ville_offset) / (int)ystride;
	ville_hex = ville.x + ville.y * grid_width;

	float kalle_offset = 0.0f + common_offset;
	kalle.x =  ((int)m.x - align_shift) / (int)xstride;
	if (kalle.x % 2 == 1) { kalle_offset += odd_column_offset; }
	kalle.y = (int)(m.y - kalle_offset) / (int)ystride;
	kalle_hex = kalle.x + kalle.y * grid_width;

	// snprintf(dumb_debug_string, 256, "kalle.x %d .y %d ville.x %d . %d", kalle.x, kalle.y, ville.x, ville.y);

	if (kalle_hex == ville_hex)
	{
		if (kalle.x >= 0 && kalle.y >= 0 && kalle.x < grid_width && kalle.y < grid_width ) { hex_number = kalle_hex; }
		else { hex_number = -123; }
	}
	else
	{
		vec2 origo_ville = { (float)ville.x * xstride + xsize/2.0f, (float)ville.y * ystride + ysize/2.0f + ville_offset - common_offset/2.0f };
		vec2 origo_kalle = { (float)kalle.x * xstride + xsize/2.0f, (float)kalle.y * ystride + ysize/2.0f + kalle_offset - common_offset/2.0f };

		vec2 ville_to_mouse = v2_sub( origo_ville, m );
		vec2 kalle_to_mouse = v2_sub( origo_kalle, m );

		float ville_len = v2_length(ville_to_mouse);
		float kalle_len = v2_length(kalle_to_mouse);
		debug_ville_len = ville_len;
		debug_kalle_len = kalle_len;

		if ( ville_len <= kalle_len )
		{
			if ( ville.x >= 0 && ville.y >= 0 && ville.x < grid_width && ville.y < grid_width ) { hex_number = ville_hex; }
			else { hex_number = -1991; }
		}
		else
		{
			if (kalle.x >= 0 && kalle.y >= 0 && kalle.x < grid_width && kalle.y < grid_width ) { hex_number = kalle_hex; }
			else { hex_number = -97; }
		}
	}
	return hex_number;
}

/*
Hex grid coordinate systems:
STAGGERED: 
	x-axis is towards right, but staggered, y is down. 
	This is the default system for perkele
SLANTED: 
	x-axis is towards right and down, y is down, angle between them is about 60 degrees. 
	This is used for calculating distance. (Hex version of Manhattan distance(?))
*/
ivec2 StaggeredToSlanted(ivec2 staggered_hex)
{
	ivec2 slanted_hex = { staggered_hex.x, staggered_hex.y - staggered_hex.x / 2 }; // ACHTUNG!!! might need something more (maybe floor()...)
	return slanted_hex;
}

ivec2 SlantedToStaggered(ivec2 slanted_hex)
{
	ivec2 staggered_hex = { slanted_hex.x, slanted_hex.y + slanted_hex.x / 2 };
	return staggered_hex;
}

int32_t CalculateHexDistance(ivec2 start_hex, ivec2 end_hex)
{
	ivec2 slanted_start_hex = StaggeredToSlanted(start_hex);
	ivec2 slanted_end_hex = StaggeredToSlanted(end_hex);

	int32_t dx = slanted_start_hex.x - slanted_end_hex.x;
	int32_t dy = slanted_start_hex.y - slanted_end_hex.y;

	if (dx < 0 && dy < 0 || dx > 0 && dy > 0)
	{
		return abs(dx) + abs(dy);
	}
	else
	{
		return abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	}
}

bool HexesAreNeighbours(int32_t hex_a, int32_t hex_b)
{
	for (int i = 0; i < 6; i++)
	{
		if ( map_edges[ map_nodes[hex_a].edge[i] ].end_node_index == hex_b )
		{
			return true;
		}
	}
	return false;
}

void LoadEdgeYeah()
{
	int w, h;
	edge_texture = LoadTexture("data/gfx/arrow.png", &w, &h);
	GLuint edge_vs = NewShader(GL_VERTEX_SHADER, "data/shaders/edge-vert.glsl");
	GLuint edge_fs = NewShader(GL_FRAGMENT_SHADER, "data/shaders/edge-frag.glsl");
	edge_sp = NewProgram(edge_vs, edge_fs);

	GLfloat vertex_data[] = {
		// positions xyz    //texcoord uv
		 1.0,  -1.0f, 0.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,

		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
	};

	{
		glGenVertexArrays(1, &edge_vao);
		glBindVertexArray(edge_vao);

		glGenBuffers(1, &edge_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, edge_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

		GLint pos_loc = glGetAttribLocation(edge_sp, "position");
		glEnableVertexAttribArray( pos_loc );
		glVertexAttribPointer( pos_loc, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0 );

		GLint uv_loc = glGetAttribLocation(edge_sp, "uv_coord");
		glEnableVertexAttribArray( uv_loc );
		glVertexAttribPointer( uv_loc, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// pitää joku edge-bufferi tehdä kanssa niinkuin noitten heksojenkin kanssa (TODO)
	}
}

void LoadHexMapDebugOverlayStuff()
{
	// uses the same texture as the regular hex map

	GLuint hex_debug_overlay_vs = NewShader(GL_VERTEX_SHADER, "data/shaders/hex-overlay-debug-vert.glsl");
	GLuint hex_debug_overlay_fs = NewShader(GL_FRAGMENT_SHADER, "data/shaders/hex-overlay-debug-frag.glsl");
	hex_debug_overlay_sp = NewProgram(hex_debug_overlay_vs, hex_debug_overlay_fs);

	GLfloat vertex_data[] = {
		// Positions   // texcoord
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
		
		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,  
	};

	{
		glGenVertexArrays(1, &hex_debug_overlay_vao);
		glBindVertexArray(hex_debug_overlay_vao);

		glGenBuffers(1, &hex_debug_overlay_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, hex_debug_overlay_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

		glEnableVertexAttribArray( glGetAttribLocation(hex_debug_overlay_sp, "position") );
		glVertexAttribPointer( glGetAttribLocation(hex_debug_overlay_sp, "position") , 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)0 );

		glEnableVertexAttribArray( glGetAttribLocation(hex_debug_overlay_sp, "uv_coord") );
		glVertexAttribPointer( glGetAttribLocation(hex_debug_overlay_sp, "uv_coord") , 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat) ));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &hex_indices_debug_overlay_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, hex_indices_debug_overlay_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint32_t) * map_size, NULL, GL_DYNAMIC_DRAW );

		glEnableVertexAttribArray( glGetAttribLocation( hex_debug_overlay_sp, "hex_index" ) );
		glVertexAttribIPointer( glGetAttribLocation( hex_debug_overlay_sp, "hex_index" ), 1, GL_INT, 1*sizeof(GLint), (GLvoid*)0 );
		glVertexAttribDivisor( glGetAttribLocation( hex_debug_overlay_sp, "hex_index" ), 1 );

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}
}

void LoadHexMapStuff()
{
	int w, h;
	hex_map_texture = LoadTexture("data/gfx/terrain.png", &w, &h);

	GLuint hex_vs = NewShader(GL_VERTEX_SHADER, "data/shaders/hex-vert.glsl");
	GLuint hex_fs = NewShader(GL_FRAGMENT_SHADER, "data/shaders/hex-frag.glsl");
	hex_sp = NewProgram(hex_vs, hex_fs);

	GLfloat vertex_data[] = {
		// Positions   // texcoord
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
		
		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,  
	};

	{
		glGenVertexArrays(1, &hex_vao);
		glBindVertexArray(hex_vao);

		glGenBuffers(1, &hex_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, hex_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

		glEnableVertexAttribArray( glGetAttribLocation(hex_sp, "position") );
		glVertexAttribPointer( glGetAttribLocation(hex_sp, "position") , 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)0 );

		glEnableVertexAttribArray( glGetAttribLocation(hex_sp, "uv_coord") );
		glVertexAttribPointer( glGetAttribLocation(hex_sp, "uv_coord") , 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat) ));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// siis sheideri tarvii heksan indeksin. mutta se tarvii myös terrain typenkin suoraan, se ei voi hakea terrain_typeä indeksin perusteella. 
		// eli pitäisi tehdä joku terrain type array, jota täytellään sen mukaan mitä ja minkä verran tavaraa on tuossa indeksi-arrayssä(?)
		// ja tunkea se johonkin bufferiin joka syötetään gl:lle

		glGenBuffers(1, &hex_indices_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, hex_indices_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint32_t) * map_size, NULL, GL_DYNAMIC_DRAW );

		glEnableVertexAttribArray( glGetAttribLocation( hex_sp, "hex_index" ) );
		glVertexAttribIPointer( glGetAttribLocation( hex_sp, "hex_index" ), 1, GL_INT, 1*sizeof(GLint), (GLvoid*)0 );
		glVertexAttribDivisor( glGetAttribLocation( hex_sp, "hex_index" ), 1 );

		glBindBuffer(GL_ARRAY_BUFFER, 0);



		glGenBuffers(1, &hex_terrain_types_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, hex_terrain_types_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint32_t) * map_size, NULL, GL_DYNAMIC_DRAW );

		glEnableVertexAttribArray( glGetAttribLocation( hex_sp, "terrain_type" ) );
		glVertexAttribIPointer( glGetAttribLocation( hex_sp, "terrain_type" ), 1, GL_INT, 1*sizeof(GLint), (GLvoid*)0 );
		glVertexAttribDivisor( glGetAttribLocation( hex_sp, "terrain_type" ), 1 );

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}
}


#if 0
int32_t GetRandomTerrain()
{
	// int32_t roll = rand() % 1000;
	int32_t roll = MWC%1000;
	int32_t terrain = 0;
	if ( roll < 200 ) { terrain = 1; }
	else if ( roll < 400 ) { terrain = 2; }
	else if ( roll < 600 ) { terrain = 8; }
	else if ( roll < 690 ) { terrain = 0; }
	else if ( roll < 780 ) { terrain = 5; }
	else if ( roll < 870 ) { terrain = 6; }
	else if ( roll < 995 ) { terrain = 7; }
	else if ( roll < 997 ) { terrain = 3; }
	else if ( roll < 1000 ) {terrain = 4; }

	return terrain;
}
#else

int LoadMapTerrainFromCSV(const char* file_path, int expected_map_size, uint32_t* map_data_ptr)
{
	// OPTIONAL TODO: separate program that converts csv to something more tighter

	int return_code = 0;
	FILE *fp = fopen(file_path, "r");

	if ( fp == NULL )
	{
		return_code = -1;
	}
	else
	{
		char line[512];
		int i = 0;
		//int line_count = 0;
		while( fgets(line, 512, fp) )
		{
			//line_count++;
			char *token = strtok(line, ",");
			while(token && i < expected_map_size)
			{
				*map_data_ptr = atoi(token);
				i++;
				map_data_ptr++;
				token = strtok(NULL, ",");
			}
		}
		fclose(fp);
	}

	return return_code;
}

int32_t terrain_table[1000] = {
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 
	0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 
	0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 
	0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 
	0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 
	0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 
	0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 
	0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 
	0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 
	0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 
	0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 
	0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 
	0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 
	0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 
	0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 
	0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 
	0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 
	0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 
	0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 
	0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 
	0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 
	0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 
	0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 
	0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 
	0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 
	0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 
	0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 
	0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 
	0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 
	0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 
	0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0003, 0x0003, 0x0004, 0x0004, 0x0004, 
};
int32_t GetRandomTerrain()
{
	// return terrain_table[ rand() % 1000 ];
	// return terrain_table[ MWC % 1000 ];
	// return MWC % 120;
	return MWC % 7;
}
#endif

void InitMapNodes()
{
	int32_t total_n_count = 0;
	map_nodes = (MapNode*)malloc(map_size * sizeof(MapNode));
	map_node_neighbours = (MapNodeNeighbours*)malloc(map_size * sizeof(MapNodeNeighbours));
	came_along_edges = (int32_t*)malloc(map_size * sizeof(int32_t));

	for (int i = 0; i < map_size; i++)
	{
		came_along_edges[i] = -1;
	}

	for (int i = 0; i < map_size; i++)
	{
		map_nodes[i].index = i;
		map_nodes[i].x = i % map_width;
		map_nodes[i].y = i / map_width;
		map_nodes[i].occupier = -1;
		map_nodes[i].pathfind_island_id = -1;
		map_nodes[i].terrain = PASSABLE;
		if (map_data[i] == 5 || map_data[i] == 6)
		{
			map_nodes[i].terrain = IMPASSABLE;
		}

		map_node_neighbours[i].n_north = -1;
		map_node_neighbours[i].n_northeast = -1;
		map_node_neighbours[i].n_southeast = -1;
		map_node_neighbours[i].n_south = -1;
		map_node_neighbours[i].n_southwest = -1;
		map_node_neighbours[i].n_northwest = -1;

		for (int j = 0; j < 6; j++)
		{
			map_nodes[i].edge[j] = -1;
		}

		// north
		if (map_nodes[i].y < map_height - 1)
		{
			map_node_neighbours[i].n_south = i + map_width;
		}

		if (map_nodes[i].y > 0)
		{
			map_node_neighbours[i].n_north = i - map_width;
		}

		// northwest
		if (map_nodes[i].x > 0 )
		{
			if (i % 2 == 1)
			{
				map_node_neighbours[i].n_northwest = i - 1;
			}
			else if ( i % 2 == 0 && map_nodes[i].y > 0)
			{
				map_node_neighbours[i].n_northwest = i - 1 - map_width;
			}
		}

		// southwest
		if (map_nodes[i].x > 0 )
		{
			if ( i % 2 == 0 )
			{
				map_node_neighbours[i].n_southwest = i - 1;
			}
			else if ( i % 2 == 1 && map_nodes[i].y < map_height - 1 )
			{
				map_node_neighbours[i].n_southwest = i - 1 + map_width;
			}
		}

		// northeast
		if (map_nodes[i].x < map_width - 1)
		{
			if ( i % 2 == 1)
			{
				map_node_neighbours[i].n_northeast = i + 1;
			}
			else if ( i % 2 == 0 && map_nodes[i].y > 0)
			{
				map_node_neighbours[i].n_northeast = i + 1 - map_width;
			}
		}

		// southeast
		if (map_nodes[i].x < map_width - 1)
		{
			if (i % 2 == 0)
			{
				map_node_neighbours[i].n_southeast = i + 1;
			}
			else if ( i % 2 == 1 && map_nodes[i].y < map_height - 1)
			{
				map_node_neighbours[i].n_southeast = i + 1 + map_width;
			}
		}

		int32_t n_count = 0;
		if ( map_node_neighbours[i].n_north != -1 ) { n_count++; }
		if ( map_node_neighbours[i].n_northeast != -1 ) { n_count++; }
		if ( map_node_neighbours[i].n_southeast != -1 ) { n_count++; }
		if ( map_node_neighbours[i].n_south != -1 ) { n_count++; }
		if ( map_node_neighbours[i].n_southwest != -1 ) { n_count++; }
		if ( map_node_neighbours[i].n_northwest != -1 ) { n_count++; }
		map_node_neighbours[i].n_count = n_count;

		total_n_count += n_count;
	}

	printf("MAP SIZE: %d\n", map_size);
	printf("TOTAL NEIGHBOUR COUNT: %d\n", total_n_count);

	map_edges = (MapEdge*)malloc(total_n_count * sizeof(MapEdge));

	for (int i = 0; i < total_n_count; i++)
	{
		map_edges[i].index = i;
		map_edges[i].cost = 1;
	}

	int32_t edge_number = 0;
	for (int i = 0; i < map_size; i++)
	{
		if (map_node_neighbours[i].n_north != -1 )
		{
			map_edges[edge_number].start_node_index = i;
			map_edges[edge_number].end_node_index = map_node_neighbours[i].n_north;
			map_edges[edge_number].direction = 0;
			map_nodes[i].edge[0] = edge_number;
			edge_number++;
		}

		if (map_node_neighbours[i].n_northeast != -1 )
		{
			map_edges[edge_number].start_node_index = i;
			map_edges[edge_number].end_node_index = map_node_neighbours[i].n_northeast;
			map_edges[edge_number].direction = 1;
			map_nodes[i].edge[1] = edge_number;
			edge_number++;
		}
		if (map_node_neighbours[i].n_southeast != -1 )
		{
			map_edges[edge_number].start_node_index = i;
			map_edges[edge_number].end_node_index = map_node_neighbours[i].n_southeast;
			map_edges[edge_number].direction = 2;
			map_nodes[i].edge[2] = edge_number;
			edge_number++;
		}
		if (map_node_neighbours[i].n_south != -1 )
		{
			map_edges[edge_number].start_node_index = i;
			map_edges[edge_number].end_node_index = map_node_neighbours[i].n_south;
			map_edges[edge_number].direction = 3;
			map_nodes[i].edge[3] = edge_number;
			edge_number++;
		}
		if (map_node_neighbours[i].n_southwest != -1 )
		{
			map_edges[edge_number].start_node_index = i;
			map_edges[edge_number].end_node_index = map_node_neighbours[i].n_southwest;
			map_edges[edge_number].direction = 4;
			map_nodes[i].edge[4] = edge_number;
			edge_number++;
		}
		if (map_node_neighbours[i].n_northwest != -1 )
		{
			map_edges[edge_number].start_node_index = i;
			map_edges[edge_number].end_node_index = map_node_neighbours[i].n_northwest;
			map_edges[edge_number].direction = 5;
			map_nodes[i].edge[5] = edge_number;
			edge_number++;
		}
	}
	printf("EDGE NUMBER IN THE END: %d\n", edge_number);
	total_edge_count = edge_number;
}

void UpdateEdgeTravelCosts()
{
	for (int i = 0; i < total_edge_count; i++)
	{
#if 0
		if (map_nodes[ map_edges[i].end_node_index ].terrain == IMPASSABLE )
		{
			map_edges[i].terrain = IMPASSABLE;
		}
		else if (map_nodes[ map_edges[i].end_node_index ].terrain == PASSABLE )
		{
			map_edges[i].terrain = PASSABLE;
		}
#endif
		if ( map_data[ map_edges[i].end_node_index ] == 5) // mountain
		{
			map_edges[i].cost = 4;
		}
		else if ( map_data[ map_edges[i].end_node_index ] == 1) // light wood
		{
			map_edges[i].cost = 2;
		}
		else if ( map_data[ map_edges[i].end_node_index ] == 2) // haevy wood
		{
			map_edges[i].cost = 2;
		}
		else if ( map_data[ map_edges[i].end_node_index ] == 3) // hills
		{
			map_edges[i].cost = 3;
		}
		else if ( map_data[ map_edges[i].end_node_index ] == 4) // wooded hills
		{
			map_edges[i].cost = 3;
		}
	}
}

bool FloodFill(int32_t map_index, int32_t island_id)
{
	if ( map_nodes[map_index].terrain == IMPASSABLE ) { return false; }
	else if ( map_nodes[map_index].pathfind_island_id != -1 ) { return false; }
	else { map_nodes[map_index].pathfind_island_id = island_id;	}

	for (int i = 0; i < 6; i++)
	{
		if (map_nodes[map_index].edge[i] != -1)
		{
			FloodFill( map_edges[ map_nodes[map_index].edge[i] ].end_node_index, island_id );
		}
	}
	return true;
}

void FloodFillIslandIds()
{
	int32_t island_id = 1;

	for (int i = 0; i < map_size; i++)
	{
		if (FloodFill(i, island_id)) { island_id++; }
	}
}

void InitHexMap()
{
	rng_z = rand();
	rng_w = rand();

	map_data = (uint32_t*)malloc( (map_size) * sizeof(uint32_t));

	for (int i = 0; i < map_size; i++)
	{
		map_data[i] = GetRandomTerrain();
	}

	int load_result = LoadMapTerrainFromCSV("data/maps/terrain_perke.csv", map_size, map_data);
	if (load_result == 0)
	{
		printf("level load success!!!\n");
	}
	else
	{
		printf("level load failed mwha!\n");
	}

	hexes_to_draw_indices = (uint32_t*)malloc(map_size * sizeof(uint32_t));
	hexes_to_draw_terrain_types = (uint32_t*)malloc(map_size * sizeof(uint32_t));
	LoadHexMapStuff();
	LoadHexMapDebugOverlayStuff();

	LoadEdgeYeah();

	UpdateHexMapBuffers();

	InitMapNodes();
	UpdateEdgeTravelCosts();
	FloodFillIslandIds();
}

void ClearHexMapStuff()
{
	free(map_nodes);
	free(map_edges);

	free(map_data);
	free(hexes_to_draw_indices);
	free(hexes_to_draw_terrain_types);
}
