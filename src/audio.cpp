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
#define SFX_UI_CLICK_ERROR 3
#define SFX_UNIT_DEATH 4
#define SFX_UNIT_REST 5
#define SFX_SURFER_BATTLE_CRY 6

void PlaySfx(int id)
{
	if (audio_enabled)
	{
		if (cm_get_state( sfx[id].src) == CM_STATE_PLAYING)
		{
			cm_stop(sfx[id].src);
		}
		cm_play(sfx[id].src);
		/*
		for (int i = 0; i < 5; i++)
		{
			if (sfx[i].id == id)
			{
				cm_play(sfx[i].src);
			}
		}
		*/  
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

	sfx[3].id = 3;
	sfx[3].src = cm_new_source_from_file("data/sfx/interface6_short.ogg");

	sfx[4].id = 4;
	sfx[4].src = cm_new_source_from_file("data/sfx/slime8.ogg");

	sfx[5].id = 5;
	sfx[5].src = cm_new_source_from_file("data/sfx/spell.ogg");

	sfx[6].id = 6;
	sfx[6].src = cm_new_source_from_file("data/sfx/ogre1.ogg");

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
