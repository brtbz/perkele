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

void DrawHits(int32_t hex, vec3 color, int hits)
{
	glBindVertexArray(hits_vao);
	glUseProgram(hits_sp);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hits_texture);

	GLint input_texture_loc = glGetUniformLocation( hits_sp, "input_texture" );
	glUniform1i( input_texture_loc, 0 );

	GLint sprite_sheet_size_loc = glGetUniformLocation( hits_sp, "sprite_sheet_size" );
	glUniform2f( sprite_sheet_size_loc, 64.0f, 64.0f );

	GLint screen_size_loc = glGetUniformLocation( hits_sp, "screen_size" );
	glUniform2f( screen_size_loc, viewport_size.x, viewport_size.y );

	GLint camera_loc = glGetUniformLocation( hits_sp, "camera" );
	glUniform4f( camera_loc, camera.Min().x, camera.Min().y, camera.Max().x, camera.Max().y );

	GLint map_grid_size_loc = glGetUniformLocation( hits_sp, "map_grid_size" );
	glUniform2i( map_grid_size_loc, map_width, map_height );

	GLint hex_loc = glGetUniformLocation( hits_sp, "hex");
	glUniform1i( hex_loc, hex );

	GLint hits_loc = glGetUniformLocation( hits_sp, "hits");
	glUniform1i( hits_loc, hits);

	// temp hopefully
	GLint color_loc = glGetUniformLocation( hits_sp, "faction_color");
	glUniform3f(color_loc, color.r, color.g, color.b);

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

	//glDrawArraysInstanced(GL_POINTS, 0, 1, ARMY_COUNT_MAX);
	glDrawArraysInstanced(GL_POINTS, 0, 1, active_armies_count);

	glUseProgram(0);
	glBindVertexArray(0);
}

void InitUnitDataBuffer()
{
	// unit_data = (ivec4*)malloc( unit_limit * sizeof(ivec4) );
	unit_data = (ivec4*)malloc( ARMY_COUNT_MAX * sizeof(ivec4) );
}

void FreeUnitDataBuffer()
{
	free(unit_data);
}

void UpdateUnitDataBuffer()
{
	active_armies_count = 0;

	for (int i = 0; i < ARMY_COUNT_MAX; i++)
	{
		if (all_armies[i].active == true)
		{
			
			ivec4 unit_datum = 
			{ 
				all_armies[i].position_hex,
				all_armies[i].base_sprite,
				0,//test_divisions[i].unit_type,
				0,//test_divisions[i].unit_size
			};
			if (all_armies[i].draw == false)
			{
				unit_datum.y = 0;
			}

			unit_data[active_armies_count] = unit_datum;

			active_armies_count++;
		}
	}
/*
	for (int i = 0; i < ARMY_COUNT_MAX; i++)
	{
		ivec4 unit_datum = 
		{ 
			all_armies[i].position_hex,
			all_armies[i].base_sprite,
			0,//test_divisions[i].unit_type,
			0,//test_divisions[i].unit_size
		};
		if (all_armies[i].draw == false)
		{
			unit_datum.y = 0;
		}

		unit_data[i] = unit_datum;
	}
*/
	glBindBuffer(GL_ARRAY_BUFFER, unit_data_buffer);
	// glBufferSubData(GL_ARRAY_BUFFER, 0, unit_data_count * sizeof(ivec4), (const GLvoid*)&(unit_data[0]) );
	glBufferSubData(GL_ARRAY_BUFFER, 0, active_armies_count * sizeof(ivec4), (const GLvoid*)&(unit_data[0]) );
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
	// glBufferData(GL_ARRAY_BUFFER, sizeof(int) * unit_limit, NULL, GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ivec4) * ARMY_COUNT_MAX, NULL, GL_DYNAMIC_DRAW); // TODO: investigate what happens here

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

void LoadHits() // hitpoint overlay
{
	int w, h;
	hits_texture = LoadTexture("data/gfx/hits.png", &w, &h);
	GLuint hits_vs = NewShader(GL_VERTEX_SHADER, "data/shaders/hits-vert.glsl");
	GLuint hits_fs = NewShader(GL_FRAGMENT_SHADER, "data/shaders/hits-frag.glsl");
	hits_sp = NewProgram(hits_vs, hits_fs);

	GLfloat vertex_data[] = {
		// x     y       u    v
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
	};

    glGenVertexArrays(1, &hits_vao);
    glBindVertexArray(hits_vao);
    glGenBuffers(1, &hits_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, hits_vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

	GLint position_loc = glGetAttribLocation(hits_sp, "position");
	glEnableVertexAttribArray(position_loc);
	glVertexAttribPointer( position_loc, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)0 );

	GLint uv_coord_loc = glGetAttribLocation(hits_sp, "uv_coord");
	glEnableVertexAttribArray( uv_coord_loc );
	glVertexAttribPointer( uv_coord_loc, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)));

	// unit location buffer (int? or use that ivec4 that others are using already?)

	// faction color buffer (vec3?)

	// unit hit point buffer (int)

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
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unit_limit * 3, NULL, GL_DYNAMIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ARMY_COUNT_MAX * 3, NULL, GL_DYNAMIC_DRAW);

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
	strncpy(a->name, names[random_name], 47);
	a->name[47] = '\0';
}

