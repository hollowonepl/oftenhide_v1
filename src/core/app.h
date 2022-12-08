

#ifndef USHINE_CORE_APPLICATION
#define USHINE_CORE_APPLICATION

//#include <SDL.h>
//#include <SDL_image.h>
#include "common.h"
#include "logger.h"

namespace ushine
{
	namespace core 
	{

	class DYNLIB_EXPORT application {

	public:
		application();
		virtual ~application();

		void CreateWnd(int w, int h, bool vsync=true, bool fullscreen=true);
		void CloseWnd();

		int window_width();
		int window_height();

		virtual int Init();
		virtual int Run();
		virtual int Close();

		virtual int on_init() = 0;
		virtual int on_event() = 0;
		virtual int on_render(float timestamp) = 0;
		virtual int on_close() = 0;

		void* get_event();
		Logger* get_logger();

		void* get_renderer();
		void clear(int a, int r, int g, int b);

	private:

		class pimpl;
		std::unique_ptr<pimpl> impl;

	}; //class ::Application
	}; //namespace 	 ::core::
}; //namespace ushine::

#endif