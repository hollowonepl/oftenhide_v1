
#include <iostream>
#include <sstream>

#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

#include "app.h"
#include "texture.h"

namespace ushine
{
	namespace core
	{
		class texture::pimpl
		{
		public:

			int width;
			int height;

			bool initialized = false;

			SDL_Texture *tex;
			SDL_Surface *surface;

			std::string get_format(int format)
			{
				switch (format)
				{
					case SDL_PIXELFORMAT_UNKNOWN:
					return std::string("SDL_PIXELFORMAT_UNKNOWN");
					break;
					case SDL_PIXELFORMAT_INDEX1LSB:
					return std::string("SDL_PIXELFORMAT_INDEX1LSB");
					break;
					case SDL_PIXELFORMAT_INDEX1MSB:
					return std::string("SDL_PIXELFORMAT_INDEX1MSB");
					break;
					case SDL_PIXELFORMAT_INDEX4LSB:
					return std::string("SDL_PIXELFORMAT_INDEX4LSB");
					break;
					case SDL_PIXELFORMAT_INDEX4MSB:
					return std::string("SDL_PIXELFORMAT_INDEX4MSB");
					break;
					case SDL_PIXELFORMAT_INDEX8:
					return std::string("SDL_PIXELFORMAT_INDEX8");
					break;
					case SDL_PIXELFORMAT_RGB332:
					return std::string("SDL_PIXELFORMAT_RGB332");
					break;
					case SDL_PIXELFORMAT_RGB444:
					return std::string("SDL_PIXELFORMAT_RGB444");
					break;
					case SDL_PIXELFORMAT_RGB555:
					return std::string("SDL_PIXELFORMAT_RGB555");
					break;
					case SDL_PIXELFORMAT_BGR555:
					return std::string("SDL_PIXELFORMAT_BGR555");
					break;
					case SDL_PIXELFORMAT_ARGB4444:
					return std::string("SDL_PIXELFORMAT_ARGB4444");
					break;
					case SDL_PIXELFORMAT_RGBA4444:
					return std::string("SDL_PIXELFORMAT_RGBA4444");
					break;
					case SDL_PIXELFORMAT_ABGR4444:
					return std::string("SDL_PIXELFORMAT_ABGR4444");
					break;
					case SDL_PIXELFORMAT_BGRA4444:
					return std::string("SDL_PIXELFORMAT_BGRA4444");
					break;
					case SDL_PIXELFORMAT_ARGB1555:
					return std::string("SDL_PIXELFORMAT_ARGB1555");
					break;
					case SDL_PIXELFORMAT_RGBA5551:
					return std::string("SDL_PIXELFORMAT_RGBA5551");
					break;
					case SDL_PIXELFORMAT_ABGR1555:
					return std::string("SDL_PIXELFORMAT_ABGR1555");
					break;
					case SDL_PIXELFORMAT_BGRA5551:
					return std::string("SDL_PIXELFORMAT_BGRA5551");
					break;
					case SDL_PIXELFORMAT_RGB565:
					return std::string("SDL_PIXELFORMAT_RGB565");
					break;
					case SDL_PIXELFORMAT_BGR565:
					return std::string("SDL_PIXELFORMAT_BGR565");
					break;
					case SDL_PIXELFORMAT_RGB24:
					return std::string("SDL_PIXELFORMAT_RGB24");
					break;
					case SDL_PIXELFORMAT_BGR24:
					return std::string("SDL_PIXELFORMAT_BGR24");
					break;
					case SDL_PIXELFORMAT_RGB888:
					return std::string("SDL_PIXELFORMAT_RGB888");
					break;
					case SDL_PIXELFORMAT_RGBX8888:
					return std::string("SDL_PIXELFORMAT_RGBX8888");
					break;
					case SDL_PIXELFORMAT_BGR888:
					return std::string("SDL_PIXELFORMAT_BGR888");
					break;
					case SDL_PIXELFORMAT_BGRX8888:
					return std::string("SDL_PIXELFORMAT_BGRX8888");
					break;
					case SDL_PIXELFORMAT_ARGB8888:
					return std::string("SDL_PIXELFORMAT_ARGB8888");
					break;
					case SDL_PIXELFORMAT_RGBA8888:
					return std::string("SDL_PIXELFORMAT_RGBA8888");
					break;
					case SDL_PIXELFORMAT_ABGR8888:
					return std::string("SDL_PIXELFORMAT_ABGR8888");
					break;
					case SDL_PIXELFORMAT_BGRA8888:
					return std::string("SDL_PIXELFORMAT_BGRA8888");
					break;
					case SDL_PIXELFORMAT_ARGB2101010:
					return std::string("SDL_PIXELFORMAT_ARGB2101010");
					break;
					case SDL_PIXELFORMAT_YV12:
					return std::string("SDL_PIXELFORMAT_YV12");
					break;
					case SDL_PIXELFORMAT_IYUV:
					return std::string("SDL_PIXELFORMAT_IYUV");
					break;
					case SDL_PIXELFORMAT_YUY2:
					return std::string("SDL_PIXELFORMAT_YUY2");
					break;
					case SDL_PIXELFORMAT_UYVY:
					return std::string("SDL_PIXELFORMAT_UYVY");
					break;
					case SDL_PIXELFORMAT_YVYU:
					return std::string("SDL_PIXELFORMAT_YVYU");
					break;
					default:
						return std::string("UNKNOWN");
				};
			}

