/*
Display modes:
--fake fullscreen (borderless windowed with primary display's current resolution) SDL seems to handle this one best. default perhaps
--real fullscreen (SDL seems to be unable to switch to other resolutions though it can report them (might be Linux (and Nvidia?) specific disability))
--windowed (with window decoration). This has some mouse y-offset bug with some versions of SDL and possibly xfce's display manager (LightDM?) (or possibly compositor??)
  should allow all kinds of resolutions for this one (within reasonable limits) (640x360 7680×4320?)
List available display resolutions as reported by SDL, but have input boxes where you can enter some arbitrary values for x and y (within limits)
This list can be quite big (got 109 entries) in some cases and include non-working modes.

How to handle situations where settings file has options that produce black screen? (Tell user to delete the settings file in readme.txt?)
or have that confirm box with 15 second timer?

Ignore multi-monitor setups for now (just use the primary display)

Audio options:
sound effects on/off
music on/off
master gain
sfx gain
music gain

Dev options:
show debug window
bypass main menu on startup
*/

/*
; GRAPHICS
; 0 = fake fullscreen (aka borderless windowed), 1 = real fullscreen, 2 = windowed
screen_mode = 0 [0 = fakefullscreen, 1 = realfullscreen, 2 = windowed]

; screen width (ignored if screen_mode is fake fullscreen, allowed values from 640 to 7680)
screen_w = 1280 [640...7680]

; screen height (ignored if screen_mode is fake fullscreen, allowed values from 360 to 4320)
screen_h = 720 [360...4320]

; true or false, only way to limit frame rate currently
vsync = true [true, false]

; AUDIO
sfx = true

music = true
master_gain = 1.0
music_gain = 0.5
sfx_gain = 1.0

; DEV
show_debug_window = true
bypass_main_menu = true
*/

void DefaultConfigValues(PerkeleConfigs *pc)
{
	pc->screen_mode = 0;
	pc->screen_w = 1280;
	pc->screen_h = 720;
	pc->enable_vsync = true;
	pc->enable_music = true;
	pc->enable_sfx = true;
	pc->master_gain = 1.0f;
	pc->music_gain = 0.5f;
	pc->sfx_gain = 1.0f;
	pc->enable_debug_window = false;
	pc->bypass_main_menu = false;
};

void WriteConfigsToFile(PerkeleConfigs *pc, const char *file_name)
{
	char temp_file[1024];
	int write_head = 0;

	int LINE_MAX = 80;

	FILE *fp = fopen(file_name, "w");
	write_head += snprintf(&temp_file[write_head], LINE_MAX, "# Perkele configuration file\n# This line is a comment\n\n");

	write_head += snprintf(&temp_file[write_head], LINE_MAX, "# GRAPHICS\n");
	write_head += snprintf(&temp_file[write_head], LINE_MAX, "screen_mode=%d\n", pc->screen_mode);
	write_head += snprintf(&temp_file[write_head], LINE_MAX, "screen_w=%d\n", pc->screen_w);
	write_head += snprintf(&temp_file[write_head], LINE_MAX, "screen_h=%d\n", pc->screen_h);
	write_head += snprintf(&temp_file[write_head], LINE_MAX, "enable_vsync=%d\n", pc->enable_vsync);

	write_head += snprintf(&temp_file[write_head], LINE_MAX, "\n# AUDIO\n");
	write_head += snprintf(&temp_file[write_head], LINE_MAX, "enable_music=%d\n", pc->enable_music);
	write_head += snprintf(&temp_file[write_head], LINE_MAX, "enable_sfx=%d\n", pc->enable_sfx);
	write_head += snprintf(&temp_file[write_head], LINE_MAX, "master_gain=%.2f\n", pc->master_gain);
	write_head += snprintf(&temp_file[write_head], LINE_MAX, "music_gain=%.2f\n", pc->music_gain);
	write_head += snprintf(&temp_file[write_head], LINE_MAX, "sfx_gain=%.2f\n", pc->sfx_gain);

	write_head += snprintf(&temp_file[write_head], LINE_MAX, "\n# DEV\n");
	write_head += snprintf(&temp_file[write_head], LINE_MAX, "enable_debug_window=%d\n", pc->enable_debug_window);
	write_head += snprintf(&temp_file[write_head], LINE_MAX, "bypass_main_menu=%d\n", pc->bypass_main_menu);

	fwrite(&temp_file[0], 1, write_head, fp);
	fclose(fp);
}

