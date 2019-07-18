void Step(double delta)
{
	int camera_move_x = 0;
	int camera_move_y = 0;
	bool zoom_adjusted = false;

	bool left_clicked = false;
	bool right_clicked = false;

	int ui_hovered = 0;
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) { ui_hovered = 1; }
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	SDL_Event evt;
	const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);;

	while (SDL_PollEvent(&evt) != 0)
	{
		ImGui_ImplSDL2_ProcessEvent(&evt);

		if (evt.type == SDL_QUIT)
		{
			game_running = false;
		}
		/*
		// Can't use this yet, because the camera doesn't know what to do when viewport size or aspect ratio changes
		else if (evt.type == SDL_WINDOWEVENT)
		{
			if (evt.window.event == SDL_WINDOWEVENT_RESIZED)
			{				
				int w, h;
				SDL_GL_GetDrawableSize(window, &w, &h);
				viewport_size.x = (float)w;
				viewport_size.y = (float)h;				
			}
		}
		*/

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
		}
		if (ui_hovered == 0)
		{
			if (evt.type == SDL_MOUSEWHEEL)
			{
				camera.IncreaseSizeBy( 1.0f + evt.wheel.y * -0.2f, mouse_pos_map );
				zoom_adjusted = true;		
			}
			else if ( evt.type == SDL_MOUSEBUTTONDOWN )
			{
				if ( evt.button.button == SDL_BUTTON_LEFT)
				{
					left_clicked = true;
				}
				else if ( evt.button.button == SDL_BUTTON_RIGHT)
				{
					right_clicked = true;
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

	SDL_GetMouseState( &(mouse_pos_screen.x), &(mouse_pos_screen.y) );

	if (mouse_edge_scroll && mouse_first_move_done)
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

		if (keyboard_state[SDL_SCANCODE_E] | keyboard_state[SDL_SCANCODE_HOME])
		{
			camera.IncreaseSizeBy(1.0f + (float)delta * -1.2f, mouse_pos_map);
			zoom_adjusted = true;
		}
		if (keyboard_state[SDL_SCANCODE_Q] | keyboard_state[SDL_SCANCODE_END])
		{
			camera.IncreaseSizeBy(1.0f + (float)delta * 1.2f, mouse_pos_map);
			zoom_adjusted = true;
		}
	}

	float zoom_level = (float)camera.Size().x / viewport_size.x;

	float camerah_x = camera.Min().x + (float)camera_move_x * (float)(delta) * 1000.0f * 1.0f * zoom_level;
	float camerah_y = camera.Min().y + (float)camera_move_y * (float)(delta) * 1000.0f * 1.0f * zoom_level;

	camera.MoveToNewMin( { camerah_x, camerah_y } );

	mouse_pos_map.x = ( (float)mouse_pos_screen.x * zoom_level + camera.Min().x );
	mouse_pos_map.y = ( (float)mouse_pos_screen.y * zoom_level + camera.Min().y );
	highlighted_hex = GetMouseOveredHex(mouse_pos_map);

	int custom_cursor_should_be = 0;
	// 0 arrow, 1 swords 
	// Use SDL_SetCursor only once per frame to avoid flickering 
	// (TODO?) keep track of current value and use SDL_SetCursor only when it changes?

	HexesWithinCameraBounds();

	if (unit_data_buffer_needs_update)
	{
		UpdateUnitDataBuffer();
	}

	if (army_moving)
	{
		AdvanceArmyMoveAnimation(moving_army);
	}

	if (army_attacking)
	{
		AdvanceArmyAttackAnimation(moving_army);
	}

	if (selected_army != NULL && selected_army->move_done == true && selected_army->action_done == true)
	{
		selected_army = NULL;
	}

	if (!army_moving && !army_attacking) // disable UI while moving unit around the map. is that overkill?
	{
		if ( right_clicked && selected_army != NULL)
		{
			if (selected_army->move_done == true || selected_army->action_done == true)
			{
				selected_army->move_done = true;
				selected_army->action_done = true;
			}
			selected_army = NULL;
			draw_path = false;
			ClearPaths(pathfinder);
		}
		else if ( left_clicked && selected_army != NULL )
		{
			if (HexIsFree(highlighted_hex) && selected_army->move_done == false)
			{
				if (ignore_move_rules || HexIsInReachableNodes(highlighted_hex))
				{
					BeginArmyMoveAnimation(selected_army->index, selected_army->position_hex, highlighted_hex);
					PlaySfx(SFX_UNIT_MOVE);
					unit_data_buffer_needs_update = true;
					path_edges_size = 0;
					draw_path = false;
				}
				else
				{
					PlaySfx(SFX_UI_CLICK_ERROR);
				}
			}
			else if ( selected_army->action_done == false && HexesAreNeighbours( selected_army->position_hex, highlighted_hex) && map_nodes[highlighted_hex].occupier > -1 )
			{
				defenders_hex = highlighted_hex;
				BeginArmyAttackAnimation( selected_army->index, selected_army->position_hex, defenders_hex );
				ResolveCombat( selected_army, &all_armies[map_nodes[highlighted_hex].occupier] );
				PlaySfx(SFX_GOBLIN_ROAR);
				unit_data_buffer_needs_update = true;
				path_edges_size = 0;
				draw_path = false;
			}
			else
			{
				PlaySfx(SFX_UI_CLICK_ERROR);
			}
		}
		else if ( left_clicked && selected_army == NULL )
		{
			if ( map_nodes[highlighted_hex].occupier > -1 )
			{
				if ( all_armies[ map_nodes[highlighted_hex].occupier ].move_done == false && all_armies[ map_nodes[highlighted_hex].occupier ].faction == active_faction )
				{
					selected_army = &all_armies[ map_nodes[highlighted_hex].occupier ];
					PlaySfx(SFX_UI_CLICK_A);	
				}
				else
				{
					PlaySfx(SFX_UI_CLICK_ERROR);
				}
			}
		}

		if ( selected_army != NULL && selected_army->move_done == false )
		{
			// TODO: don't do this every frame
			FindReachableNodes(pathfinder, selected_army->position_hex, selected_army->movement, selected_army->faction);

			draw_path = true;

			if ( selected_army->position_hex != highlighted_hex)
			{
				if ( current_path.x != selected_army->position_hex || current_path.y != highlighted_hex )
				{
					if ( FindPath(pathfinder, selected_army->position_hex, highlighted_hex, selected_army->faction) < 0 )
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

		if (selected_army != NULL)
		{
			if ( HexesAreNeighbours( selected_army->position_hex, highlighted_hex) )
			{
				if ( !HexIsFree(highlighted_hex) )
				{
					if (selected_army->action_done == false)
					{
						//SDL_SetCursor(cursor_swords_bmp);
						custom_cursor_should_be = 1;
					}

				}
			}
		}
	}

	if (ui_hovered) { io.ConfigFlags ^= ImGuiConfigFlags_NoMouseCursorChange; }
	else
	{
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
		if ( custom_cursor_should_be == 0)
		{ SDL_SetCursor(cursor_arrow); }
		else
		{ SDL_SetCursor(cursor_swords); }
	}

	static bool draw_ipi = true;
	static bool cycle_palette_phase = true;
	static int current_palette_phase = 0;

	static int text_string_count = 0;
	static float text_string_loc_x = 200.0f;
	static float text_string_loc_y = 124.0f;

	static char str0[48] = "Victimae Paschali Laudes";

	static int show_edge = 0;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();

	ShowGUIFrames();

	ShowHighlightedUnitInfo();

	ShowSelectedUnitInfo();

	ShowCombatResultsWindow();

	if (show_debug_ui)
	{
		ShowDebugUI();
	}

	if (show_main_menu)
	{
		ShowMainMenu();
	}

	if (show_memory_window)
	{
		ShowMemoryInspector();
	}

	if (show_settings_window)
	{
		ShowSettingsWindow();
	}

	static bool show_menu_button = true;
	if (show_menu_button )
	{
		ShowMenuButtonOverlay();		
	}

	static bool show_end_turn_button = true;
	if (show_end_turn_button)
	{
		ShowEndTurnButtonOverlay();
	}

	static bool show_cool_info_overlay_topbar = true;
	if (show_cool_info_overlay_topbar)
	{
		ShowCoolInfoOverlayTopBar();
	}

	ShowRestButton();

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

	if (draw_ipi)
	{
		if (cycle_palette_phase)
		{
			current_palette_phase = (master_timer / 100 ) % 256;
		}
		DrawIPI(current_palette_phase, cycle_palette_phase, zoom_level);
	}

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	DrawHexes();
	glDisable(GL_DEPTH_TEST);

	if (draw_path)
	{
		vec3 overlay_color = { 1.0f, 1.0f, 1.0f };
		UpdateHexMapBuffersForReachableNodesOverlay();
		DrawHexDebugOverlay(overlay_color, reachable_nodes_number);

		if (draw_hex_debug_overlay)
		{
			overlay_color = { 1.0f, 0.5f, 1.0f };
			UpdateHexMapBuffersForDebugOverlay();
			DrawHexDebugOverlay(overlay_color, analyzed_nodes_number);

			for (int i = 0; i <= path_edges_size; i++)
			{
				DrawEdgeAsArrow( map_edges[ path_edges[i] ].start_node_index, map_edges[ path_edges[i] ].end_node_index, map_edges[ path_edges[i] ].direction * 60.0f );
			}			
		}
	}

	glEnable(GL_DEPTH_TEST);
	DrawArmies(unit_data_count);
	glDisable(GL_DEPTH_TEST);
	if (army_moving)
	{
		float transition = (float)(movement_timer - movement_starts) / (float)ms_per_hex;
		DrawMovingArmy(moving_army, prev_hex, moving_to_hex, transition, all_armies[moving_army].base_sprite );
	}
	if (army_attacking)
	{
		float transition = (float)(movement_timer - movement_starts) / (float)(attack_anim_len);
		transition *= 3.1419f;
		transition = sinf(transition);
		transition *= 0.5f;
		transition = ClampValueToRangeF(transition, 0.0f, 1.0f);

		DrawMovingArmy(moving_army, all_armies[moving_army].position_hex, defenders_hex, transition, all_armies[moving_army].base_sprite );	
	}

	DrawStrings();

	{
		vec3 color_green = {0.0f, 1.0f, 0.0f };
		vec3 color_dark_green = {0.0f, 0.5f, 0.0f};
		vec3 color_red = { 1.0f, 0.0f, 0.0f};
		vec3 color_blue = { 0.0f, 0.0f, 1.0f};
		for ( int i = 0; i < ARMY_COUNT_MAX; i++)
		{
			if (all_armies[i].move_done == true && all_armies[i].faction == active_faction )
			{
				points_colors[i] = color_dark_green;
			}
			else if ( all_armies[i].move_done == false && all_armies[i].faction == active_faction )
			{
				points_colors[i] = color_green;
			}
			else
			{
				points_colors[i] = color_red;
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, points_color_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, ARMY_COUNT_MAX * sizeof(vec3), (const GLvoid*)&(points_colors[0]));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		DrawAllPoints(8.0f, true);
		DrawAllPoints(5.0f, false);
	}

	for (int i = 0; i < ARMY_COUNT_MAX; i++)
	{
		int hits = all_armies[i].hits_current;
		hits = ClampValueToRange(hits, 0, 15);

		vec3 color_faction = { 0.9f, 0.6f, 0.9f};;

		if ( all_armies[i].active == true )
		{

			if (all_armies[i].faction == FACTION_GOBLINS)
			{
				color_faction = { 0.9f, 0.6f, 0.4f};
			}
			else if ( all_armies[i].faction == FACTION_SURFERS)
			{
				color_faction = { 0.5f, 1.0f, 0.8f };
			}

			if (all_armies[i].move_done == true || all_armies[i].faction != active_faction)
			{
				color_faction.r -= 0.2f;
				color_faction.g -= 0.2f;
				color_faction.b -= 0.2f;
			}

			int hex = all_armies[i].position_hex;

			DrawHits(hex, color_faction, hits);
		}
	}

	ImGui::Render();
	//SDL_GL_MakeCurrent(window, gl_context); // ???
	//glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(window);
}

void MainLoop()
{
	uint64_t last_time_ticks = SDL_GetPerformanceCounter() - program_start_time_ticks;

	StartMusic();

	while(game_running)
	{
		uint64_t this_time_ticks = SDL_GetPerformanceCounter() - program_start_time_ticks;
		uint64_t delta_ticks = this_time_ticks - last_time_ticks;
		master_timer = (uint32_t)(1000.0 * ((double)this_time_ticks / (double)perf_freq));
		double delta_sec = (double)delta_ticks / (double)perf_freq;

		Step(delta_sec);

		last_time_ticks = this_time_ticks;
	}
}
