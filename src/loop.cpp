// ImGui helper functions
// Helper to display a little (?) mark which shows a tooltip when hovered.
static void ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
///

void DrawDQK(int palette_phase, bool phase_shift)
{
	glBindVertexArray(ipi_vao);
	glUseProgram(ipi_sp);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, dqk.texture);
	//glBindTexture(GL_TEXTURE_2D, delagardie_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, dqk.palette_texture);

	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 1, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, &(dqk.palette[0]) );
	//glTexSubImage2D(GL_TEXTURE_2D, 0, palette_phase, 0, 256 - palette_phase, 1, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, &(dqk.palette[0]) );
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, palette_phase, 1, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, &(dqk.palette[1024 - palette_phase * 4]) );

	GLint input_texture_loc = glGetUniformLocation( ipi_sp, "input_texture");
	glUniform1i( input_texture_loc, 0);

	GLint palette_texture_loc = glGetUniformLocation( ipi_sp, "palette_texture");
	glUniform1i( palette_texture_loc, 1);

	GLint time_loc = glGetUniformLocation( ipi_sp, "time");
	glUniform1ui( time_loc, master_timer);

	GLint phase_shift_loc = glGetUniformLocation( ipi_sp, "phase_shift");
	glUniform1i( phase_shift_loc, phase_shift );

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);
	glBindVertexArray(0);
}

