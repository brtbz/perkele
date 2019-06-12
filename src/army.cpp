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

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glBindVertexArray(0);
}

void DrawMovingArmy(int32_t a)
{

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

		unit_data[i] = unit_datum;
	}

	glBindBuffer(GL_ARRAY_BUFFER, unit_data_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, unit_data_count * sizeof(ivec4), (const GLvoid*)&(unit_data[0]) );
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//int division_count = 8;
	//ivec4* unit_data;
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

void Kill(Army *a)
{

}

void Attack(Army *attacker, Army *defender)
{
	float attacker_strength = (float)attacker->strength;
	float defender_strength = (float)defender->strength;

	float attacker_bonus = 0.0f;
	float defender_bonus = 0.2f;

	attacker_strength += attacker_bonus;
	defender_strength += defender_bonus;

	int attacker_roll = MWC % 20;
	int defender_roll = MWC % 20;

	attacker_strength += attacker_roll;
	defender_strength += defender_roll;

	if (attacker_strength > defender_strength)
	{
		// attacker wins!
		defender->hits_current -= 2;
	}
	else
	{
		// defender wins!
		attacker->hits_current -= 1;
	}

	if (attacker->hits_current <= 0)
	{
		Kill(attacker);
	}
	if (defender->hits_current <= 0)
	{
		Kill(defender);
	}
}

void MoveArmyToNewHex(int32_t army, int32_t hex)
{
	map_nodes[ test_armies[army].position_hex ].occupier = -1;
	test_armies[army].position_hex = hex;
	map_nodes[ test_armies[army].position_hex ].occupier = army;
}

void ArmyMover(int32_t army, int32_t *path, int32_t path_size, uint32_t allowed_time)
{
	// I guess most of the movement code is handled here in c++. 
	// glsl is just told: now draw unit between these two hexes with transition being x percent complete
	// only one unit moves at a time, so I guess there's no point in adding that 'between hexes' capability to the normal unit drawing code
	// when unit starts to move, stop drawing it in normal way, and resume normal operations once it stops.
}

bool HexIsFree(int32_t hex)
{
	return map_nodes[ hex ].occupier == -1 ? true : false;
}

void ArrangePiecesAroundOnTheBoardJohnImOnlyTesting()
{
	MoveArmyToNewHex(177, 3763);
	MoveArmyToNewHex(176, 4148);
	MoveArmyToNewHex(174, 30 * 128 + 55);
	test_armies[177].base_sprite = GOBLIN_BATHROBE_RED;

	vec2 temp_hehe = {-1.0f, -1.0f};
	vec2 temp_vec2 = { 860.0f, 650.0f };
	camera.IncreaseSizeBy(0.42f, temp_hehe);
	camera.MoveToNewMin(temp_vec2);
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
		test_armies[i].movement_points_max = 5;
		test_armies[i].movement_points_current = 5;
		test_armies[i].hits_max = 10;
		test_armies[i].hits_current = 10;
		test_armies[i].strength = 12;
		RandomName(&test_armies[i]);
	}
}

void TakeDownArmyStuff()
{
	FreeUnitDataBuffer();
}