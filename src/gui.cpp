// Helper to display a little (?) mark which shows a tooltip when hovered.
void ShowHelpMarker(const char* desc)
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

void ShowMenuButtonOverlay()
{
	const float distance = 2.0f;
	ImVec2 window_pos = ImVec2( ImGui::GetIO().DisplaySize.x - distance, 0 + distance );
	ImVec2 window_pos_pivot = ImVec2( 1.0f, 0.0f );

	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowSize( ImVec2(60.0f, 40.0f) );
	ImGui::SetNextWindowBgAlpha(0.8f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("Example: Simple overlay", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		if (ImGui::Button("MENU", ImVec2(60.0f, 40.0f))) { show_main_menu = !show_main_menu; }
	}
	ImGui::End();
	ImGui::PopStyleVar(1);
}

void ShowEndTurnButtonOverlay()
{
	const float distance = 4.0f;
	ImVec2 window_pos = ImVec2( ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y - distance );
	ImVec2 window_pos_pivot = ImVec2(0.5f, 1.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowBgAlpha(0.8f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("END TURN BUTTON OVERLAY", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		if (ImGui::Button("END TURN", ImVec2(160.0f, 40.0f) )) { game_turn++; EndTurn(); }
	}
	ImGui::End();
	ImGui::PopStyleVar(1);
}

void ShowRestButton()
{
	const float distance = 4.0f;
	ImVec2 window_pos = ImVec2( ImGui::GetIO().DisplaySize.x / 2.0f - 164.0f, ImGui::GetIO().DisplaySize.y - distance );
	ImVec2 window_pos_pivot = ImVec2(0.5f, 1.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowBgAlpha(0.8f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("REST BUTTON OVERLAY", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		if (ImGui::Button("REST", ImVec2(160.0f, 40.0f) ))
		{
			if (selected_army != NULL && selected_army->move_done == false && selected_army->action_done == false)
			{
				PlaySfx(SFX_GOBLIN_ROAR);
				int hits_recovered = selected_army->wounded_soldiers;
				if (CheckForEnemyZonesOfControl(selected_army->position_hex, selected_army->faction))
				{
					hits_recovered = 1;
				}
				selected_army->hits_current += hits_recovered;
				selected_army->wounded_soldiers -= hits_recovered;
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
	ImGui::PopStyleVar(1);
}

void ShowCoolInfoOverlayTopBar()
{
	float distance = 2.0f;
	ImVec2 window_pos = ImVec2( ImGui::GetIO().DisplaySize.x / 2.0f, 0 + distance );
	ImVec2 window_pos_pivot = ImVec2( 0.5f, 1.0f );
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowSize(ImVec2(896.0f, 18.0f));
	ImGui::SetNextWindowBgAlpha(0.3f);

	if ( ImGui::Begin("Cool Info Overlay Top Bar", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav) )
	{
		// use ImGui's columns to group the text here nicely
		ImGui::Text("Turn: %d - %s (cool info here in the info overlay top bar yeah!)", game_turn, active_faction_str);
	}
	ImGui::End();
}

void ShowHighlightedUnitInfo()
{
	ImVec2 window_pos = ImVec2( ImGui::GetIO().DisplaySize.x - 202.0f, 48.0f );
	ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowSize(ImVec2(200.0f, 230.0f));
	// ImGui::SetNextWindowBgAlpha(0.3f);

	if (ImGui::Begin("highlighted unit", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		if ( highlighted_hex > -1 && map_nodes[highlighted_hex].occupier > -1 )
		{
			int army_index = map_nodes[highlighted_hex].occupier;
			ImGui::Text("%s", all_armies[ army_index ].name );
			ImGui::Text("faction: %s", faction_names[ all_armies[ army_index ].faction ]);
			ImGui::Text("Strength: %d", all_armies[ army_index ].strength);
			ImGui::Text("Armor: %d", all_armies[ army_index ].armor);
			ImGui::Text("Movement: %d", all_armies[ army_index ].movement);
			ImGui::Text("Hits:  %d / %d", all_armies[ army_index ].hits_current, all_armies[ army_index ].hits_max);
			ImGui::Text("Wounded: %d Dead: %d", all_armies[army_index].wounded_soldiers, all_armies[army_index].dead_soldiers)	;
		}
	}
	ImGui::End();
}

void ShowSelectedUnitInfo()
{
	ImVec2 window_pos = ImVec2( ImGui::GetIO().DisplaySize.x - 202.0f, 282.0f );
	ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowSize(ImVec2(200.0f, 230.0f));
	// ImGui::SetNextWindowBgAlpha(0.3f);

	if (ImGui::Begin("selectedd unit", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
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
}

void ShowCombatResultsWindow()
{
	ImGui::SetNextWindowPos( ImVec2( ImGui::GetIO().DisplaySize.x - 404.0f , ImGui::GetIO().DisplaySize.y - 76.0f ) );
	ImGui::SetNextWindowSize( ImVec2(400.0f, 72.0f));
	ImGui::Begin("Combat Results", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoMove);
	ImGui::Text("%s", combat_result_str1);
	ImGui::Text("%s", combat_result_str2);
	ImGui::End();
}

void ShowSettingsWindow()
{
	const float padding_to_screen_edge = 100.0f;
	const float max_width = 720.0f;

	ImVec2 window_pos_pivot = ImVec2( 0.5f, 0.5f );
	ImVec2 window_size = ImVec2( ImGui::GetIO().DisplaySize.x - 2 * padding_to_screen_edge, ImGui::GetIO().DisplaySize.y - 2 * padding_to_screen_edge );
	if (window_size.x > max_width) { window_size.x = max_width; }

	ImGui::SetNextWindowPos( ImVec2( ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y / 2.0f ), ImGuiCond_Always, window_pos_pivot );
	ImGui::SetNextWindowSize( window_size );

	ImGui::Begin("Settings", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);

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

void ShowMainMenu()
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

void ShowArmyEditWindow()
{
	if (selected_army != NULL)
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
}

void ShowMemoryInspector()
{
	static void* mm_ptr = (void*)(&all_armies[0]);
	static int inspector_line_count = 16;
		ImGui::Begin("Memory. &all_armies[0] (Can set this only in source code for now.)");
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

void ShowDebugUI()
{
	static bool show_demo_window = false;
	static char loadmap_str[128] = "data/maps/terrain_perke_no.csv";

	// dear imgui feature demo
	if (show_demo_window) { ImGui::ShowDemoWindow(&show_demo_window); }
	{
		ImGui::Begin("debug window");

		ImGui::Checkbox("ImGuiDemo", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::SameLine();
		ImGui::Checkbox("Settings###SETTINGS_CHECKBOX", &show_settings_window);
		ImGui::SameLine();
		ImGui::Checkbox("Memory", &show_memory_window);

		ImGui::Separator();

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Separator();

		// ImGui::Text("zoom_level: %.2f", zoom_level);
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

		ImGui::Separator();

		ImGui::Text("Total units: 0");

		static int unit_location = 3444;
		static int unit_type = GOBLIN_BATHROBE_BLUE;
		static int unit_faction = 0;
		static char unit_name[32] = "New Army";

		ImGui::InputInt("location", &unit_location);
		ImGui::InputInt("type", &unit_type);
		ImGui::InputInt("faction", &unit_faction);
		ImGui::InputText("name", unit_name, 32);

		if ( ImGui::Button("Add Unit") ) { /*AddUnit();*/ }

		ImGui::End();
	}
}

void ShowGUIFrames()
{
	ImGuiWindowFlags gui_frame_window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing 
		| ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus; // | ImGuiWindowFlags_NoMouseInputs;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	{
		ImVec2 window_pos = ImVec2( 0.0f, 0.0f );
		ImVec2 window_pos_pivot = ImVec2( 0.0f, 0.0f );
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowSize(ImVec2( ImGui::GetIO().DisplaySize.x , 38.0f));
		// ImGui::SetNextWindowBgAlpha(0.3f);

		ImGui::Begin("GUI_Frame_1", NULL, gui_frame_window_flags);
		ImGui::End();
	}

	{
		ImVec2 window_pos = ImVec2( ImGui::GetIO().DisplaySize.x - 210.0f, 0.0f );
		ImVec2 window_pos_pivot = ImVec2( 0.0f, 0.0f );
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowSize(ImVec2( 210.0f, ImGui::GetIO().DisplaySize.y));
		// ImGui::SetNextWindowBgAlpha(0.3f);

		ImGui::Begin("GUI_Frame_2", NULL, gui_frame_window_flags);
		ImGui::End();
	}
	{
		ImVec2 window_pos = ImVec2( 0.0f, ImGui::GetIO().DisplaySize.y - 48.0f );
		ImVec2 window_pos_pivot = ImVec2( 0.0f, 0.0f );
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowSize(ImVec2( ImGui::GetIO().DisplaySize.x, 48.0f));
		// ImGui::SetNextWindowBgAlpha(0.3f);

		ImGui::Begin("GUI_Frame_3", NULL, gui_frame_window_flags);
		ImGui::End();
	}
	ImGui::PopStyleVar(2);
}
