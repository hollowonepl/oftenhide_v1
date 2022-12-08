
#ifndef USHINE_CORE_AUDIOPLAYER
#define USHINE_CORE_AUDIOPLAYER

namespace ushine
{
	namespace core 
	{

	class audio_stream;

	class audio_player {

	public:
		audio_player();
		virtual ~audio_player();

		bool init(application* app);
		void close();

		bool load(audio_stream& stream, std::string path);

		void play(audio_stream& stream);
		void pause(audio_stream& stream);
		void stop(audio_stream& stream);

		double get_current_pos(audio_stream& stream);

	private:

		class pimpl;
		std::unique_ptr<audio_player::pimpl> impl;

	}; //class ::audio_player

	class audio_stream {

	public:
		audio_stream();
		audio_stream(audio_stream& src_stream);
		virtual ~audio_stream();

		bool is_valid();
		void release();

	private:
		class pimpl;
		std::unique_ptr<audio_stream::pimpl> impl;

		friend audio_player;
	}; //class ::audio_player


	}; //namespace 	 ::core::
}; //namespace ushine::

#endif