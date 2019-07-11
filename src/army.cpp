void DrawArmies(int32_t count)
{
	glBindVertexArray(army_vao);
	glUseProgram(army_sp);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, army_texture);

	GLint input_texture_loc = glGetUniformLocation( army_sp, "input_texture" );
	glUniform1i( input_texture_loc, 0 );

	GLint sprite_sheet_size_loc = glGetUniformLocation( army_sp, "sprite_sheet_size" );
	glUniform2f( sprite_sheet_size_loc, 2048.0f, 1536.0f );

	GLint screen_size_loc = glGetUniformLocation( army_sp, "screen_size" );
	glUniform2f( screen_size_loc, viewport_size.x, viewport_size.y );

	GLint camera_loc = glGetUniformLocation( army_sp, "camera" );
	glUniform4f( camera_loc, camera.Min().x, camera.Min().y, camera.Max().x, camera.Max().y );

	GLint map_grid_size_loc = glGetUniformLocation( army_sp, "map_grid_size" );
	glUniform2i( map_grid_size_loc, map_width, map_height );

	GLint time_loc = glGetUniformLocation( army_sp, "time" );
	glUniform1i( time_loc, master_timer );

	GLint selected_army_pos_loc = glGetUniformLocation( army_sp, "selected_army");
	int32_t selected_army_position_hex = -1;
	if ( selected_army )
	{
		selected_army_position_hex = selected_army->position_hex;
	}
	glUniform1i( selected_army_pos_loc, selected_army_position_hex );

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glBindVertexArray(0);
}

void DrawMovingArmy(int32_t a, int32_t start_hex, int32_t end_hex, float transition, int32_t base_sprite)
{
	glBindVertexArray(moving_army_vao);
	glUseProgram(moving_army_sp);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, army_texture);

	GLint input_texture_loc = glGetUniformLocation( moving_army_sp, "input_texture" );
	glUniform1i( input_texture_loc, 0 );

	GLint sprite_sheet_size_loc = glGetUniformLocation( moving_army_sp, "sprite_sheet_size" );
	glUniform2f( sprite_sheet_size_loc, 2048.0f, 1536.0f );

	GLint screen_size_loc = glGetUniformLocation( moving_army_sp, "screen_size" );
	glUniform2f( screen_size_loc, viewport_size.x, viewport_size.y );

	GLint camera_loc = glGetUniformLocation( moving_army_sp, "camera" );
	glUniform4f( camera_loc, camera.Min().x, camera.Min().y, camera.Max().x, camera.Max().y );

	GLint map_grid_size_loc = glGetUniformLocation( moving_army_sp, "map_grid_size" );
	glUniform2i( map_grid_size_loc, map_width, map_height );

	GLint start_hex_loc = glGetUniformLocation( moving_army_sp, "start_hex" );
	GLint end_hex_loc = glGetUniformLocation( moving_army_sp, "end_hex" );
	GLint transition_loc = glGetUniformLocation( moving_army_sp, "transition" );
	GLint base_sprite_loc = glGetUniformLocation( moving_army_sp, "base_sprite" );

	glUniform1i(start_hex_loc, start_hex);
	glUniform1i(end_hex_loc, end_hex);
	glUniform1f(transition_loc, transition);
	glUniform1i(base_sprite_loc, base_sprite);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glBindVertexArray(0);
}

void DrawAllPoints(float size, bool black)
{
	glBindVertexArray(points_vao);
	glUseProgram(points_sp);

	glPointSize(size);

	GLint camera_loc = glGetUniformLocation( points_sp, "camera");
	GLint black_loc = glGetUniformLocation( points_sp, "black");

	glUniform4f( camera_loc, camera.Min().x, camera.Min().y, camera.Max().x, camera.Max().y );

	int black_int = 0;
	black ? black_int = 1 : black_int = 0;
	glUniform1i(black_loc, black_int );

	glDrawArraysInstanced(GL_POINTS, 0, 1, 183);

	glUseProgram(0);
	glBindVertexArray(0);
}

void InitUnitDataBuffer()
{
	unit_data = (ivec4*)malloc( unit_limit * sizeof(ivec4) );
}

void FreeUnitDataBuffer()
{
	free(unit_data);
}