void Step(double delta)
{
	int camera_move_x = 0;
	int camera_move_y = 0;
	bool zoom_adjusted = false;

	bool left_clicked = false;

	int ui_hovered = 0;
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) { ui_hovered = 1; }

	SDL_Event evt;
	const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);;

	SDL_GetMouseState( &(mouse_pos_screen.x), &(mouse_pos_screen.y) );

	while (SDL_PollEvent(&evt) != 0)
	{
		ImGui_ImplSDL2_ProcessEvent(&evt);

		if (evt.type == SDL_QUIT)
		{
			game_running = false;
		}

		else if (evt.type == SDL_KEYDOWN)
		{
			if (evt.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			{
				if (show_main_menu) { game_running = false; }
				else { show_main_menu = true; }
			}
			else if (evt.key.keysym.scancode == SDL_SCANCODE_F1)
			{
				show_debug_ui = !show_debug_ui;
			}
			else if (evt.key.keysym.scancode == SDL_SCANCODE_Q)
			{
				//game_running = false;
			}
		}
		if (ui_hovered == 0)
		{
			if (evt.type == SDL_MOUSEWHEEL)
			{
				//ivec2 temphehe = { (int)mouse_pos_map.x, (int)mouse_pos_map.y };
				camera.IncreaseSizeBy( 1.0f + evt.wheel.y * -0.2f, mouse_pos_map );
				zoom_adjusted = true;		
			}
			else if ( evt.type == SDL_MOUSEBUTTONDOWN )
			{
				if ( evt.button.button == SDL_BUTTON_LEFT)
				{
					left_clicked = true;
				}
			}
			else if ( evt.type == SDL_MOUSEMOTION )
			{
				if (!mouse_first_move_done)
				{
					mouse_first_move_done = true;
				}
			}
		}

	}

	

	if (mouse_first_move_done)
	{
		if ( mouse_pos_screen.x == 0 ) { camera_move_x = -1; }
		if ( mouse_pos_screen.y == 0 ) { camera_move_y = -1; }
		if ( mouse_pos_screen.x == (int)(viewport_size.x)-1 ) { camera_move_x = 1; }
		if ( mouse_pos_screen.y == (int)(viewport_size.y)-1 ) { camera_move_y = 1; }
	}

	if ( ImGui::GetIO().WantCaptureKeyboard == false )
	{
		if (keyboard_state[SDL_SCANCODE_W] | keyboard_state[SDL_SCANCODE_UP]) { camera_move_y = -1; }
		if (keyboard_state[SDL_SCANCODE_S] | keyboard_state[SDL_SCANCODE_DOWN]) { camera_move_y = 1; }
		if (keyboard_state[SDL_SCANCODE_A] | keyboard_state[SDL_SCANCODE_LEFT]) { camera_move_x = -1; }
		if (keyboard_state[SDL_SCANCODE_D] | keyboard_state[SDL_SCANCODE_RIGHT]) { camera_move_x = 1; }		
	}

	float zoom_level = (float)camera.Size().x / viewport_size.x;

	float camerah_x = camera.Min().x + (float)camera_move_x * (float)(delta) * 1000.0f * 1.0f * zoom_level;
	float camerah_y = camera.Min().y + (float)camera_move_y * (float)(delta) * 1000.0f * 1.0f * zoom_level;

	camera.MoveToNewMin( { camerah_x, camerah_y } );

	mouse_pos_map.x = ( (float)mouse_pos_screen.x * zoom_level + camera.Min().x );
	mouse_pos_map.y = ( (float)mouse_pos_screen.y * zoom_level + camera.Min().y );
	highlighted_hex = GetMouseOveredHex(mouse_pos_map);

	HexesWithinCameraBounds();

	if (unit_data_buffer_needs_update)
	{
		UpdateUnitDataBuffer();
	}

	if ( left_clicked && selected_army != NULL && highlighted_hex > 0)
	{
		//selected_army->position_hex = highlighted_hex;
		MoveArmyToNewHex( selected_army->index, highlighted_hex );
		PlaySfx(SFX_UNIT_MOVE);
		selected_army = NULL;
		unit_data_buffer_needs_update = true;
		path_edges_size = 0;
		draw_path = false;
	}
	else if ( left_clicked && selected_army == NULL )
	{
		for (int i = 0; i < 183; i++)
		{
			if ( test_armies[i].position_hex == highlighted_hex )
			{
				selected_army = &test_armies[i];
				PlaySfx(SFX_UI_CLICK_A);
			}
		}
	}

	if ( selected_army != NULL )
	{
		if ( selected_army->position_hex != highlighted_hex)
		{
			if ( current_path.x != selected_army->position_hex || current_path.y != highlighted_hex )
			{
				if ( FindPath(selected_army->position_hex, highlighted_hex) < 0 )
				{
					draw_path = false;
				}
				else
				{
					draw_path = true;
				}
				current_path.x = selected_army->position_hex;
				current_path.y = highlighted_hex;
			}
		}
	}



	static float music_gain = 0.05f;
	static float master_gain = 1.0f;

	static bool show_demo_window = false;
	static bool draw_dqk = false;
	static bool cycle_palette_phase = false;
	static int current_palette_phase = 0;

	static int text_string_count = 0;
	static float text_string_loc_x = 200.0f;
	static float text_string_loc_y = 124.0f;

	static char str0[48] = "Victimae Paschali Laudes";

	static char loadmap_str[128] = "data/maps/terrain_perke_no.csv";



	static int show_edge = 0;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();

	if (selected_army != NULL)
	{
		ImGui::Begin("SELECTED UNIT");
		ImGui::Text("%s", selected_army->name);
		ImGui::Text("Strength: %d", selected_army->strength);
		ImGui::Text("Moves: %d / %d", selected_army->movement_points_current, selected_army->movement_points_max);
		ImGui::Text("Hits:  %d / %d", selected_army->hits_current, selected_army->hits_max);
		ImGui::Separator();
		ImGui::Text("Base sprite: %d", selected_army->base_sprite);
		ImGui::InputInt("change", (int*)&(selected_army->base_sprite) );
		unit_data_buffer_needs_update = true;

		ImGui::End();
	}

	static ivec2 hex_one = { 40, 35 };
	static ivec2 hex_two = { 51, 23 };
	static int32_t hex_distance_result = 0;
	static int32_t some_temp_low_score = 0;
	{
		ImGui::Begin("Hex Distance");
		ImGui::InputInt2("hex one", (int*)&hex_one);
		ImGui::InputInt2("hex two", (int*)&hex_two);
		//if (ImGui::Button("Calculate hex distance")) { hex_distance_result = CalculateHexDistance(hex_one, hex_two); }
		//ImGui::Text("hex distance result: %d", hex_distance_result);
		if ( ImGui::Button("Find Path") ) { some_temp_low_score = FindPath( hex_one.y * map_width + hex_one.x, hex_two.y * map_width + hex_two.x); draw_path = true; }
		ImGui::Text("Some temp low score %d", some_temp_low_score);
		ImGui::InputFloat("H weight", &pathfind_weight_h); ImGui::SameLine();
		ShowHelpMarker("Heuristic weight for pathfind algorithm.");

		ImGui::End();

	}

	if (show_debug_ui)
	{
		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window) { ImGui::ShowDemoWindow(&show_demo_window); }

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("delagardiebug");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Checkbox("Demo", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::SameLine();
			ImGui::Checkbox("Settings###SETTINGS_CHECKBOX", &show_settings_window);
			ImGui::SameLine();
			ImGui::Checkbox("Memory", &show_memory_window);
#if 0
			ImGui::SameLine();			
			ImGui::Checkbox("8bit index palette pic", &draw_dqk);
			ImGui::Checkbox("Cycle palette phase", &cycle_palette_phase);
			ImGui::SliderInt("phase", &current_palette_phase, 0, 256);
#endif
			ImGui::Separator();

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			// ImGui::Text("Time: %d", master_timer);
			ImGui::SliderFloat("master gain", &master_gain, 0.0f, 2.0f, "%.4f", 1.0f);
			ImGui::SliderFloat("music gain", &music_gain, 0.0f, 2.0f, "%.4f", 1.0f);

			static int sfx_number = 0;
			ImGui::SliderInt("sfx", &sfx_number, 0, 1);
			ImGui::SameLine();
			ImGui::Text("%d ", sfx[sfx_number].id);
			ImGui::SameLine();
			if ( ImGui::Button("woah!") ) { cm_play(sfx[sfx_number].src); }

#if 0
			ImGui::Separator();
			ImGui::InputText("text", str0, IM_ARRAYSIZE(str0));
			ImGui::SliderInt("text c", &text_string_count, 0, 24);
			ImGui::SliderFloat("text x", &text_string_loc_x, -500.0f, 2000.0f, "%.3f", 1.0f);
			ImGui::SliderFloat("text y", &text_string_loc_y, -200.0f, 1200.0f, "%.3f", 1.0f);
#endif
			ImGui::Separator();

			// ImGui::Text("Camera %.2f %.2f %.2f %.2f", camera.Min().x, camera.Min().y, camera.Max().x, camera.Max().y);
			ImGui::Text("zoom_level: %.2f", zoom_level);
			ImGui::Text("CameraSize %.2f %.2f", camera.Size().x, camera.Size().y);
			ImGui::Text("CameraOrigo %.2f %.2f", camera.Origo().x, camera.Origo().y);
/*
			ImGui::Text("tile column min: %d max: %d", dumb_dbg.tile_column_min, dumb_dbg.tile_column_max);
			ImGui::Text("tile row min: %d max: %d", dumb_dbg.tile_row_min, dumb_dbg.tile_row_max);
			ImGui::Text("hexes to draw count %d", hexes_to_draw_count);
			ImGui::Text("MOUSE: X: %d, Y: %d MAP: X: %.2f Y: %.2f", mouse_pos_screen.x, mouse_pos_screen.y, mouse_pos_map.x, mouse_pos_map.y);
*/
			ivec2 staggered_hex = { map_nodes[highlighted_hex].x, map_nodes[highlighted_hex].y };
			ivec2 slanted_hex = StaggeredToSlanted( staggered_hex );
			ImGui::Text("HIGHLIGHT HEX: %d (Staggered: X%d Y%d Slanted: X%d Y%d)", 
				highlighted_hex,
				map_nodes[highlighted_hex].x,
				map_nodes[highlighted_hex].y,
				slanted_hex.x,
				slanted_hex.y);
/*
			ImGui::Text("MapNode: %d, Neighbors: N: %d NE: %d SE: %d S: %d SW: %d NW: %d", 
				map_nodes[highlighted_hex].index,
				map_node_neighbours[highlighted_hex].n_north,
				map_node_neighbours[highlighted_hex].n_northeast,
				map_node_neighbours[highlighted_hex].n_southeast,
				map_node_neighbours[highlighted_hex].n_south,
				map_node_neighbours[highlighted_hex].n_southwest,
				map_node_neighbours[highlighted_hex].n_northwest
				);
			ImGui::Text("Neighbor # %d", map_node_neighbours[highlighted_hex].n_count);
*/

			ImGui::Separator();
			ImGui::Text("%s", dumb_debug_string);
			ImGui::Text("%s", dumb_debug_string2);
			//ImGui::Text("kalle len: %f ville len: %f", debug_kalle_len, debug_ville_len);

			ImGui::Separator();
			ImGui::Checkbox("Pathfind debug overlay", &draw_hex_debug_overlay);
			ImGui::Text("total in open set: %d", number_of_nodes_that_were_in_open_set_debug);
			ImGui::Text("open_set_count: %d", open_set_count);
			ImGui::Text("closed_set_count: %d", closed_set_count);
			ImGui::Separator();

			ImGui::InputText("loadmap", loadmap_str, IM_ARRAYSIZE(loadmap_str));
			ImGui::SameLine();
			if ( ImGui::Button("LOAD MAP") ) { int rc = LoadMapTerrainFromCSV(loadmap_str, map_size, map_data); }

#if 0
			ImGui::Separator();
			ImGui::InputInt("edge", &show_edge, 1, 10);
			ImGui::SameLine();
			ImGui::Text("start %d end %d cost %d", map_edges[show_edge].start_node_index, map_edges[show_edge].end_node_index, map_edges[show_edge].cost);
#endif
			ImGui::End();
		}
	}

	if (show_main_menu)
	{
		ImGui::SetNextWindowPos(ImVec2( viewport_size.x / 2.0f, viewport_size.y /2.0f ), 0, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2( 216.0f, 280.0f ));
		ImGui::Begin("Main Menu", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize );
		if (ImGui::Button("Continue###MAIN_MENU_CONTINUE", ImVec2(200,50))) { show_main_menu = !show_main_menu; }
		if (ImGui::Button("New Map###MAIN_MENU_NEW_MAP", ImVec2(200,50))) { /* NewMap(); */ }
		if (ImGui::Button("Settings###MAIN_MENU_SETTINGS", ImVec2(200,50))) { show_settings_window = !show_settings_window; }
		if (ImGui::Button("Debug Window###MAIN_MENU_DEBUG_WINDOW", ImVec2(200,50))) { show_debug_ui = !show_debug_ui; }
		if (ImGui::Button("Exit to OS###MAIN_MENU_EXIT", ImVec2(200,50))) { game_running = false; }
		ImGui::End();
	}

	static void* mm_ptr = (void*)(&test_armies[0]);
	static int inspector_line_count = 16;
	if (show_memory_window)
	{
		ImGui::Begin("Memory. &test_armies[0] (Can set this only in source code for now.)");
		ImGui::Separator();

		ImGui::PushFont(im_font_monofonto);

		for (int i = 0; i < inspector_line_count; i++)
		{
			ImGui::Text("%016lx     %02hhx %02hhx %02hhx %02hhx", (uint64_t)mm_ptr+i*16, *((uint8_t*)mm_ptr+i*16), *((uint8_t*)mm_ptr+i*16+1), *((uint8_t*)mm_ptr+i*16+2), *((uint8_t*)mm_ptr+i*16+3));
			ImGui::SameLine();
			ImGui::Text(" %02hhx %02hhx %02hhx %02hhx", *((uint8_t*)mm_ptr+i*16+4), *((uint8_t*)mm_ptr+i*16+5), *((uint8_t*)mm_ptr+i*16+6), *((uint8_t*)mm_ptr+i*16+7));
			ImGui::SameLine();
			ImGui::Text(" %02hhx %02hhx %02hhx %02hhx", *((uint8_t*)mm_ptr+i*16+8), *((uint8_t*)mm_ptr+i*16+9), *((uint8_t*)mm_ptr+i*16+10), *((uint8_t*)mm_ptr+i*16+11));
			ImGui::SameLine();
			ImGui::Text(" %02hhx %02hhx %02hhx %02hhx", *((uint8_t*)mm_ptr+i*16+12), *((uint8_t*)mm_ptr+i*16+13), *((uint8_t*)mm_ptr+i*16+14), *((uint8_t*)mm_ptr+i*16+15));

			if ( i % 4 == 3 )
			{
				ImGui::Separator();
			}
		}
		ImGui::PopFont();

		ImGui::InputInt("lines", &inspector_line_count, 4, 16);
		ImGui::InputInt("osote", (int*)((uint64_t)&mm_ptr), 16, 256);
		ImGui::SameLine(); ShowHelpMarker("Small adjusts to move pointer around,\nbigger adjusts to crash the program.");

		ImGui::End();
	}

	if (show_settings_window)
	{
		ImGui::Begin("Settings");
		ImGui::Checkbox("Fullscreen###SETTINGS_FULLSCREEN", &settings_temp.fullscreen);
		ImGui::Checkbox("Borderless###SETTINGS_BORDERLESS", &settings_temp.borderless);
		ImGui::Checkbox("Vsync###SETTINGS_VSYNC", &settings_temp.vsync);
		ImGui::InputInt("Width###SETTINGS_WIDTH", &settings_temp.screen_width);
		ImGui::InputInt("Height###SETTINGS_HEIGHT", &settings_temp.screen_height);
		ImGui::Separator();
		if (ImGui::Button("OK")) { SaveSettings(); }
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) { settings_temp = settings_confirmed; show_settings_window = !show_settings_window; }
		ImGui::SameLine();
		ImGui::Button("Apply Changes");
		ImGui::SameLine();
		if (ImGui::Button("Restore Defaults") ) { DefaultSettings(); }
		ImGui::Text("%s", settings_msg);
		ImGui::End();
	}

	cm_set_master_gain((double)master_gain);
	cm_set_gain(music_src, (double)music_gain);

	for (int i = 0; i < text_string_count; i++)
	{
		AddStringToBeDrawn(text_string_loc_x, text_string_loc_y + 64.0f * (float)i, str0, IM_ARRAYSIZE(str0));
	}
	




	glClearColor(223.0f/255.0f, 190.0f/255.0f, 138.0f/255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	DrawHexes();
	glDisable(GL_DEPTH_TEST);
	//DrawEdgeAsArrow( map_edges[show_edge].start_node_index, map_edges[show_edge].end_node_index, map_edges[show_edge].direction * 60.0f );

	if (draw_path)
	{
		for (int i = 0; i <= path_edges_size; i++)
		{
			DrawEdgeAsArrow( map_edges[ path_edges[i] ].start_node_index, map_edges[ path_edges[i] ].end_node_index, map_edges[ path_edges[i] ].direction * 60.0f );
		}
		if (draw_hex_debug_overlay)
		{
			UpdateHexMapBuffersForDebugOverlay();
			DrawHexDebugOverlay();		
		}
		
	}

	glEnable(GL_DEPTH_TEST);
	DrawArmies(unit_data_count);
	glDisable(GL_DEPTH_TEST);

	

	DrawStrings();

	if (draw_dqk)
	{
		if (cycle_palette_phase)
		{
			current_palette_phase = (master_timer / 100 ) % 256;
		}
		DrawDQK(current_palette_phase, cycle_palette_phase);
	}

	// Rendering
	ImGui::Render();
	//SDL_GL_MakeCurrent(window, gl_context); // ???
	//glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(window);
}


void MainLoop()
{
	uint64_t last_time_ticks = SDL_GetPerformanceCounter() - program_start_time_ticks;
	uint64_t delta_container[3] = {0,0,0};

	StartMusic();

	while(game_running)
	{
		uint64_t this_time_ticks = SDL_GetPerformanceCounter() - program_start_time_ticks;
		uint64_t delta_ticks = this_time_ticks - last_time_ticks;
		master_timer = (uint32_t)(1000.0 * ((double)this_time_ticks / (double)perf_freq));

#if 0
		delta_container[2] = delta_container[1];
		delta_container[1] = delta_container[0];
		delta_container[0] = delta_ticks;

		uint64_t delta_average = (delta_container[0] + delta_container[1] + delta_container[2]) / 3;
		double delta_sec = (double)delta_average / (double)perf_freq;
#else
		double delta_sec = (double)delta_ticks / (double)perf_freq;
#endif
		Step(delta_sec);

		last_time_ticks = this_time_ticks;
	}
}
