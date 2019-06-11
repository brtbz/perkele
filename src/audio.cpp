// TODO add wrappers so that only audio.cpp directly accesses cmixer.

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

#define SFX_UNIT_MOVE 0
#define SFX_UI_CLICK_A 1
#define SFX_GOBLIN_ROAR 2

void PlaySfx(int id)
{
	if (audio_enabled)
	{
		for (int i = 0; i < 3; i++)
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

	sfx[0].id = 0;
	sfx[0].src = cm_new_source_from_file("data/sfx/swing.ogg");

	sfx[1].id = 1;
	sfx[1].src = cm_new_source_from_file("data/sfx/metal-small1.ogg");

	sfx[2].id = 2;
	sfx[2].src = cm_new_source_from_file("data/sfx/mnstr1.ogg");

	music_src = cm_new_source_from_file("data/music/hey.ogg");
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
/*
		cm_stop(music_src);
		for (int i = 0; i < 37; i++)
		{
			cm_stop( sfx[i].src );
		}

		cm_destroy_source(music_src);
		for (int i = 0; i < 37; i++)
		{
			cm_destroy_source( sfx[i].src );
		}
*/
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
