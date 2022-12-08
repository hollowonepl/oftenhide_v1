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
	ushine::core::audio_player player;
	ushine::core::audio_stream stream;

	ushine::core::texture back_buffer;
	
	ushine::core::texture demo_effect_01_tex_01;

	ushine::core::texture_loader_png png_loader;

	ushine::renderer::basic2d_context basic2d;

	bool musicOk = false, playingOk=false;

	virtual int on_init()
	{

		CreateWnd(SCREEN_W, SCREEN_H, true,false);
		basic2d.init(SCREEN_W, SCREEN_H);	
		png_loader.init(this);

		bool textLoaded = back_buffer.create(this, SCREEN_W,SCREEN_H);

		std::string imagePath = getResourcePath() + "texture_brick.png";
		textLoaded = textLoaded & png_loader.load(demo_effect_01_tex_01, imagePath);		

		if (!textLoaded)
		{
			get_logger()->LogLine(std::string("Texture load failed!"));
			CloseWnd();
			return -1;
		}

		musicOk = player.init(this);
		if (musicOk)
		musicOk = player.load(stream, (getResourcePath()+"Argasek - Silicon Venture.mp3").c_str());		




		return 0;
	}

	virtual int on_close()
	{
		if (playingOk)
			player.stop(stream);

		player.close();

		back_buffer.release();		
		demo_effect_01_tex_01.release();

		CloseWnd();
		return 0;
	}

	int logo_shift = 520;
	int logo_idx=0;

	float global_alpha=0;

	int animation = 0;	
	float additive_progress=0;
		float first_frame = -1;

	virtual int demo_effect_01(float timestamp)
	{
		if (first_frame==-1) first_frame=timestamp;
		additive_progress = (timestamp-first_frame)/700;

		if (animation==0)
			animation = timestamp;
		
		int progress = (timestamp-animation)*10;
		animation = timestamp;		

		if (global_alpha<1.0)
		{
			float alpha_growth = (float)(timestamp)/1000/25;
			global_alpha+=alpha_growth;
			std::cout << "global_alpha: " << global_alpha << "alpha_growth: " << alpha_growth << std::endl;			
		} else 
		{
			global_alpha=1.0;
		}



		float w = back_buffer.get_width();
		float h = back_buffer.get_height();

		float png_w = demo_effect_01_tex_01.get_width();
		float png_h = demo_effect_01_tex_01.get_height();		

		int* buf = (int*)back_buffer.lock();
		int* texture = (int*)demo_effect_01_tex_01.lock();

		basic2d.clear(buf, 255, 0, 0, 0,w,h);

		if (buf == nullptr)
			return -1;

		ushine::math::base_vector_2<float> 
					move1;
		move1.x = cos(additive_progress*1)*0.4;
		move1.y = sin(additive_progress*1.5)*0.4;		

		for (int i=0; i<w; i++)
			for (int j=0; j<h; j++)
			{

				ushine::math::base_vector_2<float> p, uv;
				p.x = -1+2.0*i/w+move1.x;
				p.y = -1+2.0*j/h+move1.y;

				float a = atan2(p.y, p.x);
				float r = sqrt(p.dot(p))/2;

				////twist
				uv.x = r - 0.25*additive_progress;
				uv.y = cos(a*5.0+2.0*sin(additive_progress+2.2*r));

				/////normalizing the uv
				int uvx = abs((int)(uv.x*(png_w-1))%255);
				int uvy = abs((int)(uv.y*(png_h-1))%255);		

				ushine::renderer::color_f src_col;

				int src_argb = texture[uvx+uvy*demo_effect_01_tex_01.get_height()];

				src_col.from_argb(src_argb);
				
				if (r>0.1)
				{
					////image process the effect
					src_col.r*=(-.5*uv.y);
					src_col.r*=(-.5*uv.y);
					src_col.r*=(-.5*uv.y);		
				} else
				{
					////image process the effect
					src_col.r*=(-.5*uv.y);
					src_col.r*=(-.5*uv.y);
					src_col.r*=(-.5*uv.y);					
				}						

				src_col.apply_blend_alpha(global_alpha);

				int dst_argb = src_col.to_argb(); 

				buf[i+j*(int)w]=dst_argb; 
			}

		//printf("frame end\n");

		demo_effect_01_tex_01.unlock();
		
		back_buffer.unlock();


		
		back_buffer.blit_to_screen(this);	
		return 0;	
	}	

	virtual int on_render(float timestamp)
	{	
		if (musicOk && playingOk==false && logo_shift<=320)
		{
			player.play(stream);
			playingOk=true; 
		}		


		demo_effect_01(timestamp);

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