		private:

		};


		texture::texture()  : impl(new pimpl)
		{

		}

		texture::~texture()
		{
			release();
		}

		bool texture::is_initialized()
		{
			return impl->initialized;
		}

		bool texture::create(ushine::core::application* app, int w, int h)
		{
			impl->width =w;
			impl->height=h;


		    int amask = 0xff000000;
		    int rmask = 0x00ff0000;
		    int gmask = 0x0000ff00;
		    int bmask = 0x000000ff;

			impl->surface = SDL_CreateRGBSurface(0, w, h, 32,
                                   rmask, gmask, bmask, amask);
			if (impl->surface == nullptr)
			{
				app->get_logger()->LogLine(std::string("Creating new surface failed"));
				return false;
			}


			impl->tex = SDL_CreateTexture((SDL_Renderer*)app->get_renderer(),
                               SDL_PIXELFORMAT_ARGB8888,
                               SDL_TEXTUREACCESS_STATIC, w,h);

			if (impl->tex == nullptr){
				app->get_logger()->LogLine(std::string("Creating new texture failed"));
				return false;
			}

			impl->initialized = true;
			return impl->initialized;
		}

		void texture::release()
		{
			if (!impl->initialized)
				return;
			SDL_DestroyTexture(impl->tex);
			SDL_FreeSurface(impl->surface);

			impl->initialized = false;
		}

		void* texture::lock()
		{
			if (!impl->initialized)
				return nullptr;

			SDL_Rect rect;
			rect.x = 0;
			rect.y = 0;
			rect.w = impl->width;
			rect.h = impl->height;

			void* pixelDataBuf;
			int pitch;


//old locking
//			auto returnValue = SDL_LockTexture(
//					impl->tex, &rect, & pixelDataBuf, &pitch);
//			return pixelDataBuf;
//new locking
			auto returnValue = SDL_LockSurface(impl->surface);
			return impl->surface->pixels;




//			std::stringstream strbuilder;
//
//			strbuilder << "Texture locked {" << " [res: " << impl->width << "x" << impl->height << "][pitch: " << pitch << "]}";
//			std::cout << strbuilder.str();


		}

		void* texture::get_buffer()
		{
			return impl->surface->pixels;
		}

		void texture::unlock()
		{
			if (!impl->initialized)
				return;
//old unlocking
//			SDL_UnlockTexture(impl->tex);
//new unlocking
			SDL_UnlockSurface(impl->surface);
			SDL_UpdateTexture(impl->tex, nullptr, impl->surface->pixels, impl->surface->pitch);
		}

		int texture::get_width()
		{
			if (!impl->initialized)
				return -1;
			return impl->width;

		}

		int texture::get_height()
		{
			if (!impl->initialized)
				return -1;
			return impl->height;
		}

		void texture::blit_to_screen(ushine::core::application* app)
		{
			if (!impl->initialized)
				return;
			SDL_RenderCopy((SDL_Renderer*)app->get_renderer(), impl->tex, NULL, NULL);
		}

