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
	InitArmyStuff();

	InitNewCamera();

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