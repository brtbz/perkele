@echo off
:: tested with MS Visual C/C++ 2015 build tools
:: run some vcvarsall.bat (x64) or something so that cl.exe can be found and other required variables are set
:: Point SDL2_INCLUDE_DIR to the location of SDL2 header files
:: Point SDL2_LIB_DIR to the location of SDL2.lib and SDL2main.lib

SETLOCAL
SET SDL2_INCLUDE_DIR=..\SDL2-2.0.9\include\
SET SDL2_LIB_DIR=..\SDL2-2.0.9\lib\x64\
SET OBJ_DIR=obj\
SET SRC_DIR=src\
SET LIBS_DIR=src\libs\
SET OPTIM_LVL=/Od

:glad
cl.exe %OPTIM_LVL% /c /TC %LIBS_DIR%glad.c
if %ERRORLEVEL% == 0 ( GOTO imgui )
if %ERRORLEVEL% NEQ 0 (	GOTO error_report )

:imgui
cl.exe %OPTIM_LVL% /c /TP /I%SDL2_INCLUDE_DIR% %LIBS_DIR%imgui.cpp %LIBS_DIR%imgui_impl_sdl.cpp %LIBS_DIR%imgui_impl_opengl3.cpp %LIBS_DIR%imgui_draw.cpp %LIBS_DIR%imgui_demo.cpp %LIBS_DIR%imgui_widgets.cpp
if %ERRORLEVEL% == 0 ( GOTO cmixer )
if %ERRORLEVEL% NEQ 0 (	GOTO error_report )

:cmixer
cl.exe %OPTIM_LVL% /c /TC %LIBS_DIR%cmixer.c
if %ERRORLEVEL% == 0 ( GOTO stb_vorbis )
if %ERRORLEVEL% NEQ 0 (	GOTO error_report )

:stb_vorbis
cl.exe %OPTIM_LVL% /c /TC %LIBS_DIR%stb_vorbis.c
if %ERRORLEVEL% == 0 ( GOTO implelibs )
if %ERRORLEVEL% NEQ 0 (	GOTO error_report )

:implelibs
cl.exe %OPTIM_LVL% /c /TC %LIBS_DIR%implelibs.c
if %ERRORLEVEL% == 0 ( GOTO perkele )
if %ERRORLEVEL% NEQ 0 (	GOTO error_report )

:perkele
md %OBJ_DIR%
move /y *.obj %OBJ_DIR%
cl.exe %OPTIM_LVL% /FePERKELE.EXE %SRC_DIR%perkele.cpp /I%SDL2_INCLUDE_DIR% /I%LIBS_DIR% %OBJ_DIR%cmixer.obj %OBJ_DIR%glad.obj %OBJ_DIR%imgui.obj %OBJ_DIR%imgui_demo.obj %OBJ_DIR%imgui_draw.obj %OBJ_DIR%imgui_impl_opengl3.obj %OBJ_DIR%imgui_impl_sdl.obj %OBJ_DIR%imgui_widgets.obj %OBJ_DIR%implelibs.obj %OBJ_DIR%stb_vorbis.obj %SDL2_LIB_DIR%SDL2.lib %SDL2_LIB_DIR%SDL2main.lib /link /subsystem:windows
if %ERRORLEVEL% == 0 (
echo ALL DONE!
GOTO end
)
if %ERRORLEVEL% NEQ 0 (	GOTO error_report )

:error_report
echo Build Failed!
echo ERRORLEVEL: %ERRORLEVEL%
goto end

:end
