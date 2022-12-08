#include <iostream>
#include <time.h>

#include "bass.h"

#include "app.h"
#include "logger.h"

#include "audio_player.h"

namespace ushine
{
	namespace core 
	{


	//private interfaces

	class audio_player::pimpl {
	public:
		application* app;
	private:
	};

	class audio_stream::pimpl 
	{
	public:

		bool is_valid;
		HSTREAM stream_channel;	
	private:

	};


	//audio player handlers

	audio_player::audio_player() : impl(new pimpl)
	{

	}
	audio_player::~audio_player()
	{
		impl->app = nullptr;
	}

	bool audio_player::init(application* app)
	{
		impl->app = app;
		if (app == nullptr)
			return false;

		impl->app->get_logger()->LogLine(std::string("BASS Sound Player initialization..."));

		// check the correct BASS was loaded
		if (HIWORD(BASS_GetVersion())!=BASSVERSION) {

			//impl->app->get_logger()->LogLine(std::string("An incorrect version of BASS was loaded"));
			return false;
		}

		if (!BASS_Init(-1,44100,BASS_DEVICE_8BITS,0,NULL)) {
			//impl->app->get_logger()->LogLine(std::string("Can't initialize device" ));
			return false;
		}
		BASS_SetConfig(BASS_CONFIG_UPDATETHREADS, 5);

		return true;
		
	}
	void audio_player::close()
	{
		BASS_Free(); // close output
	}

	bool audio_player::load(audio_stream& stream, std::string path)
	{

		//audio_stream stream;

		stream.impl->stream_channel =
			BASS_StreamCreateFile(FALSE,path.c_str(),0,0,0);

		if (stream.impl->stream_channel)
		{
			stream.impl->is_valid = true;
			impl->app->get_logger()->LogLine(std::string("Audio Stream loaded successfully: ")+path);
			return true;
		} else
		{
			stream.impl->is_valid = false;
			impl->app->get_logger()->LogLine(std::string("Audio Stream failed to load: ")+path);
			return false;
		}

		//return stream;
	}
	
	void audio_player::play(audio_stream& stream)
	{
		if (stream.impl->is_valid)
		{
			if (!BASS_ChannelPlay(stream.impl->stream_channel,false))
				impl->app->get_logger()->LogLine(std::string("Can't play the stream"));
		}
	}
	void audio_player::pause(audio_stream& stream)
	{

	}
	void audio_player::stop(audio_stream& stream)
	{
		BASS_ChannelStop(stream.impl->stream_channel);
	}

	double audio_player::get_current_pos(audio_stream& stream)
	{
		QWORD bytes = BASS_ChannelGetPosition(stream.impl->stream_channel, BASS_FILEPOS_CURRENT);
		return BASS_ChannelBytes2Seconds(stream.impl->stream_channel, bytes);
	}


	//audio stream handlers

	audio_stream::audio_stream() : impl(new pimpl)
	{}

	audio_stream::audio_stream(audio_stream& src_stream)
	{
		impl->is_valid = src_stream.impl->is_valid;
		impl->stream_channel = src_stream.impl->stream_channel;
	}

	audio_stream::~audio_stream() {}	

	bool audio_stream::is_valid()
	{
		return impl->is_valid;
	}
	void audio_stream::release()
	{
		BASS_StreamFree(impl->stream_channel);
	}


	}//::core::
} // ::ushing::
