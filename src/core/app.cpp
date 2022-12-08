#include <iostream>
#include <time.h>

#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#else
#include <SDL.h>
#include <SDL_timer.h>
#endif

#include "app.h"
#include "logger.h"


namespace ushine
{
	namespace core
	{
		class application::pimpl
		{
		public:
			Logger Log;

			SDL_Window *win;
			SDL_Renderer *ren;
			SDL_Surface *surface;

			int window_width;
			int window_height;
			//SDL_Texture *tex;
		};

		application::application() : impl(new pimpl)
		{
		}

		application::~application()
		{
		}

		int application::Init()
		{
			impl->Log.LogLine(std::string("Application Init Start"));

			if (SDL_Init(SDL_INIT_VIDEO) != 0){
				impl->Log.LogLine(std::string("SDL_Init failed: ") + SDL_GetError());
				return 1;
			}
			/*

			if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
				impl->Log.LogLine(std::string("IMG_Init failed: ") + SDL_GetError());
				SDL_Quit();
				return 1;
			}
			*/

			auto initEventResult = on_init();

			if (initEventResult!=0)
				return initEventResult;

			impl->Log.LogLine(std::string("Application Init End"));

			return 0;
		}

		int application::Close()
		{
			impl->Log.LogLine(std::string("Application Close Start"));
			//IMG_Quit();
			SDL_Quit();
			on_close();
			impl->Log.LogLine(std::string("Application Close End"));

			exit(0);

			return 0;
		}

		int application::Run()
		{
			SDL_Event e;
			bool quit = false;
			while (!quit){
				while (SDL_PollEvent(&e)){

					//impl->Log.LogLine(std::string("SDL_Event: ") + std::to_string(e.type));

					if (e.type == SDL_QUIT){
						quit = true;
					}
					if (e.type == SDL_KEYDOWN){
						if (e.key.keysym.sym == SDLK_ESCAPE)
							quit = true;
					}
					if (e.type == SDL_MOUSEBUTTONDOWN){
						quit = true;
					}
				}


				if (!quit)
				{
					float timestamp = SDL_GetTicks();

					// SDL_GetTicks();
					//clock()/(CLOCKS_PER_SEC/25);

					//SDL_RenderClear(impl->ren);
					on_render(timestamp);

					SDL_RenderPresent(impl->ren);
					//SDL_UpdateWindowSurface(impl->win);
				}

			}

			return 0;
		}

		void* application::get_event()
		{
			return nullptr;
		}

		Logger* application::get_logger()
		{
			return &impl->Log;
		}

		void* application::get_renderer()
		{
			return (void*)impl->ren;
		}

		int application::window_width()
		{
			return impl->window_width;
		}
		int application::window_height()
		{
			return impl->window_height;
		}

		void application::clear(int a, int r, int g, int b)
		{
			SDL_SetRenderDrawColor(impl->ren, r, g, b, a);
			SDL_RenderClear(impl->ren);
		}

		void application::CreateWnd(int w, int h, bool vsync, bool fullscreen)
		{
			impl->win = SDL_CreateWindow("OftenHide Demo System v.1.0", 0, 0, w, h,
				SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

			if (impl->win == nullptr) {

				this->get_logger()->LogLine(
					 std::string("SDL_CreateWindow Error: ") + SDL_GetError()
					);
				SDL_Quit();
				return;
			}

        	//impl->surface = SDL_GetWindowSurface(impl->win);
        	//impl->ren = SDL_CreateSoftwareRenderer(impl->surface);


			//if (vsync)
				impl->ren = SDL_CreateRenderer(impl->win, -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC );
			//else
			//	impl->ren = SDL_CreateRenderer(impl->win, -1, SDL_RENDERER_SOFTWARE ); //

			if (impl->ren == nullptr) {

				std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
				CloseWnd();
				return;
			}

			impl->window_width = w;
			impl->window_height = h;

			if (fullscreen)
			{
				//SDL_MaximizeWindow(impl->win);
				SDL_SetWindowFullscreen(impl->win, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}

			SDL_ShowCursor(0);


		}
		void application::CloseWnd()
		{

			SDL_DestroyRenderer(impl->ren);
			SDL_DestroyWindow(impl->win);
			SDL_Quit();
		}


	} //namespace 	 ::core
}; //namespace ushine::