		void texture::blit_to_screen(ushine::core::application* app, int x, int y, int w, int h)
		{
			if (!impl->initialized)
				return;

			SDL_Rect srcRect;

			srcRect.x = x;
			srcRect.y = y;
			srcRect.w = w;
			srcRect.h = h;

			SDL_RenderCopy((SDL_Renderer*)app->get_renderer(), impl->tex, &srcRect, NULL);
		}

		void texture::blit_to_texture(texture& tex, int alpha)
		{
			int* dest = (int*)tex.lock();
			blit_to_buf(dest,0,0, tex.get_width(), tex.get_height(), alpha);
			tex.unlock();

		}

		void texture::blit_from_buf(void* buf, int w, int h, int alpha)
		{
			int* dest = (int*)lock();
			int* src = (int*)buf;

			for (auto i=0; i<w; i++)
				for (auto j=0; j<h; j++)
				{
					if (alpha<255)
					{
						int src_col = src[i+j*w];
						int dst_col = dest[i+j*get_width()];
						int dst_alpha = 255-alpha;

						int src_b = (src_col & 0xff)* alpha >> 8;
						src_b += (dst_col & 0xff)* dst_alpha >> 8;
						int src_g = ((src_col >>8) & 0xff)* alpha >> 8;
						src_g += ((dst_col >>8) & 0xff)* dst_alpha >> 8;
						int src_r = ((src_col >>16) & 0xff)* alpha >> 8;
						src_r += ((dst_col >>16) & 0xff)* dst_alpha >> 8;
						int src_a = ((src_col >>24) & 0xff)* alpha >> 8;

						int melted_col = TO_ARGB(255, src_r, src_g,src_b);
						dest[i+j*get_width()] = melted_col;
					} else dest[i+j*get_width()]=src[i+j*w];
				}
			unlock();
		}

		void texture::blit_to_buf(void* buf, int x, int y, int w, int h, int alpha)
		{
			int* src = (int*)lock();
			int* dest = (int*)buf;

			int clip_w,clip_h;
			int clip_x,clip_y;

			clip_x = x;
			clip_y = y;

			if (x<0)
			{
				clip_x=abs(x);
			}
			if (y<0)
				clip_y=abs(y);

			clip_w = get_width();
			clip_h = get_height();

/*
			std::cout << "BLIT TEXTURE [src: siz=" << get_width() << "," << get_height() << ";"
					  << "clip_pos" << clip_x << "," << clip_y << ";"
					  << "clip_siz" << clip_w << "," << clip_h << ";]"
					  << "[dst: siz=" << w << "," << h << ";"
					  << "pos =" << x << "," << y << ";]"
					  << std::endl;
*/

			for (auto i=0; i<get_width(); i++)
				for (auto j=0; j<get_height(); j++)
				{
					int _blit_x = i+x;
					int _blit_y = j+y;

					if (_blit_x > w-1)
						continue;
					if (_blit_y > h-1)
						continue;

					if (_blit_x < 0)
						continue;
					if (_blit_y < 0)
						continue;


					int pixel_alpha = alpha;

					int src_col = src[i+j*get_width()];
					int src_alpha = (src_col >> 24) & 255;

					if (src_alpha<255)
					{
						pixel_alpha = (src_alpha*alpha)/255;
					}



					if (pixel_alpha<255)
					{
						int dst_col = dest[_blit_x+_blit_y*w];
						int dst_alpha = 255-pixel_alpha;

						int src_b = (src_col & 0xff)* pixel_alpha >> 8;
						src_b += (dst_col & 0xff)* dst_alpha >> 8;
						int src_g = ((src_col >>8) & 0xff)* pixel_alpha >> 8;
						src_g += ((dst_col >>8) & 0xff)* dst_alpha >> 8;
						int src_r = ((src_col >>16) & 0xff)* pixel_alpha >> 8;
						src_r += ((dst_col >>16) & 0xff)* dst_alpha >> 8;
						int src_a = ((src_col >>24) & 0xff)* pixel_alpha >> 8;

						int melted_col = TO_ARGB(255, src_r, src_g,src_b);
						dest[_blit_x+_blit_y*w] = melted_col;
					} else dest[_blit_x+_blit_y*w] =src_col;
				}
			unlock();
		}


	};
};
