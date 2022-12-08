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
		std::string imagePath = getResourcePath() + "az0!_envmap12_01.png";
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

	int colElement = 0;
	int colElementDir = 1;
	int animation = 0;	
	
	float first_time_tick=0;

	float additive_progress=0;

	virtual int tunnel_test(float timestamp)
	{
		if (first_time_tick==0)	first_time_tick = timestamp;

        additive_progress = timestamp;
        float animation_time = (additive_progress-first_time_tick)/1000;

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
		move1.x = cos(animation_time*1)*0.4;
		move1.y = sin(cos(animation_time*2.5))*0.4;		

		for (int i=0; i<w; i++)
			for (int j=0; j<h; j++)
			{

				ushine::math::base_vector_2<float> p, uv;
				p.x = -1+2.0*i/w;//+1.0;
				p.y = -1+2.0*j/h;//+move1.y;

				float a = atan2(p.y, p.x);
				float r = sqrt(p.dot(p))/2; //2

				////
				uv.x = 0.1*p.x/(0.11+r*0.8)+animation_time*0.2;
				uv.y = 0.1*p.y/(0.11+r*1.5)+animation_time*0.2;

				/////normalizing the uv
				int uvx = abs((int)(uv.x*(png_w-1))%255);
				int uvy = abs((int)(uv.y*(png_h-1))%255);
	

				ushine::renderer::color_f src_col;

				int src_argb = texture[uvx+uvy*png.get_width()];

				src_col.from_argb(src_argb);
				
				
				///dim the center
				src_col.r*=(1-r);
				src_col.g*=(1-r);
				src_col.b*=(1-r);						
						

				int dst_argb = src_col.to_argb();

				//if (src_col.r>0.1 && src_col.g>0.1 && src_col.b >0.1 )
				buf[i+j*(int)w]=dst_argb; //src_argb;
			}

		//printf("frame end\n");

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