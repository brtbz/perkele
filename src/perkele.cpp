#define ASSERTS_ENABLED 1

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include <SDL.h>

extern "C" {
#include "glad.h"
#include "cmixer.h"
#include "stb_vorbis.h"
#include "stb_image.h"
#include "stb_truetype.h"
}
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "macros.h"
#include "types.h"
#include "loader.h"
#include "camera.h"
#include "morrigan.h"

#include "matik.cpp"
#include "globals.cpp"
#include "configs.cpp"
#include "audio.cpp"
#include "background.cpp"
#include "camera.cpp"
#include "text.cpp"
#include "hexmap.cpp"
#include "army.cpp"
#include "pathfinder.cpp"
#include "balor.cpp"
#include "morrigan.cpp"
#include "gui.cpp"
#include "init.cpp"
#include "loader.cpp"
#include "loop.cpp"

#ifdef _WIN32
extern "C" {
__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif



int main(int argc, char* argv[])
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
