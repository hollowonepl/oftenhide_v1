#include <iostream>
#include <random>
#include <math.h>

#include "core/core.h"
#include "math/math.h"
#include "renderer/renderer.h"

#define SCREEN_W 320 //320
#define SCREEN_H 240 //240
#define RESOURCE_PATH "./textures/"

class HelloApp :
public ushine::core::application
{
public:
//	SDL_Window *win;
	ushine::core::texture tex;
	ushine::core::texture png;

	ushine::core::texture_loader_png png_loader;

	ushine::renderer::basic2d_context basic2d;

	virtual int on_init()
	{

		CreateWnd(SCREEN_W, SCREEN_H, true,true);
		basic2d.init(SCREEN_W, SCREEN_H);
		png_loader.init(this);

		bool textLoaded = tex.create(this, SCREEN_W,SCREEN_H);
		std::string imagePath = getResourcePath() + "az0!_envmap12_06.png";
		textLoaded = textLoaded & png_loader.load(png, imagePath);

		if (!textLoaded)
		{
			get_logger()->LogLine(std::string("Texture load failed!"));
			CloseWnd();
			return -1;
		}

		return 0;
	}

	virtual int on_close()
	{
		tex.release();
		CloseWnd();
		return 0;
	}

	int animation = 0;
	float additive_progress=0;
	float first_frame_tick=0;

	float noise_seed(ushine::math::base_vector_2<float> co){
		ushine::math::base_vector_2<float> seed_v;
		seed_v.x = 12.9898;
		seed_v.y = 78.233;
		float dot_co = co.dot(seed_v);
		//
		dot_co = sin(dot_co) * 43758.5453;
		return dot_co-floor(dot_co);
	}

	virtual int tv_snowing_effect(float timestamp)
	{
		additive_progress = timestamp;

		float animation_time = (additive_progress-first_frame_tick)/2000;

		float w = tex.get_width();
		float h = tex.get_height();
		float png_w = png.get_width();
		float png_h = png.get_height();

		int* buf = (int*)tex.lock();
		int* texture = (int*)png.lock();

		//basic2d.clear(buf, 255, 0, 0, 0);

		if (buf == nullptr)
			return -1;

		for (int i=0; i<w; i++)
			for (int j=0; j<h; j++)
			{
				ushine::math::base_vector_2<float> uv;

				uv.x = -1+2.0*i/w;
				uv.y = -1+2.0*j/h;

				//tv_snowing
				float ra=noise_seed(uv*animation_time)-.5;

				ushine::math::base_vector_2<float> seed_v;
				seed_v.set(uv.y, uv.y);
				float ral=noise_seed((seed_v*30.0f).integral()*animation_time+1.0f)-.5;

				float col=0.25;
				col=col*.2+(.3+ra+ral*.5)
					* ushine::math::modulo<float>(uv.y+animation_time*2.,.045); //0.45 tv snowing lines power

    			ushine::renderer::color_f total;
    			total.r = fabs(col*sin(animation_time/2)*2);
    			total.g = fabs(col*sin(animation_time/2)*2);
    			total.b = fabs(col*sin(animation_time/2)*2);
    			total.a = 1.0;

				int dst_argb = total.to_argb();
				buf[i+j*(int)w]=dst_argb;

			}
		png.unlock();
		tex.unlock();
		tex.blit_to_screen(this);
		return 0;
	}


	virtual int on_render(float timestamp)
	{
		if (first_frame_tick==0)
			first_frame_tick=timestamp;
			
		tv_snowing_effect(timestamp);
		return 0;
	}

	virtual int on_event()
	{
		return 0;
	}

private:

	std::string getResourcePath()
	{
		return std::string(RESOURCE_PATH);
	}
};



int main(int argc, char** argv)
{
	HelloApp app;

	if (app.Init()!=0)
		return 0;

	app.Run();
	app.Close();

	return 0;
}
