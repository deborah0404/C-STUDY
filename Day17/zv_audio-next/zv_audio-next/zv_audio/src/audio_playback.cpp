
#include "audio_playback.h"
extern "C"
	{
#include "SDL.h"
	}
#include<iostream>
static Uint8* pAudio_chunk=NULL;
static Uint32 audio_len=0;
static Uint8* pAudio_pos=NULL;

#define PCM_BUFFER_SIZE (1024*2*2*2) 

audio_playback::audio_playback()
{

}

audio_playback::~audio_playback()
{

}


std::queue<zv_buffer*> gs_audio_data;
std::mutex gs_audio_data_mutex;


//音频设备回调函数
void fill_audio_pcm(void* udata, Uint8* stream, int len)
	{
	gs_audio_data_mutex.lock();
	if (gs_audio_data.size() == 0)
		{
		gs_audio_data_mutex.unlock();
		return;
		}
		
	SDL_memset(stream, 0, len);
	int offset = 0;
	while (len > 0)
		{
		if (gs_audio_data.size() == 0) { break; }
		zv_buffer* pbuffer = gs_audio_data.front();
		uint32_t cp_len = (pbuffer->_len - pbuffer->_offset > len) ? len : (pbuffer->_len - pbuffer->_offset);
		memcpy(stream + offset, pbuffer->buffer() + pbuffer->_offset, cp_len);
		//SDL_MixAudio(stream + offset, (Uint8*)pbuffer->buffer() + pbuffer->_offset, cp_len, SDL_MIX_MAXVOLUME);
		//SDL_MixAudio(stream + offset, pbuffer->buffer() + pbuffer->_offset, cp_len, SDL_MIX_MAXVOLUME);
		len -= cp_len;
		offset += cp_len;
		pbuffer->_offset += cp_len;
		if (pbuffer->_len == pbuffer->_offset)
			{
			pbuffer->unref();
			gs_audio_data.pop();
			}
		}
	gs_audio_data_mutex.unlock();
	}

bool audio_playback::start(const ZDP_AUDIO_CONFIG& config, SDL_AudioSpec* sdlAudioSpec)
{
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER))            //��ʼ����ʱ������Ƶ 
		printf("Could not initialize");

	sdlAudioSpec->freq = 44100;
	sdlAudioSpec->format = AUDIO_S16SYS;
	sdlAudioSpec->channels = config.channels;
	sdlAudioSpec->silence = 0;
	sdlAudioSpec->samples = 1024;
	sdlAudioSpec->callback = fill_audio_pcm;
	sdlAudioSpec->userdata=0;

	if (SDL_OpenAudio(sdlAudioSpec, nullptr) < 0)
		{
		std::cout << SDL_GetError() << std::endl;
		//SDL_GetError();
		printf("Can not open audio!");
		return -1;
		}
	SDL_PauseAudio(0);
	return false;
}

void audio_playback::stop()
{
	printf("Audio finish\n");
	SDL_Quit();
}

bool audio_playback::set(ZDP_AUDIO_SETTING& setting)
{

	return false;
}
bool audio_playback::play_frame(AUDIO_FRAME_PACKET* buffer, SDL_AudioSpec& audioSpec){
	int audio_buffer_size = buffer->frame_len - sizeof(AUDIO_FRAME_HEAD);

	zv_buffer* pbuffer = zv_buffer_manager::get_instance()->get_buffer(audio_buffer_size);
	if (!pbuffer) { return false; }

	//85-54=31=49
	memcpy(pbuffer->buffer(), (Uint8*)buffer->data, audio_buffer_size);
	pbuffer->_len = audio_buffer_size;
	pbuffer->_offset = 0;

	gs_audio_data_mutex.lock();
	gs_audio_data.push(pbuffer);
	gs_audio_data_mutex.unlock();
	return true;
}
