#include <iostream>
#include <random>
#include <math.h>

#include "core/core.h"
#include "math/math.h"
#include "renderer/renderer.h"

#define SCREEN_W 320 //320
#define SCREEN_H 240 //240

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
		std::string imagePath = getResourcePath() + "BlueChipWall.png";
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
	float first_frame=-1;

	virtual int tunnel_test(float timestamp)
	{
		if (first_frame==-1) first_frame=timestamp;
		additive_progress = (timestamp-first_frame)/700;

		float w = tex.get_width();
		float h = tex.get_height();
		float png_w = png.get_width();
		float png_h = png.get_height();

		int* buf = (int*)tex.lock();
		int* texture = (int*)png.lock();

		//basic2d.clear(buf, 255, 0, 0, 0);

		if (buf == nullptr)
			return -1;

		ushine::math::base_vector_2<float>
					move1;
		move1.x = cos(sin(additive_progress))*0.4;
		move1.y = sin(additive_progress*1.5)*0.4;


		for (int i=0; i<w; i++)
			for (int j=0; j<h; j++)
			{


				ushine::math::base_vector_2<float> p, uv;

				p.x = -1+2.0*i/w;
				p.y = -1+2.0*j/h;

			    float r = pow( pow(p.x*p.x,4.0) + pow(p.y*p.y,2.8), 1.0/6.0 );
			    uv.x = .5*additive_progress + 0.5/r;
			    uv.y = 1.0*atan2(p.y,p.x)/3.1416;

				int uvx = abs((int)(uv.x*(png_w-1))%255);
				int uvy = abs((int)(uv.y*(png_h-1))%255);

				ushine::renderer::color_f src_col;

				int src_argb = texture[uvx+uvy*png.get_width()];

				src_col.from_argb(src_argb);
				////image process the effect

					src_col.r*=r;
					src_col.g*=r;
					src_col.b*=r;


				int dst_argb = src_col.to_argb();
				buf[i+j*(int)w]=dst_argb;
			}
		png.unlock();
		tex.unlock();
		tex.blit_to_screen(this);
		return 0;
	}


	virtual int on_render(float timestamp)
	{


		tunnel_test(timestamp);

		return 0;
	}

	virtual int on_event()
	{
		return 0;
	}

private:

	std::string getResourcePath()
	{
		return std::string("./textures/");
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
