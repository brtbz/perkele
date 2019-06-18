@echo off
:: This is supposed to be the same as the build.bat but it skips building the bundled libraries

SETLOCAL
SET SDL2_INCLUDE_DIR=..\SDL2-2.0.9\include\
SET SDL2_LIB_DIR=..\SDL2-2.0.9\lib\x64\
SET OBJ_DIR=obj\
SET SRC_DIR=src\
SET LIBS_DIR=src\libs\
SET OPTIM_LVL=/Od

:perkele
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
