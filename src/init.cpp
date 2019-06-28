void CustomSDLCursors()
{
	SDL_Surface *swords_bmp = SDL_LoadBMP("data/gfx/cursorsword.bmp");
	cursor_swords = SDL_CreateColorCursor(swords_bmp, 22, 22);

	SDL_Surface *walk_bmp = SDL_LoadBMP("data/gfx/cursorwalk.bmp");
	cursor_walk = SDL_CreateColorCursor(walk_bmp, 16, 16);

	SDL_Surface *arrow_bmp = SDL_LoadBMP("data/gfx/cursorarrow.bmp");
	cursor_arrow = SDL_CreateColorCursor(arrow_bmp, 1, 0);

}

void Init()
{
	DefaultConfigValues(&perkele_configs);
	ReadConfigsFromFile(&perkele_configs, config_file_name);
	ValidateConfigs(&perkele_configs);
	WriteConfigsToFile(&perkele_configs, config_file_name);

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
	{
		fprintf(stderr, "Couldn't init SDL.\n");
	}

	int display_num = SDL_GetNumVideoDisplays();
	fprintf(stderr, "display_num: %d\n", display_num);

	int display_mode_num = SDL_GetNumDisplayModes(display_num - 1);
	fprintf(stderr, "display_mode_num: %d\n", display_mode_num);
	if (display_mode_num < 0)
	{
		fprintf(stderr, "ERror: %s\n", SDL_GetError());
	}

	SDL_DisplayMode *display_modes = NULL;
	display_modes = (SDL_DisplayMode*)malloc(display_mode_num * sizeof(SDL_DisplayMode));
	for (int i = 0; i < display_mode_num; i++)
	{
		SDL_GetDisplayMode(0, i, &display_modes[i]);
	}

	perkele_display_modes_count = display_mode_num;
	perkele_display_modes = (PerkeleDisplayMode*)malloc(perkele_display_modes_count * sizeof(PerkeleDisplayMode));
	for (int i = 0; i < perkele_display_modes_count; i++)
	{
		perkele_display_modes[i].w = display_modes[i].w;
		perkele_display_modes[i].h = display_modes[i].h;
		snprintf(&(perkele_display_modes[i].name[0]), 64, "%d: %d x %d", i, perkele_display_modes[i].w, perkele_display_modes[i].h);
		// fprintf(stderr, "%s\n", &(perkele_display_modes[i].name[0]));
	}
	
	SDL_DisplayMode current_display_mode;
	SDL_GetCurrentDisplayMode(0, &current_display_mode);

	// CAN I DELETE THESE???
	viewport_size.x = (float)perkele_configs.screen_w;
	viewport_size.y = (float)perkele_configs.screen_h;

	bool vsync_flag = perkele_configs.enable_vsync;
	int create_window_flags = 0;
	int create_window_w = perkele_configs.screen_w;
	int create_window_h = perkele_configs.screen_h;

	switch ( perkele_configs.screen_mode )
	{
		case 0:
			create_window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN_DESKTOP;
			create_window_w = current_display_mode.w;
			create_window_h = current_display_mode.h;

			//tempppp
			viewport_size.x = (float)current_display_mode.w;
			viewport_size.y = (float)current_display_mode.h;
			break;
		case 1:
			create_window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN;
			break;
		case 2:
			create_window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
	}

	window = SDL_CreateWindow(
		"PERKELE!",
		10, //SDL_WINDOWPOS_UNDEFINED,
		100, //SDL_WINDOWPOS_UNDEFINED,
		create_window_w,
		create_window_h,
		create_window_flags
		);

	//SDL_SetWindowResizable(window, SDL_TRUE);

	SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	gl_context = SDL_GL_CreateContext(window);

	if(!gladLoadGL()) {
		printf("GLAD GLOUDN*T GLOAD GL! Something went wrong!\n");
		exit(-1);
	}

	{
		// Version info
		SDL_version compiled;
		SDL_version linked;
		SDL_VERSION(&compiled);
		SDL_GetVersion(&linked);
		printf("Compiled against SDL version %d.%d.%d\n", compiled.major, compiled.minor, compiled.patch);
		printf("Linking against SDL version %d.%d.%d\n", linked.major, linked.minor, linked.patch);
		printf("OpenGL version: %s\n", glGetString(GL_VERSION));
		printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		printf("Vendor: %s\n", glGetString(GL_VENDOR));
		printf("Renderer: %s\n", glGetString(GL_RENDERER));
	}

	if (vsync_flag)
	{
		//nop
	}
	else
	{
		int trySetSwapInterval = SDL_GL_SetSwapInterval(0);
		if (trySetSwapInterval == -1) { printf("%s\n", SDL_GetError()); }
		else { printf("Vsync disabled\n"); }
	}

	CustomSDLCursors();



	// AUDIO

	SDL_AudioSpec fmt, got;
	audio_mutex = SDL_CreateMutex();

	/* Init SDL audio */
	memset(&fmt, 0, sizeof(fmt));
	fmt.freq      = 44100;
	fmt.format    = AUDIO_S16;
	fmt.channels  = 2;
	fmt.samples   = 1024;
	fmt.callback  = audio_callback;

	audio_dev = SDL_OpenAudioDevice(NULL, 0, &fmt, &got, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
	if (audio_dev == 0)
	{
		fprintf(stderr, "Error: failed to open audio device '%s'\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	/* Init library */
	cm_init(got.freq);
	cm_set_lock(lock_handler);
	cm_set_master_gain(0.5);

	/* Start audio */
	SDL_PauseAudioDevice(audio_dev, 0);



	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	int size_pixels = 15;
	float size_pixels_monofonto = 15.0f;
	//io.Fonts->AddFontFromFileTTF("data/fonts/roboto-medium.ttf", size_pixels);
	im_font_roboto = io.Fonts->AddFontFromFileTTF("data/fonts/roboto-medium.ttf", size_pixels);
	im_font_monofonto = io.Fonts->AddFontFromFileTTF("data/fonts/monofonto.ttf", size_pixels_monofonto);
	//io.Fonts->GetTexDataAsAlpha8();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	printf("Dear ImGui version: %s\n", IMGUI_VERSION);


	srand((unsigned int)time(NULL));



	// these switches and values are all over. maybe they could all gather to some place nice
	bypass_main_menu = perkele_configs.bypass_main_menu;
	bypass_main_menu ? show_main_menu = false : show_main_menu = true;

	show_debug_ui = perkele_configs.enable_debug_window;

	// InitFont();
	pathfinder = (Pathfinder*)malloc(sizeof(Pathfinder));
	InitPathfinder(pathfinder);
	InitHexMap();
	InitArmyStuff();
	InitNewCamera();



	ArrangePiecesAroundOnTheBoardJohnImOnlyTesting();
}

void Shutdown()
{
	UnloadMusicAndSounds();

	ClearHexMapStuff();
	TakeDownArmyStuff();

	ShutdownPathfinder(pathfinder);
	free(pathfinder);

	SDL_CloseAudioDevice(audio_dev);

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
