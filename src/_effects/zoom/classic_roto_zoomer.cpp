#include <iostream>
#include <random>
#include <math.h>

#include "core/core.h"
#include "math/math.h"
#include "renderer/renderer.h"

#define SCREEN_W 320 
#define SCREEN_H 240 


using namespace ushine::math;
using namespace ushine::renderer;

class HelloApp :
public ushine::core::application
{
private:
	std::string getResourcePath()
	{
		return std::string("./textures/");
	}
public:

	ushine::core::texture back_buffer;
	ushine::core::texture texture;

	ushine::renderer::basic2d_context basic2d;
	ushine::core::texture_loader_png png_loader;

	virtual int on_init()
	{

		CreateWnd(SCREEN_W, SCREEN_H, false,true);
		basic2d.init(SCREEN_W, SCREEN_H);
		png_loader.init(this);

		std::string path = std::string("./textures/tex3.png");
		auto result = png_loader.load(texture, path);

		back_buffer.create(this,SCREEN_W,SCREEN_H);

		return 0;
	}

	virtual int on_close()
	{
		back_buffer.release();
		CloseWnd();
		return 0;
	}

	vec4 sample_texture(ushine::core::texture& tex, int* buf, vec2 uv)
	{
		int uvx = int(uv.x*tex.get_width())%tex.get_width();
		int uvy = int(uv.y*tex.get_height())%tex.get_height();

		color_f outc;

		int argb = buf[uvx+uvy*tex.get_height()];
		outc.from_argb(argb);

		return vec4(outc.r,outc.g,outc.b,outc.a);
	}

	float first_frame_time = -1;
	float additive_time = 0;
	int* texture_buf=nullptr;
	

	base_matrix_2<float> rot (float a)
	{
		return base_matrix_2<float>::rotate(a); 
	}

	ushine::math::vec4 pixel_render(float timestamp, vec2 fragCoord, vec2 iResolution)
	{
		vec4 fragColor(0.,0.,0.,1.0);

  		vec2 uv = vec2(fragCoord.x / iResolution.x, fragCoord.y / iResolution.y);
		//uv+=cos(additive_time/1000);

		base_matrix_2<float> 
			tmat=base_matrix_2<float>::scale(
				5.0f+sin(additive_time/1000)*2,
				5.0f+sin(additive_time/1000)*2);

			tmat=tmat*rot(additive_time/1000);

		uv = tmat*uv;

		fragColor = sample_texture(texture,texture_buf,abs(uv));
		fragColor.w=1.0f;

		return fragColor;
	}
	
	virtual int on_render(float timestamp)
	{
		using namespace ushine::math;

		int* buf = (int*)back_buffer.lock();
		if (buf == nullptr)
			return -1;

		texture_buf = (int*) texture.lock();

		if (first_frame_time==-1)
			first_frame_time=timestamp;
		
		additive_time = (timestamp-first_frame_time);

		//basic2d.clear(buf, 255, 255, 255, 255, SCREEN_W,SCREEN_H);

		vec2 res(back_buffer.get_width(),back_buffer.get_height());

		for (auto i=0; i<(int)res.x; i++)
			for (auto j=0; j<(int)res.y; j++)
			{
				auto color = pixel_render(timestamp, vec2(i,j), res);

				ushine::renderer::color_f col;
				col.r=color.x;
				col.g=color.y;
				col.b=color.z;
				col.a=color.w;

				buf[i+j*int(res.x)]=col.to_argb();
			}

		texture.unlock();
		back_buffer.unlock();
		back_buffer.blit_to_screen(this);

		return 0;
	}

	virtual int on_event()
	{
		return 0;
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
