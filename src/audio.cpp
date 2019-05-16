// TODO add wrappers so that only audio.cpp directly accesses cmixer. Although probably useless if I stay on unity build.

static void audio_callback(void *udata, Uint8 *stream, int size)
{
	cm_process((cm_Int16*) stream, size / 2);
	//cm_process((void*) stream, size / 2);
}

static void lock_handler(cm_Event *e)
{
	if (e->type == CM_EVENT_LOCK)
	{
		SDL_LockMutex(audio_mutex);
	}
	if (e->type == CM_EVENT_UNLOCK)
	{
		SDL_UnlockMutex(audio_mutex);
	}
}

#define SFX_SPELLCASTER_A 200
#define SFX_SPELLCASTER_B 201
#define SFX_SPELLCASTER_C 202
#define SFX_SPELLCASTER_D 203
#define SFX_SPELLCASTER_E 204

#define SFX_HERO_CHARGE 48
#define SFX_HERO_DEATH 49

#define SFX_UNIT_ATTACK_A 23
#define SFX_UNIT_ATTACK_B 24

#define SFX_UNIT_MOVE 104
#define SFX_UI_CLICK_B 103

#define SFX_UI_CLICK_A 101

void PlaySfx(int id)
{
	if (audio_enabled)
	{
		for (int i = 0; i < 37; i++)
		{
			if (sfx[i].id == id)
			{
				cm_play(sfx[i].src);
			}
		}   
	}
}

int LoadMusicAndSounds()
{
	// TODO: check if the sound effects are loaded too

	// SOUDNS
	const int sfx_ids[37] = {
		100, 101, 102, 103, 104,
		 10,  12,  13,  16,  17,
		  1, 200, 201, 202, 203,
		204,  22,  23,  24,  26,
		 27,  28,  29,   2,  31,
		 32,  35,   3,  41,  47,
		 48,  49,   4,   6,   7,
		  8,   9,
	};

	const char *sfx_src_filenames[37] = {
		"data/sfx/100.wav",
		"data/sfx/101.wav",
		"data/sfx/102.wav",
		"data/sfx/103.wav",
		"data/sfx/104.wav",
		"data/sfx/10.wav",
		"data/sfx/12.wav",
		"data/sfx/13.wav",
		"data/sfx/16.wav",
		"data/sfx/17.wav",
		"data/sfx/1.wav",
		"data/sfx/200.wav",
		"data/sfx/201.wav",
		"data/sfx/202.wav",
		"data/sfx/203.wav",
		"data/sfx/204.wav",
		"data/sfx/22.wav",
		"data/sfx/23.wav",
		"data/sfx/24.wav",
		"data/sfx/26.wav",
		"data/sfx/27.wav",
		"data/sfx/28.wav",
		"data/sfx/29.wav",
		"data/sfx/2.wav",
		"data/sfx/31.wav",
		"data/sfx/32.wav",
		"data/sfx/35.wav",
		"data/sfx/3.wav",
		"data/sfx/41.wav",
		"data/sfx/47.wav",
		"data/sfx/48.wav",
		"data/sfx/49.wav",
		"data/sfx/4.wav",
		"data/sfx/6.wav",
		"data/sfx/7.wav",
		"data/sfx/8.wav",
		"data/sfx/9.wav",
	};

	for (int i = 0; i < 37; i++)
	{
		sfx[i].id = sfx_ids[i];
		sfx[i].src = cm_new_source_from_file(sfx_src_filenames[i]);
	}

	music_src = cm_new_source_from_file("data/music/hey.ogg");
	// music_src = cm_new_source_from_file("data/music/junkala_title.wav");
	if (!music_src) {
		fprintf(stderr, "Error: failed to create source '%s'\n", cm_get_error());
		return 0;
	}

	return 1;
}

void UnloadMusicAndSounds()
{
	if (audio_enabled)
	{
		cm_destroy_source(music_src);
		for (int i = 0; i < 37; i++)
		{
			cm_destroy_source( sfx[i].src );
		}
	}
}

void StartMusic()
{
	if (audio_enabled)
	{
		cm_set_loop(music_src, 1);
		cm_play(music_src);
	}
}