void SetArmyName(Army *a, const char *name)
{
	strncpy(a->name, name, 48);
	a->name[47] = '\0';
}

void MoveArmyToNewHex(int32_t army, int32_t hex)
{
	map_nodes[ all_armies[army].position_hex ].occupier = -1;
	all_armies[army].position_hex = hex;
	map_nodes[ all_armies[army].position_hex ].occupier = army;
}

void DeactivateDead()
{
	for (int i = 0; i < ARMY_COUNT_MAX; i++)
	{
		if (all_armies[i].dead == true)
		{
			all_armies[i].active = false;
		}
	}
}

void CleanInactiveArmiesFromBoard()
{
	for (int i = 0; i < ARMY_COUNT_MAX; i++)
	{
		if (all_armies[i].active == false)
		{
			MoveArmyToNewHex(i, -13);
		}
	}
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
	all_armies[army].draw = false;
	army_moving = true;
	movement_starts = master_timer;
	path_position = pathfinder->path_edges_size;
	prev_hex = start;
	moving_to_hex = map_edges[ pathfinder->path_edges[path_position] ].end_node_index;
}

void AdvanceArmyMoveAnimation(int32_t army)
{
	movement_timer = master_timer;
	if ( movement_starts + ms_per_hex < movement_timer )
	{
		path_position--;
		allowed_steps--;

		if (path_position < 0)
		{
			//end
			army_moving = false;
			all_armies[army].move_done = true;
			all_armies[army].draw = true;

			MoveArmyToNewHex(army, pathfinder->current_path.y);
			draw_path = false;
			unit_data_buffer_needs_update = true;
			allowed_steps = 512;
		}
		else if (allowed_steps <= 0)
		{
			//end
			army_moving = false;
			all_armies[army].move_done = true;
			all_armies[army].draw = true;
			MoveArmyToNewHex(army, moving_to_hex);
			draw_path = false;
			unit_data_buffer_needs_update = true;
			allowed_steps = 512;
		}
		else
		{
			movement_starts = master_timer;
			prev_hex = moving_to_hex;
			moving_to_hex = map_edges[ pathfinder->path_edges[path_position] ].end_node_index; // this line looks pretty horrible
		}
	}
}

void BeginArmyAttackAnimation(int32_t army, int32_t start, int32_t end)
{
	// attack animation is a move animation that goes nowhere, just quickly bumps against the target
	moving_army = army;
	all_armies[army].draw = false;
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
		all_armies[army].action_done = true;
		all_armies[army].draw = true;

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
	data_size = ARMY_COUNT_MAX * sizeof(Army);

	uint8_t *data = (uint8_t*)malloc(data_size);

	memcpy(data, &all_armies[0], data_size);

	FILE *fp = fopen(file_path, "wb");

	fwrite(data, data_size, 1, fp);

	fclose(fp);

	free(data);
}

