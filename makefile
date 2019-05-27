MAIN_SRC = perkele.cpp
GLAD_SRC = glad.c
IMGUI_SRC = imgui.cpp imgui_draw.cpp imgui_widgets.cpp imgui_demo.cpp imgui_impl_opengl3.cpp imgui_impl_sdl.cpp
CMIXER_SRC = cmixer.c
STB_VORBIS_SRC = stb_vorbis.c
LIBS_SRC = implelibs.c # stb_image.h stb_rect_pack.h stb_truetype.h ini.h

COMPILER_C = cc
COMPILER_CPP = g++

CFLAGS_C = -std=c99 -g
CFLAGS_CPP = -std=c++11 -I/usr/include/SDL2 -Isrc/libs/ -g

LFLAGS_C = -lm
LFLAGS_CPP = -ldl -lSDL2

MAIN_SRC_PATH = $(addprefix src/, $(MAIN_SRC))

GLAD_SRC_PATH = $(addprefix src/libs/, $(GLAD_SRC))
GLAD_OBJ = glad.o
GLAD_OBJ_PATH = $(addprefix obj/, $(GLAD_OBJ))

IMGUI_SRC_PATH = $(addprefix src/libs/, $(IMGUI_SRC))
IMGUI_OBJ = imgui.o imgui_draw.o imgui_widgets.o imgui_demo.o imgui_impl_opengl3.o imgui_impl_sdl.o
IMGUI_OBJ_PATH = $(addprefix obj/, $(IMGUI_OBJ))

CMIXER_SRC_PATH = $(addprefix src/libs/, $(CMIXER_SRC))
CMIXER_OBJ = cmixer.o
CMIXER_OBJ_PATH = $(addprefix obj/, $(CMIXER_OBJ))

STB_VORBIS_SRC_PATH = $(addprefix src/libs/, $(STB_VORBIS_SRC))
STB_VORBIS_OBJ = stb_vorbis.o
STB_VORBIS_OBJ_PATH = $(addprefix obj/, $(STB_VORBIS_OBJ))

LIBS_SRC_PATH = $(addprefix src/libs/, $(LIBS_SRC))
LIBS_OBJ = implelibs.o
LIBS_OBJ_PATH = $(addprefix obj/, $(LIBS_OBJ))

BIN = perkele

qwik : clean perkele

all : clean glad imgui cmixer stb_vorbis libs perkele

disa : clean disassembly

perkele : $(MAIN_SRC_PATH) ; $(COMPILER_CPP) $(MAIN_SRC_PATH) $(GLAD_OBJ_PATH) $(IMGUI_OBJ_PATH) $(CMIXER_OBJ_PATH) $(STB_VORBIS_OBJ_PATH) $(LIBS_OBJ_PATH) $(CFLAGS_CPP) $(LFLAGS_CPP) -o $(BIN)

glad : $(GLAD_SRC_PATH) ; $(COMPILER_C) -c $(GLAD_SRC_PATH) $(CFLAGS_C)
	mv $(GLAD_OBJ) obj

imgui : $(IMGUI_SRC_PATH) ; $(COMPILER_CPP) -c $(IMGUI_SRC_PATH) $(CFLAGS_CPP)
	mv $(IMGUI_OBJ) obj

cmixer : $(CMIXER_SRC_PATH) ; $(COMPILER_C) -c $(CMIXER_SRC_PATH) $(CFLAGS_C)
	mv $(CMIXER_OBJ) obj

stb_vorbis : $(STB_VORBIS_SRC_PATH) ; $(COMPILER_C) -c $(STB_VORBIS_SRC_PATH) $(CFLAGS_C)
	mv $(STB_VORBIS_OBJ) obj

libs : $(LIBS_SRC_PATH) ; $(COMPILER_C) -c $(LIBS_SRC_PATH) $(CFLAGS_C)
	mv $(LIBS_OBJ) obj

clean : 
	mkdir -p obj
	rm -f perkele

disassembly : $(MAIN_SRC_PATH) ; $(COMPILER_CPP) -S $(CFLAGS_CPP) $(MAIN_SRC_PATH)
