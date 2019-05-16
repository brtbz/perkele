DelagardiSettings GetDefaultSettings()
{
	DelagardiSettings settings_default = { 1280, 720, false, false, true };
	return settings_default;
}

int32_t WriteSettingsToFile(DelagardiSettings settings)
{
	// TODO: check for errors and tell about it in return code
	char screen_width_str[6];
	char screen_height_str[6];
	char fullscreen_str[2];
	char borderless_str[2];
	char vsync_str[2];
	snprintf(&screen_width_str[0], 6, "%d", settings.screen_width);
	snprintf(&screen_height_str[0], 6, "%d", settings.screen_height);
	snprintf(&fullscreen_str[0], 2, "%d", settings.fullscreen);
	snprintf(&borderless_str[0], 2, "%d", settings.borderless);
	snprintf(&vsync_str[0], 2, "%d", settings.vsync);

	ini_t* ini = ini_create(NULL);
	ini_property_add(ini, INI_GLOBAL_SECTION, "screen_width", 0, screen_width_str, 0);
	ini_property_add(ini, INI_GLOBAL_SECTION, "screen_height", 0, screen_height_str, 0);
	ini_property_add(ini, INI_GLOBAL_SECTION, "fullscreen", 0, fullscreen_str, 0);
	ini_property_add(ini, INI_GLOBAL_SECTION, "borderless", 0, borderless_str, 0);
	ini_property_add(ini, INI_GLOBAL_SECTION, "vsync", 0, vsync_str, 0);

	int ini_size = ini_save(ini, NULL, 0);
	char *ini_data = (char*)malloc(ini_size);
	ini_size = ini_save(ini, ini_data, ini_size);
	ini_destroy(ini);

	FILE *fp = fopen(settings_file_name, "w");
	fwrite(ini_data, 1, ini_size, fp);
	fclose(fp);
	free(ini_data);
}

void DefaultSettings()
{
	settings_temp = GetDefaultSettings();
	settings_confirmed = settings_temp;

	WriteSettingsToFile(settings_confirmed);

	strncpy(settings_msg, "Defaults restored. Restart.", 32);
}

DelagardiSettings LoadSettings()
{
	DelagardiSettings settings_loaded = GetDefaultSettings();

	FILE* fp = fopen(settings_file_name, "r");
	if (fp != NULL)
	{
		printf("%s was found!\n", settings_file_name);

		fseek(fp, 0, SEEK_END);
		int size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char *ini_data = (char*)malloc(size+1);
		fread(ini_data, 1, size, fp); // check return value
		ini_data[size] = '\0';
		fclose(fp);

		ini_t* ini = ini_load(ini_data, NULL);
		free(ini_data);
		int screen_width_index = ini_find_property(ini, INI_GLOBAL_SECTION, "screen_width", 12);
		const char *screen_width_str = ini_property_value(ini, INI_GLOBAL_SECTION, screen_width_index);
		settings_loaded.screen_width = atoi(screen_width_str);

		int screen_height_index = ini_find_property(ini, INI_GLOBAL_SECTION, "screen_height", 13);
		const char *screen_height_str = ini_property_value(ini, INI_GLOBAL_SECTION, screen_height_index);
		settings_loaded.screen_height = atoi(screen_height_str);

		int fullscreen_index = ini_find_property(ini, INI_GLOBAL_SECTION, "fullscreen", 10);
		const char *fullscreen_str = ini_property_value(ini, INI_GLOBAL_SECTION, fullscreen_index);
		settings_loaded.fullscreen = (bool)atoi(fullscreen_str);

		int borderless_index = ini_find_property(ini, INI_GLOBAL_SECTION, "borderless", 10);
		const char *borderless_str = ini_property_value(ini, INI_GLOBAL_SECTION, borderless_index);
		settings_loaded.borderless = (bool)atoi(borderless_str);

		int vsync_index = ini_find_property(ini, INI_GLOBAL_SECTION, "vsync", 5);
		const char *vsync_str = ini_property_value(ini, INI_GLOBAL_SECTION, vsync_index);
		settings_loaded.vsync = (bool)atoi(vsync_str);

		ini_destroy(ini);
	}
	else
	{
		printf("Couldn't find %s!\n", settings_file_name);
		DefaultSettings();
		settings_loaded = GetDefaultSettings();
	}
	
	return settings_loaded;
}

void ValidateSettings()
{
	// TODO: replace with defaults/clamp within range if invalid
}

void SaveSettings()
{
	ValidateSettings();

	settings_confirmed = settings_temp;
	WriteSettingsToFile(settings_confirmed);

	strncpy(settings_msg, "Settings saved. Restart.", 32);
}