int LoadUnitLocationsFromFile(const char* file_path)
{
	int expected_data_size = ARMY_COUNT_MAX * sizeof(Army);
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

	if (file_size != expected_data_size)
	{
		// old version of "savefile", can't use it
		fclose(fp);
		return -2;
	}

	uint8_t *data = (uint8_t*)malloc(file_size);

	fread(data, file_size, 1, fp);

	memcpy(&all_armies[0], data, file_size);

	fclose(fp);
	free(data);

	return 0;
}

//                                                   SPRITE, MOVE, HP, ARMOR, STRENGTH, SOUND
ArmyTemplate army_template_surfer = {            SURFER_BOY,    6, 15,     9,       15, SFX_SURFER_BATTLE_CRY};
ArmyTemplate army_template_goblin = { GOBLIN_BATHROBE_GREEN,    5, 15,    12,        9, SFX_GOBLIN_ROAR };

void ApplyArmyTemplate(ArmyTemplate *at, Army *a)
{
	a->base_sprite = at->base_sprite;
	a->movement = at->movement;
	a->hits_max = at->hits_max;
	a->hits_current = at->hits_max;
	a->armor = at->armor;
	a->strength = at->strength;
	a->attack_sound = at->attack_sound;
}

void NewActiveArmy(Army *a, const char* name, Faction faction, ArmyBaseSprite abs, int32_t position)
{
	SetArmyName(&all_armies[ a->index ], name);
	all_armies[ a->index ].base_sprite = abs;
	// MoveArmyToNewHex( a->index, position );
	all_armies[ a->index ].position_hex = position;
	all_armies[ a->index ].faction = faction;
	a->active = true;
	a->draw = true;
}

void ArrangePiecesAroundOnTheBoardJohnImOnlyTesting()
{
	NewActiveArmy( &all_armies[0], "1st Royal Goblin Battalion", FACTION_GOBLINS, GOBLIN_BATHROBE_BLUE, 4532 );
	NewActiveArmy( &all_armies[1], "2nd Royal Goblin Battalion", FACTION_GOBLINS, GOBLIN_BATHROBE_GREEN, 4533 );
	NewActiveArmy( &all_armies[2], "3rd Royal Goblin Battalion", FACTION_GOBLINS, GOBLIN_BATHROBE_PINK, 4534 );
	NewActiveArmy( &all_armies[3], "4th Royal Goblin Battalion", FACTION_GOBLINS, GOBLIN_BATHROBE_BLUE, 4535 );
	NewActiveArmy( &all_armies[4], "5th Royal Goblin Battalion", FACTION_GOBLINS, GOBLIN_BATHROBE_RED, 4536 );

	NewActiveArmy( &all_armies[5], "1st Surfer Battalion 'Sweden'", FACTION_SURFERS, SURFER_BOY, 3891 );
	NewActiveArmy( &all_armies[6], "2nd Surfer Battalion 'Norway'", FACTION_SURFERS, SURFER_BOY, 3892 );
	NewActiveArmy( &all_armies[7], "3rd Surfer Battalion 'Stockholm'", FACTION_SURFERS, SURFER_BOY, 3893 );
	NewActiveArmy( &all_armies[8], "4th Surfer Battalion 'Narvik'", FACTION_SURFERS, SURFER_BOY, 3894 );
	NewActiveArmy( &all_armies[9], "5th Surfer Battalion 'Trondheim'", FACTION_SURFERS, SURFER_BOY, 3895 );

	for (int i = 5; i < 10; i++)
	{
		ApplyArmyTemplate( &army_template_surfer, &all_armies[i] );
	}

	for (int i = 0; i < ARMY_COUNT_MAX; i++)
	{
		if ( all_armies[i].active == true )
		{
			unit_data_count++;
		}
	}

	vec2 temp_hehe = {-1.0f, -1.0f};
	vec2 temp_vec2 = { 860.0f, 650.0f };
	camera.IncreaseSizeBy(0.42f, temp_hehe);
	camera.MoveToNewMin(temp_vec2);

	for ( int i = 0; i < map_size; i++)
	{
		map_nodes[i].occupier = -1;
	}

	LoadUnitLocationsFromFile("autosave.hws");
	for (int i = 0; i < ARMY_COUNT_MAX; i++)
	{
		MoveArmyToNewHex(i, all_armies[i].position_hex ); // assigns the correct occupier for the map nodes
	}

	CleanInactiveArmiesFromBoard();
	unit_data_buffer_needs_update = true;
}

