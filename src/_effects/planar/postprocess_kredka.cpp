#include <iostream>
#include <random>
#include <math.h>

#include "core/core.h"
#include "math/math.h"
#include "renderer/renderer.h"

#define RESOURCE_PATH "./textures/"
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
		float first_frame = -1;

	virtual int radial_zoom_effect(float timestamp)
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
		move1.x = cos(additive_progress*1)*0.4;
		move1.y = sin(additive_progress*10.5)*1.4;	

		ushine::math::base_matrix_2<float> 
					rotation, scale, trans;

		rotation[0][0] = cos(additive_progress);
		rotation[0][1] = -sin(additive_progress);
		rotation[1][0] = sin(additive_progress);
		rotation[1][1] = cos(additive_progress);

		scale[0][0] = 1.5+sin(additive_progress);
		scale[0][1] = 0;
		scale[1][0] = 0;
		scale[1][1] = 1.5+sin(additive_progress);

		trans = rotation*scale;


		ushine::math::base_vector_2<float> zero;
		zero.zero();

		for (int i=0; i<w; i++)
			for (int j=0; j<h; j++)
			{
				ushine::math::base_vector_3<float> p;
				ushine::math::base_vector_2<float> p2, uv;
				ushine::renderer::color_f o, total;


				p2.x = i/w;
				p2.y = j/h;

				p2 = p2 * trans;

				p.x = p2.x;
				p.y = p2.y;

				ushine::math::base_vector_2<float> q;
				q.x = sin(1.1*additive_progress+p.x);
				q.y = sin(1.2*additive_progress+p.y);

				float inner_radius = sqrt(q.dot(q));

				uv.x = p.x*sqrt(inner_radius)+cos(inner_radius);
				uv.y = p.y*sqrt(inner_radius)+sin(inner_radius);


				//uv.x = 0.5+(p.x*0.992);
				//uv.y = 0.5+(p.y*0.992);
				int uvx = int(fabs(uv.x*256))%255;
				int uvy = int(fabs(uv.y*256))%255;

				int argb = texture[uvx+uvy*256];

				o.from_argb(argb);
				p.z = (o.r+o.g+o.b)/3;

				
				for (int iter=0; iter<4; iter++)
				{
					float l = sqrt(o.r*o.r + o.g*o.g);
					p.z += powf(fmaxf(0.0,0.5-l),2.0)*expf(-iter*0.08);
				}
				
				//total = o;
				
				total.r=inner_radius*o.r*o.r + p.z;
				total.g=inner_radius*o.g*o.g + p.z;
				total.b=inner_radius*o.g*o.g + p.z;							
				total.a=1.0;
				

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