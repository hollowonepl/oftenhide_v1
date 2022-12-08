#include <iostream>

#include "../core/core.h"
#include "../math/math.h"

#include "basic2d_color.h"
#include "basic3d_vertex.h"
#include "basic2d_context.h"
#include "basic2d_effect.h"


namespace ushine
{
	namespace renderer
	{

	class basic2d_effect::pimpl
	{
	public:
		ushine::core::application* app;
		ushine::core::texture* textures;
		
		ushine::core::texture invalid_texture;

		int tex_default_w;
		int tex_default_h;

		int textures_index;
	};


	basic2d_effect::basic2d_effect() : impl(new pimpl) {}

	basic2d_effect::~basic2d_effect() { }

	bool basic2d_effect::init(ushine::core::application* app, int textures, int w, int h)
	{
		if (textures<1)
			return false;

		impl->app = app;
		impl->tex_default_w = w;
		impl->tex_default_h = h;
		impl->textures_index = textures;

		impl->textures = new ushine::core::texture[textures];
		impl->textures[0].create(app,w,h);

		return true;
	}

	bool basic2d_effect::release()
	{
		for (int i=0; i< impl->textures_index; i++)
			impl->textures[i].release();

		delete [] impl->textures;
		return false;
	}

	ushine::core::texture& basic2d_effect::get_texture(int index)
	{
		if (index<0 || index>(impl->textures_index-1))
		{
			return impl->invalid_texture;
		}

		return impl->textures[index];
	}

	bool basic2d_effect::present(int index)
	{
		//todo: add unlocking/locking check for the texture

		impl->textures[index].blit_to_screen(impl->app);	
		return true;
	}

	};
};