void NewDefaultBlankArmy(Army *a, int index)
{
	a->index = index;
	a->base_sprite = PUDDING_GOLEM;
	a->position_hex = index;
	a->movement = 5;
	a->hits_max = 15;
	a->hits_current = 15;
	a->strength = 12;
	a->faction = 0;
	a->wounded_soldiers = 0;
	a->dead_soldiers = 0;
	a->armor = 15;
	a->zone_of_control = 1;
	a->move_done = false;
	a->action_done = false;
	a->dead = false;
	a->draw = true;
	a->active = false;
	a->attack_sound = SFX_GOBLIN_ROAR;

	RandomName(a);
}

void InitArmyStuff()
{
	LoadArmyGraphicStuff();
	InitUnitDataBuffer();
	for (int i = 0; i < ARMY_COUNT_MAX; i++)
	{
		NewDefaultBlankArmy(&all_armies[i], i);
	}

	LoadHits();
}

void ResetArmiesToDefaultTemporaryPleaseDelete()
{
	for (int i = 0; i < ARMY_COUNT_MAX; i++)
	{
		NewDefaultBlankArmy(&all_armies[i], i);
	}

	NewActiveArmy( &all_armies[0], "1st Royal Goblin Battalion", FACTION_GOBLINS, GOBLIN_BATHROBE_BLUE, 4532 );
	NewActiveArmy( &all_armies[1], "2nd Royal Goblin Battalion", FACTION_GOBLINS, GOBLIN_BATHROBE_GREEN, 4533 );
	NewActiveArmy( &all_armies[2], "3rd Royal Goblin Battalion", FACTION_GOBLINS, GOBLIN_BATHROBE_PINK, 4534 );
	NewActiveArmy( &all_armies[3], "4th Royal Goblin Battalion", FACTION_GOBLINS, GOBLIN_BATHROBE_BLUE, 4535 );
	NewActiveArmy( &all_armies[4], "5th Royal Goblin Battalion", FACTION_GOBLINS, GOBLIN_BATHROBE_RED, 4536 );

	NewActiveArmy( &all_armies[5], "1st Surfer Battalion 'Sweden'", FACTION_SURFERS, SURFER_BOY, 3891 );
	NewActiveArmy( &all_armies[6], "2nd Surfer Battalion 'Norway'", FACTION_SURFERS, SURFER_BOY, 3892 );
	NewActiveArmy( &all_armies[7], "3rd Surfer Battalion 'Stockholm'", FACTION_SURFERS, SURFER_BOY, 3893 );
	NewActiveArmy( &all_armies[8], "4th Surfer Battalion 'Narvik'", FACTION_SURFERS, SURFER_BOY, 3894 );
	NewActiveArmy( &all_armies[9], "5th Surfer Battalion 'Trondheim'", FACTION_SURFERS, SURFER_BOY, 3895 );

	for (int i = 5; i < 10; i++)
	{
		ApplyArmyTemplate( &army_template_surfer, &all_armies[i] );
	}

	unit_data_count = 0;

	for (int i = 0; i < ARMY_COUNT_MAX; i++)
	{
		if ( all_armies[i].active == true )
		{
			unit_data_count++;
		}
	}

	for ( int i = 0; i < map_size; i++)
	{
		map_nodes[i].occupier = -1;
	}

	for (int i = 0; i < ARMY_COUNT_MAX; i++)
	{
		MoveArmyToNewHex(i, all_armies[i].position_hex ); // assigns the correct occupier for the map nodes
	}

	CleanInactiveArmiesFromBoard();
	unit_data_buffer_needs_update = true;
}

void TakeDownArmyStuff()
{
	SaveUnitLocationsToFile("autosave.hws");
	FreeUnitDataBuffer();
}
