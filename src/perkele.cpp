#define ASSERTS_ENABLED 1

#include <stdio.h>
#include <time.h>

//#include <emmintrin.h>

#include <SDL.h>

extern "C" {
#include "glad.h"
#include "cmixer.h"
#include "stb_vorbis.h"
#include "stb_image.h"
#include "stb_truetype.h"
#include "ini.h"
}
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "macros.h"
#include "types.h"
#include "loader.h"
#include "camera.h"

#include "matik.cpp"
#include "globals.cpp"
#include "settings.cpp"
#include "audio.cpp"
#include "camera.cpp"
#include "text.cpp"
#include "hexmap.cpp"
#include "army.cpp"
#include "pathfinder.cpp"
#include "init.cpp"
#include "loader.cpp"
#include "loop.cpp"



int main()
{

	perf_freq = SDL_GetPerformanceFrequency();
	program_start_time_ticks = SDL_GetPerformanceCounter();
	printf("-----\n%.4f\tProgram start.\n-----\n", 0.0);

	Init();
	LoadInitialData();

	uint64_t init_done_at = SDL_GetPerformanceCounter();
	double initialization_time = (double)(init_done_at - program_start_time_ticks) / (double)perf_freq;
	printf("-----\n%.04f\tInitialization done.\n-----\n", initialization_time);

	MainLoop();

	Shutdown();

	return 0;
}
