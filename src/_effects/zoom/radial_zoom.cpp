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
		std::string imagePath = getResourcePath() + "az0!_envmap12_02.png"; //"az0!_envmap12_01.png";
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

	ushine::renderer::color_f deform(ushine::math::base_vector_2<float>& p, int* texture)
	{

				ushine::math::base_vector_2<float> uv;

				ushine::math::base_vector_2<float> q;
				q.x = sin(1.1*additive_progress+p.x);
				q.y = sin(1.2*additive_progress+p.y);

				float inner_radius = sqrt(q.dot(q));

				//uv.x = p.x+cos(sin(0.2*additive_progress/4)); //*sqrt(inner_radius); //sin(0.5+0.5*additive_progress)+p.x*sqrt(inner_radius);
				//uv.y = p.y-sin(cos(0.6*additive_progress)); //*sqrt(inner_radius)); //sin(0.6+0.22*additive_progress)+p.y*sqrt(inner_radius);

				uv.x = p.x*sqrt(inner_radius);//+cos(inner_radius);
				uv.y = p.y*sqrt(inner_radius);//+sin(inner_radius);

				//uv.x*=0.5;
				//uv.y*=0.5;


				int uvx = int(fabs(uv.x*256))%255;
				int uvy = int(fabs(uv.y*256))%255;

				float r = 1.5/(1.0+p.dot(p));

				int argb = texture[uvx+uvy*256];

				ushine::renderer::color_f src_col;
				src_col.from_argb(argb);

				return src_col;
	}


	virtual int radial_zoom_effect(float timestamp)
	{
		additive_progress = timestamp/(3.1415*8);

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
		move1.x = cos(additive_progress*1)*0.4;
		move1.y = sin(additive_progress*1.5)*1.4;

		ushine::math::base_vector_2<float> zero;
		zero.zero();

		for (int i=0; i<w; i++)
			for (int j=0; j<h; j++)
			{
				ushine::math::base_vector_2<float> p, s, distance, uv;
				ushine::renderer::color_f total;


				p.x = -1+2.0*i/w;
				p.y = -1+2.0*j/h;

				s=p;

				//s.x = i*2; // p.x;
				//s.y = j*2; //p.y;

				distance = (zero-p)/10.0f;
				float wobble = 2.0;

				for (int iterations=0; iterations<4; iterations++)
				{

					ushine::renderer::color_f deformed_col = deform(s,texture);
					deformed_col.r = ushine::math::smoothstep<float>(0.1, 1.5, deformed_col.r)*wobble;
					deformed_col.g = ushine::math::smoothstep<float>(0.1, 1.5, deformed_col.g)*wobble;
					deformed_col.b = ushine::math::smoothstep<float>(0.1, 1.5, deformed_col.b)*wobble;

					total.r+=deformed_col.r*wobble;
					total.g+=deformed_col.g*wobble;
					total.b+=deformed_col.b*wobble;

					wobble*=0.99;
					s+=distance;
					s.x=s.x+sin(distance.x);
					s.y=s.y+sin(distance.y);
				}

				total.r=total.r/4.0;
				total.g=total.g/4.0;
				total.b=total.b/4.0;

				float r = 1.5/(1.0+p.dot(p));

				ushine::renderer::color_f src_col;
				src_col = total;

				src_col.r*=r/4;
				src_col.g*=r/4;
				src_col.b*=r/4; //total.z*r;
				src_col.a=1.0;

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
		radial_zoom_effect(timestamp);
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