void UpdateUnitDataBuffer()
{
	for (int i = 0; i < 183; i++)
	{
		ivec4 unit_datum = 
		{ 
			test_armies[i].position_hex,
			test_armies[i].base_sprite,
			0,//test_divisions[i].unit_type,
			0,//test_divisions[i].unit_size
		};
		if (test_armies[i].draw == false)
		{
			unit_datum.y = 0;
		}

		unit_data[i] = unit_datum;
	}

	glBindBuffer(GL_ARRAY_BUFFER, unit_data_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, unit_data_count * sizeof(ivec4), (const GLvoid*)&(unit_data[0]) );
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unit_data_buffer_needs_update = false;
}

void LoadArmyGraphicStuff()
{
	// BASIC SHADER
	int w, h;
	army_texture = LoadTexture("data/gfx/DungeonCrawl_ProjectUtumnoTileset.png", &w, &h);
	GLuint army_vs = NewShader(GL_VERTEX_SHADER, "data/shaders/army-vert.glsl");
	GLuint army_fs = NewShader(GL_FRAGMENT_SHADER, "data/shaders/army-frag.glsl");
	army_sp = NewProgram(army_vs, army_fs);

	GLfloat vertex_data[] = {
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
	};

	glGenVertexArrays(1, &army_vao);
	glBindVertexArray(army_vao);

	glGenBuffers(1, &army_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, army_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), &vertex_data[0], GL_STATIC_DRAW);

	GLint position_loc = glGetAttribLocation(army_sp, "position");
	glEnableVertexAttribArray( position_loc );
	glVertexAttribPointer( position_loc, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)0 );

	GLint uv_coord_loc = glGetAttribLocation(army_sp, "uv_coord");
	glEnableVertexAttribArray( uv_coord_loc );
	glVertexAttribPointer( uv_coord_loc, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);



	glGenBuffers(1, &unit_data_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, unit_data_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * unit_limit, NULL, GL_DYNAMIC_DRAW);

	GLint unit_data_loc = glGetAttribLocation( army_sp, "unit_data" );
	glEnableVertexAttribArray( unit_data_loc );
	glVertexAttribIPointer( unit_data_loc, 4, GL_UNSIGNED_INT, 4*sizeof(GLuint), (GLvoid*)0 );
	glVertexAttribDivisor( unit_data_loc, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


	// SHADER FOR ANIMATING MOVEMENT
	GLuint moving_army_vs = NewShader(GL_VERTEX_SHADER, "data/shaders/moving-army-vert.glsl");
	GLuint moving_army_fs = NewShader(GL_FRAGMENT_SHADER, "data/shaders/moving-army-frag.glsl");
	moving_army_sp = NewProgram(moving_army_vs, moving_army_fs);

	glGenVertexArrays(1, &moving_army_vao);
	glBindVertexArray(moving_army_vao);

	glGenBuffers(1, &moving_army_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, moving_army_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), &vertex_data[0], GL_STATIC_DRAW);

	position_loc = glGetAttribLocation(moving_army_sp, "position");
	glEnableVertexAttribArray(position_loc);
	glVertexAttribPointer( position_loc, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)0 );

	uv_coord_loc = glGetAttribLocation(moving_army_sp, "uv_coord");
	glEnableVertexAttribArray( uv_coord_loc );
	glVertexAttribPointer( uv_coord_loc, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void LoadPoints() // indicator for units that shows whether they are active or used their move already and so on
{
    GLuint points_vs = NewShader(GL_VERTEX_SHADER, "data/shaders/points-vert.glsl");
    GLuint points_fs = NewShader(GL_FRAGMENT_SHADER, "data/shaders/points-frag.glsl");
    points_sp = NewProgram(points_vs, points_fs);

    GLfloat vertex_data[] = {
    	0.0f, 0.0f, 0.0f
    };

    glGenVertexArrays(1, &points_vao);
    glBindVertexArray(points_vao);
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    GLint attrib_loc_position = glGetAttribLocation(points_sp, "position");
    glEnableVertexAttribArray(attrib_loc_position);
    glVertexAttribPointer(attrib_loc_position, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0 );

    glBindBuffer(GL_ARRAY_BUFFER,0);



    glGenBuffers(1, &points_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, points_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unit_limit * 3, NULL, GL_DYNAMIC_DRAW);

    GLint points_color_loc = glGetAttribLocation( points_sp, "points_color");
    glEnableVertexAttribArray(points_color_loc);
    glVertexAttribPointer(points_color_loc, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0 );
    glVertexAttribDivisor( points_color_loc, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);



    // temporary solution(!) :)
	// glGenBuffers(1, &unit_data_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, unit_data_buffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(int) * unit_limit * 4, NULL, GL_DYNAMIC_DRAW);

	GLint unit_data_loc = glGetAttribLocation( points_sp, "unit_data" );
	glEnableVertexAttribArray( unit_data_loc );
	glVertexAttribIPointer( unit_data_loc, 4, GL_UNSIGNED_INT, 4*sizeof(GLuint), (GLvoid*)0 );
	glVertexAttribDivisor( unit_data_loc, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void RandomName(Army *a)
{
	const char* names[] = 
	{
		"Make", "Pera", "Maukka", "Ilya",
		"Konsta", "Arska", "Sepi", "Jaakko",
		"Igor", "Ivan", "John", "Spike",
		"Hans", "Oleg", "Sven", "Michael",

		"Aarno", "Sergei", "Marco", "Jakke",
		"Kristian", "Jonne", "Sulevi", "Brad",
		"Flash", "Razor", "Jeff", "Vlad",
		"Kirill", "Raivo", "Kaspar", "Robert",

		"Frank", "Luca", "Jyri", "Eki",
		"Pekkis", "Leksa", "Aake", "Rami",
		"Teme", "Enrique", "Dieter", "Vallu",
		"Tyrone", "Turo", "Aki", "Calle",

		"Jugi", "Masa", "Ola", "Juhis",
		"Teukka", "Pate", "Tumppi", "Sami",
		"Iiro", "Fredi", "Vova", "Darijus",
		"Herkus", "Jurgis", "Pilypas", "Tomas"
	};

	int random_name = rand() % 64;
	strncpy(a->name, names[random_name], 31);
	a->name[31] = '\0';
}

void SetArmyName(Army *a, const char *name)
{
	strncpy(a->name, name, 32);
	a->name[31] = '\0';
}

void MoveArmyToNewHex(int32_t army, int32_t hex)
{
	map_nodes[ test_armies[army].position_hex ].occupier = -1;
	test_armies[army].position_hex = hex;
	map_nodes[ test_armies[army].position_hex ].occupier = army;
}

void BeginArmyMoveAnimation(int32_t army, int32_t start, int32_t end)
{
	// I guess most of the movement code is handled here in c++. 
	// glsl is just told: now draw unit between these two hexes with transition being x percent complete
	// only one unit moves at a time(1), so I guess there's no point in adding that 'between hexes' capability to the normal unit drawing code
	// when unit starts to move, stop drawing it in normal way, and resume normal operations once it stops.
	//
	// 1) but I want glorious WEGO-system! everyone should move at same time! (NOT YET!)
	moving_army = army;
	test_armies[army].draw = false;
	army_moving = true;
	movement_starts = master_timer;
	path_position = path_edges_size;
	prev_hex = start;
	moving_to_hex = map_edges[ path_edges[path_position] ].end_node_index;
}

void AdvanceArmyMoveAnimation(int32_t army)
{
	movement_timer = master_timer;
	if ( movement_starts + ms_per_hex < movement_timer )
	{
		path_position--;

		if (path_position < 0)
		{
			//end
			army_moving = false;
			test_armies[army].move_done = true;
			test_armies[army].draw = true;

			MoveArmyToNewHex(army, current_path.y);
			draw_path = false;
			unit_data_buffer_needs_update = true;		
		}
		else
		{
			movement_starts = master_timer;
			prev_hex = moving_to_hex;
			moving_to_hex = map_edges[ path_edges[path_position] ].end_node_index;
		}
	}
}

void BeginArmyAttackAnimation(int32_t army, int32_t start, int32_t end)
{
	// attack animation is a move animation that goes nowhere, just quickly bumps against the target
	moving_army = army;
	test_armies[army].draw = false;
	army_attacking = true;
	movement_starts = master_timer;
}

void AdvanceArmyAttackAnimation(int32_t army)
{
	movement_timer = master_timer;
	if ( movement_starts + attack_anim_len < movement_timer )
	{
		// attack anim done

		army_attacking = false;
		test_armies[army].move_done = true;
		test_armies[army].draw = true;

		draw_path = false;
		unit_data_buffer_needs_update = true;
	}
}

bool HexIsFree(int32_t hex)
{
	return map_nodes[ hex ].occupier == -1 ? true : false;
}

void SaveUnitLocationsToFile(const char* file_path)
{
	int data_size = 0;
	data_size = 183 * sizeof(Army);

	uint8_t *data = (uint8_t*)malloc(data_size);

	memcpy(data, &test_armies[0], data_size);

	FILE *fp = fopen(file_path, "wb");

	fwrite(data, data_size, 1, fp);

	fclose(fp);

	free(data);
}

int LoadUnitLocationsFromFile(const char* file_path)
{
	FILE *fp = fopen(file_path, "rb");

	if ( fp == NULL )
	{
		fprintf(stderr, "can'f find %s\n", file_path );

		return -1;
	}

	fpos_t begin;
	fgetpos(fp, &begin);
	fseek(fp, 0, SEEK_END);
	int file_size = ftell(fp);
	fsetpos(fp, &begin);

	uint8_t *data = (uint8_t*)malloc(file_size);

	fread(data, file_size, 1, fp);

	memcpy(&test_armies[0], data, file_size);

	fclose(fp);
	free(data);

	return 0;
}

void ArrangePiecesAroundOnTheBoardJohnImOnlyTesting()
{
	MoveArmyToNewHex(177, 4532);
	MoveArmyToNewHex(176, 4533);
	MoveArmyToNewHex(174, 4534);
	test_armies[177].base_sprite = GOBLIN_BATHROBE_RED;

	test_armies[174].faction = FACTION_GOBLINS;
	test_armies[176].faction = FACTION_GOBLINS;
	test_armies[177].faction = FACTION_GOBLINS;


	MoveArmyToNewHex(100, 3891);
	MoveArmyToNewHex(101, 3892);
	MoveArmyToNewHex(102, 3893);
	MoveArmyToNewHex(103, 3894);
	test_armies[100].base_sprite = SURFER_BOY;
	test_armies[101].base_sprite = SURFER_BOY;
	test_armies[102].base_sprite = SURFER_BOY;
	test_armies[103].base_sprite = SURFER_BOY;

	test_armies[100].movement = 6;
	test_armies[101].movement = 6;
	test_armies[102].movement = 6;
	test_armies[103].movement = 6;

	test_armies[100].faction = FACTION_SURFERS;
	test_armies[101].faction = FACTION_SURFERS;
	test_armies[102].faction = FACTION_SURFERS;
	test_armies[103].faction = FACTION_SURFERS;

	SetArmyName(&test_armies[100], "1st Surfer Battalion 'Sweden'");
	SetArmyName(&test_armies[101], "2nd Surfer Battalion 'Norway'");
	SetArmyName(&test_armies[102], "3rd Surfer Battalion 'Stockholm'");
	SetArmyName(&test_armies[103], "4th Surfer Battalion 'Narvik'");

	SetArmyName(&test_armies[174], "1st Royal Goblin Battalion");
	SetArmyName(&test_armies[176], "2nd Royal Goblin Battalion");
	SetArmyName(&test_armies[177], "3rd Royal Goblin Battalion");

	vec2 temp_hehe = {-1.0f, -1.0f};
	vec2 temp_vec2 = { 860.0f, 650.0f };
	camera.IncreaseSizeBy(0.42f, temp_hehe);
	camera.MoveToNewMin(temp_vec2);



	for ( int i = 0; i < map_size; i++)
	{
		map_nodes[i].occupier = -1;
	}

	LoadUnitLocationsFromFile("autosave.hws");
	for (int i = 0; i < 183; i++)
	{
		MoveArmyToNewHex(i, test_armies[i].position_hex ); // assigns the correct occupier for the map nodes
	}
	unit_data_buffer_needs_update = true;
}

void InitArmyStuff()
{
	LoadArmyGraphicStuff();
	InitUnitDataBuffer();
	for (int i = 0; i < 183; i++)
	{
		test_armies[i].index = i;
		test_armies[i].base_sprite = (ArmyBaseSprite)(i % 183);
		test_armies[i].position_hex = i;
		test_armies[i].movement = 5;
		test_armies[i].hits_max = 15;
		test_armies[i].hits_current = 15;
		test_armies[i].strength = 12;
		test_armies[i].faction = 0;
		test_armies[i].wounded_soldiers = 0;
		test_armies[i].dead_soldiers = 0;
		test_armies[i].armor = 15;
		test_armies[i].move_done = false;
		test_armies[i].action_done = false;
		test_armies[i].dead = false;
		test_armies[i].draw = true;
		RandomName(&test_armies[i]);

		// MoveArmyToNewHex(i, i); // assigns the correct occupier for the map nodes
	}
}

void TakeDownArmyStuff()
{
	SaveUnitLocationsToFile("autosave.hws");
	FreeUnitDataBuffer();
}