int ReadConfigsFromFile(PerkeleConfigs *pc, const char *file_name)
{
	const char* key_strings[] = {
		"screen_mode",
		"screen_w",
		"screen_h",
		"enable_vsync",
		"enable_music",
		"enable_sfx",
		"master_gain",
		"music_gain",
		"sfx_gain",
		"enable_debug_window",
		"bypass_main_menu"
	};
	
	typedef enum CFG_KEY_STRING
	{
		SCREEN_MODE,
		SCREEN_W,
		SCREEN_H,
		ENABLE_VSYNC,
		ENABLE_MUSIC,
		ENABLE_SFX,
		MASTER_GAIN,
		MUSIC_GAIN,
		SFX_GAIN,
		ENABLE_DEBUG_WINDOW,
		BYPASS_MAIN_MENU,
		KEY_STRING_COUNT,
	} CFG_KEY_STRING;
	
	FILE *fp = fopen(file_name, "r");
	if ( fp == NULL )
	{
		return -1; // no file :'(
	}
	fseek(fp, 0, SEEK_END);
	int file_size = ftell(fp);
	if (file_size >= 1024)
	{
		return -2; // too big file! I refuse to read it!
	}
	fseek(fp, 0, SEEK_SET);

	char temp_line[80];

	while (fgets(temp_line, 80, fp))
	{
		if (temp_line[0] != '#' && temp_line[0] != ' ' && temp_line[0] != '\n')
		{
			char *token = strtok(temp_line, "=");

			for (int i = 0; i < KEY_STRING_COUNT; i++)
			{
				if (strcmp( key_strings[i], token ) == 0 )
				{
					// IT*S A HIT! so get the value
					token = strtok(NULL, "=");

					// and save it in the right place
					switch (i)
					{
						case SCREEN_MODE: 
							pc->screen_mode = atoi(token);
							break;

						case SCREEN_W:
							pc->screen_w = atoi(token);
							break;

						case SCREEN_H:
							pc->screen_h = atoi(token);
							break;

						case ENABLE_VSYNC:
							pc->enable_vsync = (bool)atoi(token);
							break;

						case ENABLE_MUSIC:
							pc->enable_music = (bool)atoi(token);
							break;

						case ENABLE_SFX:
							pc->enable_sfx = (bool)atoi(token);
							break;

						case MASTER_GAIN:
							pc->master_gain = atof(token);
							break;

						case MUSIC_GAIN:
							pc->music_gain = atof(token);
							break;

						case SFX_GAIN:
							pc->sfx_gain = atof(token);
							break;

						case ENABLE_DEBUG_WINDOW:
							pc->enable_debug_window = (bool)atoi(token);
							break;

						case BYPASS_MAIN_MENU:
							pc->bypass_main_menu = (bool)atoi(token);
							break;

						default :
							break;
					}
				}
			}
		}
	}
	fclose(fp);
}

void ValidateConfigs(PerkeleConfigs *pc)
{
	if ( !ValueWithinRange( pc->screen_mode, 0, 2 )) { pc->screen_mode = 0; }
	pc->screen_w = ClampValueToRange( pc->screen_w, 640, 7680 );
	pc->screen_h = ClampValueToRange( pc->screen_h, 360, 4320 );
	if ( !ValueWithinRange( pc->enable_vsync, 0, 1 )) { pc->enable_vsync = 1; }
	if ( !ValueWithinRange( pc->enable_music, 0, 1 )) { pc->enable_music = 1; }
	if ( !ValueWithinRange( pc->enable_sfx, 0, 1 )) { pc->enable_sfx = 1; }
	pc->master_gain = ClampValueToRangeF( pc->master_gain, 0.0f, 2.0f );
	pc->music_gain = ClampValueToRangeF( pc->music_gain, 0.0f, 2.0f );
	pc->sfx_gain = ClampValueToRangeF( pc->sfx_gain, 0.0f, 2.0f );
	if ( !ValueWithinRange( pc->enable_debug_window, 0, 1 )) { pc->enable_debug_window = 0; }
	if ( !ValueWithinRange( pc->bypass_main_menu, 0, 1 )) { pc->bypass_main_menu = 0; }
}
