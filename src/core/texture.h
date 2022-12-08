

#ifndef USHINE_CORE_TEXTURE
#define USHINE_CORE_TEXTURE

#define TO_ARGB(a,r,g,b) (a << 24) + (r << 16) + (g << 8) + b

namespace ushine
{
	namespace core
	{
		class application;

		class DYNLIB_EXPORT texture {
		public:
			texture();
			virtual ~texture();

			bool is_initialized();
			bool create(ushine::core::application* app, int w, int h);
			//bool load(ushine::core::application* app, const char* path);

			void release();

			void* lock();
			void* get_buffer();
			void unlock();

			int get_width();
			int get_height();

			void blit_to_screen(ushine::core::application* app);
			void blit_to_screen(ushine::core::application* app, int x, int y, int w, int h);
			void blit_to_texture(texture& tex, int alpha=255);
			void blit_to_buf(void* buf, int x, int y, int w, int h, int alpha=255);
			void blit_from_buf(void* buf, int w, int h, int alpha=255);

		private:
			class pimpl;
			std::unique_ptr<pimpl> impl;

		};

		class DYNLIB_EXPORT texture_loader
		{
		public:
			application* app;
			inline virtual void init(application* a0) { app= a0;}
			virtual bool load(texture& tex, std::string& path) = 0;
		};

	}
}


#endif
