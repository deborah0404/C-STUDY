
#ifndef _AUDIO_PLAYBACK_H_
#define _AUDIO_PLAYBACK_H_


#include "zva_common.h"
#include "zv_buffer.h"
#include "SDL.h"

class audio_playback
{
public:
	audio_playback();
	~audio_playback();
	SDL_AudioSpec sdlAudioSpec;

	bool start(const ZDP_AUDIO_CONFIG&,SDL_AudioSpec);
	bool start(const ZDP_AUDIO_CONFIG& config, SDL_AudioSpec& sdlAudioSpec);
	bool start(const ZDP_AUDIO_CONFIG& config, SDL_AudioSpec* sdlAudioSpec);
	void stop();

	bool set(ZDP_AUDIO_SETTING&);
	bool play_frame(AUDIO_FRAME_PACKET*, SDL_AudioSpec&);
};


#endif	// _AUDIO_PLAYBACK_H_
