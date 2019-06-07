void CustomSDLCursors()
{
	uint8_t swords_cursor_data[4*32] = {
		0, 0, 0, 0, 24, 0, 0, 24, 
		36, 0, 0, 36, 34, 0, 0, 68, 
		41, 0, 0, 148, 20, 128, 1, 40, 
		10, 64, 2, 80, 5, 32, 4, 160, 
		2, 144, 9, 64, 1, 72, 18, 128, 
		0, 164, 37, 0, 0, 82, 74, 0, 
		0, 41, 148, 0, 0, 21, 40, 0, 
		0, 10, 80, 0, 0, 4, 160, 0, 
		0, 9, 80, 0, 0, 18, 200, 0, 
		2, 37, 164, 64, 5, 74, 82, 160, 
		5, 148, 41, 160, 5, 8, 16, 160, 
		4, 208, 11, 32, 2, 48, 12, 64, 
		5, 8, 16, 160, 9, 200, 19, 144, 
		18, 48, 12, 72, 36, 0, 0, 36, 
		72, 0, 0, 18, 80, 0, 0, 10, 
		32, 0, 0, 4, 0, 0, 0, 0
	};

	uint8_t swords_cursor_mask[4*32] = {
		0, 0, 0, 0, 24, 0, 0, 24, 
		60, 0, 0, 60, 62, 0, 0, 124, 
		63, 0, 0, 252, 31, 128, 1, 248, 
		15, 192, 3, 240, 7, 224, 7, 224, 
		3, 240, 15, 192, 1, 248, 31, 128, 
		0, 252, 63, 0, 0, 126, 126, 0, 
		0, 63, 252, 0, 0, 31, 248, 0, 
		0, 15, 240, 0, 0, 7, 224, 0, 
		0, 15, 240, 0, 0, 31, 248, 0, 
		2, 63, 252, 64, 7, 126, 126, 224, 
		7, 252, 63, 224, 7, 248, 31, 224, 
		7, 240, 15, 224, 3, 240, 15, 192, 
		7, 248, 31, 224, 15, 248, 31, 240, 
		30, 48, 12, 120, 60, 0, 0, 60, 
		120, 0, 0, 30, 112, 0, 0, 14, 
		32, 0, 0, 4, 0, 0, 0, 0
	};

	cursor_arrow = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	cursor_swords = SDL_CreateCursor( &swords_cursor_data[0], &swords_cursor_mask[0], 32, 32, 14, 14 );

	SDL_Surface *swords_bmp = SDL_LoadBMP("data/gfx/cursorsword.bmp");
	cursor_swords_bmp = SDL_CreateColorCursor(swords_bmp, 22, 22);

	SDL_Surface *walk_bmp = SDL_LoadBMP("data/gfx/cursorwalk.bmp");
	cursor_walk_bmp = SDL_CreateColorCursor(walk_bmp, 16, 16);

	SDL_Surface *arrow_bmp = SDL_LoadBMP("data/gfx/cursorarrow.bmp");
	cursor_arrow_bmp = SDL_CreateColorCursor(arrow_bmp, 1, 0);

}

void Init()
{
	DelagardiSettings settings = LoadSettings();
	settings_temp = settings;
	settings_confirmed = settings;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
	{
		printf("Couldn't init SDL.\n");
	}

	int display_num = SDL_GetNumVideoDisplays();
	printf("display_num: %d\n", display_num);

	int display_mode_num = SDL_GetNumDisplayModes(display_num - 1);
	printf("display_mode_num: %d\n", display_mode_num);
	if (display_mode_num < 0)
	{
		printf("ERror: %s\n", SDL_GetError());
	}

	SDL_DisplayMode *display_modes = NULL;
	display_modes = (SDL_DisplayMode*)malloc(display_mode_num * sizeof(SDL_DisplayMode));
	for (int i = 0; i < display_mode_num; i++)
	{
		SDL_GetDisplayMode(0, i, &display_modes[i]);
	}

	for (int i = 0; i < display_mode_num; i++)
	{
		printf("%d:\t%dx%d@%dHz %u\n", i, display_modes[i].w, display_modes[i].h, display_modes[i].refresh_rate, display_modes[i].format);
	}

	SDL_DisplayMode current_display_mode;
	SDL_GetCurrentDisplayMode(0, &current_display_mode);



	viewport_size.x = (float)settings.screen_width;
	viewport_size.y = (float)settings.screen_height;
	int borderless_flag = 0;
	int fullscreen_flag = 0;
	bool vsync_flag = settings.vsync;
	if (settings.borderless) { borderless_flag = SDL_WINDOW_BORDERLESS; }
	if (settings.fullscreen)
	{
		//fullscreen_flag = SDL_WINDOW_FULLSCREEN;
		fullscreen_flag = SDL_WINDOW_FULLSCREEN_DESKTOP;
		//viewport_size.x = (float)current_display_mode.w;
		//viewport_size.y = (float)current_display_mode.h;
	}

	//int create_window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_ALLOW_HIGHDPI;
	int create_window_flags = SDL_WINDOW_OPENGL | borderless_flag | fullscreen_flag | SDL_WINDOW_ALLOW_HIGHDPI;

	window = SDL_CreateWindow(
		"laiska",
		10, //SDL_WINDOWPOS_UNDEFINED,
		100, //SDL_WINDOWPOS_UNDEFINED,
		viewport_size.x, //current_display_mode.w,
		viewport_size.y, //current_display_mode.h,
		create_window_flags
		);

	//SDL_SetWindowDisplayMode(window, &display_modes[3]);
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);


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



	InitFont();
	InitHexMap();

	uint64_t starting1 = SDL_GetPerformanceCounter();
	FloodFillIslandIds();
	uint64_t stopping1 = SDL_GetPerformanceCounter();
	double calculating1 = (double)(stopping1 - starting1) / (double)perf_freq;
	printf("Flood filling island id's took %.04g ms.\n", calculating1*1000.0);
	
	InitArmyStuff();
	InitNewCamera();

	ArrangePiecesAroundOnTheBoardJohnImOnlyTesting();
}

void Shutdown()
{
	UnloadMusicAndSounds();

	ClearHexMapStuff();
	TakeDownArmyStuff();

	SDL_CloseAudioDevice(audio_dev);

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}