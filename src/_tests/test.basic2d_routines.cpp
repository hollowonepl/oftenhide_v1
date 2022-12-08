#include <iostream>
#include <random>
#include <math.h>

#include "core/core.h"
#include "math/math.h"
#include "renderer/renderer.h"

#define SCREEN_W 320 //320
#define SCREEN_H 240 //240
#define FULLSCREEN_ON true
#define VSYNC_ON true


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

		CreateWnd(SCREEN_W, SCREEN_H, VSYNC_ON,FULLSCREEN_ON);
		basic2d.init(SCREEN_W, SCREEN_H);
		png_loader.init(this);


		bool textLoaded = tex.create(this, SCREEN_W,SCREEN_H);
		std::string imagePath = getResourcePath() + "tex2.png";
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


	int BaseRenderTest(float timestamp)
	{
		int* buf = (int*)tex.lock();

		if (animation==0)
			animation = timestamp;

		int progress = (timestamp-animation);
		animation = timestamp;

		if (buf !=nullptr)
		{

			basic2d.clear(buf, 255, colElement, 0, 0,SCREEN_W,SCREEN_H);

			if (colElementDir==1)
			{
				colElement+=progress;
				if (colElement>255)
				{
					colElementDir=0;
					colElement=255;
				}
			} else
			{
				colElement-=progress;
				if (colElement<0)
				{
					colElementDir=1;
					colElement=0;
				}
			}

			basic2d.draw_rect(	buf, 255, 0, 255, 0,
								colElement, 10, 100, 100 );

		    std::random_device rd;
		    std::mt19937 mt(rd());
		    std::uniform_int_distribution<double> dist(0, 255);

			std::uniform_int_distribution<double> dist_X(0, 319);
			std::uniform_int_distribution<double> dist_Y(0, 239);

			for (auto i=20; i<300; i++)
				for (auto j=20; j<200; j++)
				{
					int monochromatic_noise = (int)dist(mt);
					auto col = TO_ARGB(255,
						monochromatic_noise,
						monochromatic_noise,
						monochromatic_noise);

					basic2d.put_pixel(buf, col, i, j);
				}

			for (auto w=0; w<200; w++)
			{
				auto col = TO_ARGB(255,
					(int)dist(mt),
					(int)dist(mt),
					(int)dist(mt));

				basic2d.draw_line(buf, col,
					dist_X(mt), dist_Y(mt),dist_X(mt),dist_Y(mt));
			}

			png.blit_to_buf(buf,
				0,0,
				tex.get_width(),
				tex.get_height(),
				255-colElement);
		}

		tex.unlock();

//		png.blit_to_screen(this);
		tex.blit_to_screen(this);
		return 0;
	}

	float clamp(float x, float min, float max)
	{
	    if (x < min)
	        x = min;
	    else if (x > max)
	        x = max;

	    return x;
	}

	float smoothstep(float edge0, float edge1, float x)
	{
	    // Scale, bias and saturate x to 0..1 range
	    x = clamp((x - edge0)/(edge1 - edge0), 0.0, 1.0);
	    // Evaluate polynomial
	    return x*x*(3 - 2*x);
	}

	int sample_argb(int* tex, int w, int h, float u, float v)
	{
		float pu, pv;
		pu = u; pv = v;
		if (pu>1.0) pu = pu - (int)pu;
		if (pv>1.0) pv = pv - (int)pv;

		int i=u*(w-1);
		int j=v*(h-1);

		return tex[i+j*w];
	}

	virtual int tv_process(int *src, int *dst, int w, int h, int time)
	{

		for (int i=0; i<w; i++)
			for (int j=0; i<h; j++)
			{
				ushine::math::base_vector_2<float> q, uv;
				ushine::renderer::color_f oricol, colr,colg,colb;

				q.x = i/w;
				q.y = i/h;

				uv.x = 0.5 + (q.x-0.5)*(0.9 + 0.1*sin(0.2*time));
				uv.y = 0.5 + (q.y-0.5)*(0.9 + 0.1*sin(0.2*time));
				oricol.from_argb(
					sample_argb(src, w,h,q.x, q.y));

				//colr.from_argb(sample_argb(src,w,h,))



			}
/*
    col.r = texture2D(tex0,vec2(uv.x+0.003,-uv.y)).x;
    col.g = texture2D(tex0,vec2(uv.x+0.000,-uv.y)).y;
    col.b = texture2D(tex0,vec2(uv.x-0.003,-uv.y)).z;

    col = clamp(col*0.5+0.5*col*col*1.2,0.0,1.0);

    col *= 0.5 + 0.5*16.0*uv.x*uv.y*(1.0-uv.x)*(1.0-uv.y);

    col *= vec3(0.8,1.0,0.7);

    col *= 0.9+0.1*sin(10.0*time+uv.y*1000.0);

    col *= 0.97+0.03*sin(110.0*time);

    float comp = smoothstep( 0.2, 0.7, sin(time) );
    col = mix( col, oricol, clamp(-2.0+2.0*q.x+3.0*comp,0.0,1.0) );

    gl_FragColor = vec4(col,1.0);
*/

    	return 0;
	}





	virtual int on_render(float timestamp)
	{
		BaseRenderTest(timestamp);

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
