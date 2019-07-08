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

static void ShowMenuButtonOverlay(bool* p_open)
{
	const float distance = 2.0f;
	ImVec2 window_pos = ImVec2( ImGui::GetIO().DisplaySize.x - distance, 0 + distance );
	ImVec2 window_pos_pivot = ImVec2( 1.0f, 0.0f );

	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowSize( ImVec2(60.0f, 40.0f) );
	ImGui::SetNextWindowBgAlpha(0.8f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("Example: Simple overlay", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		if (ImGui::Button("MENU", ImVec2(60.0f, 40.0f))) { show_main_menu = !show_main_menu; }
	}
	ImGui::End();
	ImGui::PopStyleVar(2);
}

static void ShowEndTurnButtonOverlay(bool* p_open)
{
	const float distance = 4.0f;
	ImVec2 window_pos = ImVec2( ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y - distance );
	ImVec2 window_pos_pivot = ImVec2(0.5f, 1.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowBgAlpha(0.8f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("END TURN BUTTON OVERLAY", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		if (ImGui::Button("END TURN", ImVec2(160.0f, 40.0f) )) { game_turn++; EndTurn(); }
	}
	ImGui::End();
	ImGui::PopStyleVar(2);
}

void ShowRestButton(bool* p_open)
{
	const float distance = 4.0f;
	ImVec2 window_pos = ImVec2( ImGui::GetIO().DisplaySize.x / 2.0f - 164.0f, ImGui::GetIO().DisplaySize.y - distance );
	ImVec2 window_pos_pivot = ImVec2(0.5f, 1.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowBgAlpha(0.8f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("REST BUTTON OVERLAY", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		if (ImGui::Button("REST", ImVec2(160.0f, 40.0f) ))
		{
			if (selected_army != NULL && selected_army->move_done == false && selected_army->action_done == false)
			{
				PlaySfx(SFX_GOBLIN_ROAR);
				int hits_recovered = selected_army->wounded_soldiers;
				selected_army->hits_current += hits_recovered;
				selected_army->wounded_soldiers = 0;
				selected_army->move_done = true;
				selected_army->action_done = true;

				unit_data_buffer_needs_update = true; // or does it???

				selected_army = NULL;
				draw_path = false;
			}
			else
			{
				PlaySfx(SFX_UI_CLICK_ERROR);
			}
		}
	}
	ImGui::End();
	ImGui::PopStyleVar(2);
}

void ShowCoolInfoOverlayTopBar(bool* p_open)
{
	float distance = 2.0f;
	ImVec2 window_pos = ImVec2( ImGui::GetIO().DisplaySize.x / 2.0f, 0 + distance );
	ImVec2 window_pos_pivot = ImVec2( 0.5f, 1.0f );
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowSize(ImVec2(896.0f, 18.0f));
	ImGui::SetNextWindowBgAlpha(0.3f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	if ( ImGui::Begin("Cool Info Overlay Top Bar", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav) )
	{
		// use ImGui's columns to group the text here nicely
		ImGui::Text("Turn: %d - %s (cool info here in the info overlay top bar yeah!)", game_turn, active_faction_str);
	}
	ImGui::End();
	ImGui::PopStyleVar(1);
}

void ShowHighlightedUnitInfo(bool* p_open)
{
	ImVec2 window_pos = ImVec2( ImGui::GetIO().DisplaySize.x - 202.0f, 48.0f );
	ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowSize(ImVec2(200.0f, 160.0f));
	ImGui::SetNextWindowBgAlpha(0.3f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	if (ImGui::Begin("highlighted unit",  p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		if ( highlighted_hex > -1 && map_nodes[highlighted_hex].occupier > -1 )
		{
			int army_index = map_nodes[highlighted_hex].occupier;
			ImGui::Text("%s", test_armies[ army_index ].name );
			ImGui::Text("faction: %s", faction_names[ test_armies[ army_index ].faction ]);
			ImGui::Text("Strength: %d", test_armies[ army_index ].strength);
			ImGui::Text("Armor: %d", test_armies[ army_index ].armor);
			ImGui::Text("Movement: %d", test_armies[ army_index ].movement);
			ImGui::Text("Hits:  %d / %d", test_armies[ army_index ].hits_current, test_armies[ army_index ].hits_max);
			ImGui::Text("Wounded: %d Dead: %d", test_armies[army_index].wounded_soldiers, test_armies[army_index].dead_soldiers)	;
		}
	}
	ImGui::End();
	ImGui::PopStyleVar(1);
}

void ShowSelectedUnitInfo(bool* p_open)
{
	ImVec2 window_pos = ImVec2( ImGui::GetIO().DisplaySize.x - 202.0f, 212.0f );
	ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowSize(ImVec2(200.0f, 160.0f));
	ImGui::SetNextWindowBgAlpha(0.3f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	if (ImGui::Begin("selectedd unit",  p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		if (selected_army != NULL)
		{
			ImGui::Text("SELECTED UNIT");
			ImGui::Text("%s", selected_army->name);
			ImGui::Text("Strength: %d", selected_army->strength);
			ImGui::Text("Movement: %d", selected_army->movement);
			ImGui::Text("Hits:  %d / %d", selected_army->hits_current, selected_army->hits_max);
			ImGui::Text("Position %d", selected_army->position_hex);
		}
	}
	ImGui::End();
	ImGui::PopStyleVar(1);
}

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
			else if (evt.key.keysym.scancode == SDL_SCANCODE_Q)
			{
				//game_running = false;
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

	if (!army_moving) // disable UI while moving unit around the map. is that overkill?
	{
		if ( right_clicked && selected_army != NULL)
		{
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
					// selected_army = NULL;
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
				ResolveCombat( selected_army, &test_armies[map_nodes[highlighted_hex].occupier] );
				PlaySfx(SFX_GOBLIN_ROAR);
				selected_army = NULL;
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
				if ( test_armies[ map_nodes[highlighted_hex].occupier ].move_done == false && test_armies[ map_nodes[highlighted_hex].occupier ].faction == active_faction )
				{
					selected_army = &test_armies[ map_nodes[highlighted_hex].occupier ];
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
			FindReachableNodes(pathfinder, selected_army->position_hex, selected_army->movement);

			draw_path = true;

			if ( selected_army->position_hex != highlighted_hex)
			{
				if ( current_path.x != selected_army->position_hex || current_path.y != highlighted_hex )
				{
					if ( FindPath(pathfinder, selected_army->position_hex, highlighted_hex) < 0 )
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
					//SDL_SetCursor(cursor_swords_bmp);
					custom_cursor_should_be = 1;
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

	static float music_gain = perkele_configs.music_gain;
	static float master_gain = perkele_configs.master_gain;

	static bool show_demo_window = false;
	static bool draw_ipi = true;
	static bool cycle_palette_phase = true;
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

	if (0 && selected_army != NULL)
	{
		ImGui::Begin("SELECTED UNIT");
		ImGui::Text("%s", selected_army->name);
		ImGui::Text("Strength: %d", selected_army->strength);
		ImGui::Text("Movement: %d", selected_army->movement);
		ImGui::Text("Hits:  %d / %d", selected_army->hits_current, selected_army->hits_max);
		ImGui::Text("Position %d", selected_army->position_hex);
		ImGui::Separator();
		ImGui::Text("Base sprite: %d", selected_army->base_sprite);
		ImGui::InputInt("change", (int*)&(selected_army->base_sprite) );
		unit_data_buffer_needs_update = true;

		ImGui::End();
	}

	bool show_highlighted_unit_info = true;
	ShowHighlightedUnitInfo(&show_highlighted_unit_info);

	bool show_selected_unit_info = true;
	ShowSelectedUnitInfo(&show_selected_unit_info);

	const float distance = 2.0f;
	ImVec2 window_pos = ImVec2( ImGui::GetIO().DisplaySize.x - distance, 0 + distance );
	ImVec2 window_pos_pivot = ImVec2( 1.0f, 0.0f );



	{
		ImGui::SetNextWindowPos( ImVec2( ImGui::GetIO().DisplaySize.x - 404.0f , ImGui::GetIO().DisplaySize.y - 76.0f ) );
		ImGui::SetNextWindowSize( ImVec2(400.0f, 72.0f));
		ImGui::Begin("Combat Results", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::Text("%s", combat_result_str1);
		ImGui::Text("%s", combat_result_str2);
		ImGui::End();
	}

	if (show_debug_ui)
	{
		// dear imgui feature demo
		if (show_demo_window) { ImGui::ShowDemoWindow(&show_demo_window); }

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("debug window");

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
#if 0
			static int sfx_number = 0;
			ImGui::SliderInt("sfx", &sfx_number, 0, 1);
			ImGui::SameLine();
			ImGui::Text("%d ", sfx[sfx_number].id);
			ImGui::SameLine();
			if ( ImGui::Button("woah!") ) { cm_play(sfx[sfx_number].src); }

			ImGui::Separator();
			ImGui::InputText("text", str0, IM_ARRAYSIZE(str0));
			ImGui::SliderInt("text c", &text_string_count, 0, 24);
			ImGui::SliderFloat("text x", &text_string_loc_x, -500.0f, 2000.0f, "%.3f", 1.0f);
			ImGui::SliderFloat("text y", &text_string_loc_y, -200.0f, 1200.0f, "%.3f", 1.0f);
#endif
			ImGui::Separator();

			ImGui::Text("zoom_level: %.2f", zoom_level);
			ImGui::Text("CameraSize %.2f %.2f", camera.Size().x, camera.Size().y);
			ImGui::Text("CameraOrigo %.2f %.2f", camera.Origo().x, camera.Origo().y);

			ivec2 staggered_hex = { map_nodes[highlighted_hex].x, map_nodes[highlighted_hex].y };
			ivec2 slanted_hex = StaggeredToSlanted( staggered_hex );
			ImGui::Text("HIGHLIGHT HEX: %d (Staggered: X%d Y%d Slanted: X%d Y%d)", 
				highlighted_hex,
				map_nodes[highlighted_hex].x,
				map_nodes[highlighted_hex].y,
				slanted_hex.x,
				slanted_hex.y);
			ImGui::Text("island_id: %d", map_nodes[highlighted_hex].pathfind_island_id);

			ImGui::Separator();
			ImGui::Text("%s", dumb_debug_string);
			ImGui::Text("%s", dumb_debug_string2);

			ImGui::Separator();
			ImGui::Checkbox("Ignore move rules", &ignore_move_rules);
			ImGui::Checkbox("Pathfind debug overlay", &draw_hex_debug_overlay);
			ImGui::InputFloat("H weight", &(pathfinder->pathfind_weight_h) ); ImGui::SameLine();
			ShowHelpMarker("Heuristic weight for pathfind algorithm.\nHigher is faster, lower is more accurate.");
			ImGui::Text("total in open set: %d", pathfinder->number_of_nodes_that_were_in_open_set_debug);
			ImGui::Text("open_set_count: %d", pathfinder->open_set_count);
			ImGui::Text("closed_set_count: %d", pathfinder->closed_set_count);
			ImGui::Separator();
			ImGui::Text("Current Path %d %d", current_path.x, current_path.y);
			ImGui::Text("Path_edges_size %u", path_edges_size);

			ImGui::InputText("loadmap", loadmap_str, IM_ARRAYSIZE(loadmap_str));
			ImGui::SameLine();
			if ( ImGui::Button("LOAD MAP") ) { LoadNewMap(loadmap_str); }

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
		ImGui::InputInt("mem.addr", (int*)((uint64_t)&mm_ptr), 16, 256);
		ImGui::SameLine(); ShowHelpMarker("Small adjusts to move pointer around,\nbigger adjusts to crash the program.");

		ImGui::End();
	}

	if (show_settings_window)
	{
		ImGui::Begin("Settings");

		ImGui::Text("DISPLAY");
		static int e = perkele_configs.screen_mode;
		static bool tempvsync = perkele_configs.enable_vsync;
		static int dims[2] = { int(viewport_size.x), (int)viewport_size.y };

		// common sdlwindow flags: SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI
		ImGui::RadioButton("fake fullscreen", &e, 0); // additional flag: SDL_WINDOW_FULLSCREEN_DESKTOP
		if (ImGui::IsItemHovered()) { ImGui::SetTooltip("also known as borderless windowed"); }
		//ImGui::RadioButton("real fullscreen", &e, 1); // additional flag: SDL_WINDOW_FULLSCREEN
		ImGui::Text("real fullscreen not yet supported");
		ImGui::RadioButton("windowed " , &e, 2); // no additional flags
		ImGui::Text(" ");
		ImGui::Text("Window Size: (fake fullscreen mode will ignore these values)");
		ImGui::InputInt2("Dimensions", &dims[0]);
		if (ImGui::IsItemHovered()) { ImGui::SetTooltip("window dimensions"); }
		dims[0] = ClampValueToRange(dims[0], 1024, 7680 );
		dims[1] = ClampValueToRange(dims[1], 600, 4320 );

		// General BeginCombo() API, you have full control over your selection data and display type.
		// (your selection data could be an index, a pointer to the object, an id for the object, a flag stored in the object itself, etc.)
		static ImGuiComboFlags flags = 0;
		static PerkeleDisplayMode *current_pdm = NULL;

		if (ImGui::BeginCombo("Reported Available Fullscreeen Resolutions", current_pdm->name, flags)) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < perkele_display_modes_count; n++)
			{
				bool is_selected = (current_pdm == &perkele_display_modes[n]);
				if (ImGui::Selectable(perkele_display_modes[n].name, is_selected))
				{
					current_pdm = &perkele_display_modes[n];
					dims[0] = current_pdm->w;
					dims[1] = current_pdm->h;
				}
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Checkbox("Vertical Sync", &tempvsync);
		if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Recommended to be on\nthis is currently the only way to limit frame rate"); }

		ImGui::Separator();

		static bool tempmus = true;
		static bool tempsfx = true;
		static float sfx_gain = 1.0f;

		ImGui::Text("AUDIO");
		// ImGui::Checkbox("enable music", &tempmus);
		// ImGui::Checkbox("enable sound effects", &tempsfx);
		ImGui::SliderFloat("master gain", &master_gain, 0.0f, 2.0f, "%.2f", 1.0f);
		ImGui::SliderFloat("music gain", &music_gain, 0.0f, 2.0f, "%.2f", 1.0f);
		//ImGui::SliderFloat("sfx gain", &sfx_gain, 0.0f, 2.0f, "%.2f", 1.0f);

		ImGui::Separator();

		ImGui::Text("GAME");
		ImGui::Checkbox("Edge Scroll Map", &mouse_edge_scroll);

		ImGui::Separator();

		ImGui::Text("DEV");

		static bool bypass_main_menu = perkele_configs.bypass_main_menu;
		ImGui::Checkbox("enable debug ui", &show_debug_ui);
		ImGui::Checkbox("bypass main menu on startup", &bypass_main_menu);
		ImGui::Checkbox("Ignore move rules", &ignore_move_rules);
		ImGui::Checkbox("Pathfind debug overlay", &draw_hex_debug_overlay);

		ImGui::Separator();

		if (ImGui::Button("OK###SETTINGS_WINDOW_OK"))
		{
			perkele_configs.master_gain = master_gain;
			perkele_configs.music_gain = music_gain;
			perkele_configs.sfx_gain = sfx_gain;
			perkele_configs.mouse_edge_scroll = mouse_edge_scroll;
			perkele_configs.bypass_main_menu = bypass_main_menu;
			perkele_configs.enable_debug_window = show_debug_ui;
			perkele_configs.ignore_move_rules = ignore_move_rules;
			perkele_configs.pathfind_debug_overlay = draw_hex_debug_overlay;
			perkele_configs.screen_mode = e;
			perkele_configs.screen_w = dims[0];
			perkele_configs.screen_h = dims[1];
			perkele_configs.enable_vsync = tempvsync;
			ValidateConfigs(&perkele_configs);
			WriteConfigsToFile(&perkele_configs, config_file_name);
			show_settings_window = false;
		}
		if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Changed display options require restart to take effect."); }
		ImGui::SameLine();
		if (ImGui::Button("Cancel###SETTINGS_WINDOW_CANCEL") ) { show_settings_window = false; }

		ImGui::End();
	}

	static bool show_menu_button = true;
	if (show_menu_button && !show_main_menu )
	{
		ShowMenuButtonOverlay(&show_menu_button);		
	}

	static bool show_end_turn_button = true;
	if (show_end_turn_button)
	{
		ShowEndTurnButtonOverlay(&show_end_turn_button);
	}

	static bool show_cool_info_overlay_topbar = true;
	if (show_cool_info_overlay_topbar)
	{
		ShowCoolInfoOverlayTopBar(&show_cool_info_overlay_topbar);
	}

	static bool show_rest_button = true;
	ShowRestButton(&show_rest_button);

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
		DrawMovingArmy(moving_army, prev_hex, moving_to_hex, transition, test_armies[moving_army].base_sprite );
	}

	DrawStrings();

	{
		vec3 color_green = {0.0f, 1.0f, 0.0f };
		vec3 color_red = { 1.0f, 0.0f, 0.0f};
		vec3 color_blue = { 0.0f, 0.0f, 1.0f};
		for ( int i = 0; i < 183; i++)
		{
			if (test_armies[i].move_done == true && test_armies[i].faction == active_faction )
			{
				points_colors[i] = color_red;
			}
			else if ( test_armies[i].move_done == false && test_armies[i].faction == active_faction )
			{
				points_colors[i] = color_green;
			}
			else
			{
				points_colors[i] = color_blue;
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, points_color_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 183 * sizeof(vec3), (const GLvoid*)&(points_colors[0]));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		DrawAllPoints(8.0f, true);
		DrawAllPoints(5.0f, false);
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
