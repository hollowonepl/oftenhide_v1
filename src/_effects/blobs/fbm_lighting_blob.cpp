#include <iostream>

#include <random>
#include <math.h>

#include "core/core.h"
#include "math/math.h"
#include "renderer/renderer.h"

#define SCREEN_W 320
#define SCREEN_H 240

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

class HelloApp :
public ushine::core::application
{
public:
	ushine::core::texture_loader_png png_loader;

	ushine::core::texture tex;
	ushine::core::texture png;

	ushine::renderer::basic2d_context basic2d;

	ushine::math::mat2 m;

	virtual int on_init()
	{

		CreateWnd(SCREEN_W, SCREEN_H, true,true);
		basic2d.init(SCREEN_W, SCREEN_H);

		png_loader.init(this);
		bool textLoaded = tex.create(this, SCREEN_W,SCREEN_H);

		std::string imagePath = getResourcePath() + "oft_intro_6.png";
		textLoaded = textLoaded & png_loader.load(png, imagePath);

		m = ushine::math::mat2( 0.8f,  0.6f, -0.6f,  0.8f );

		return 0;
	}

	virtual int on_close()
	{
		tex.release();
		CloseWnd();

		return 0;
	}

    float first_tick = -1;
	float additive_progress=0;


	float hash( float n )
	{
		using namespace ushine::math;
		return fract(sin(n)*43758.5453f);
	}

	float noise( ushine::math::vec2 x )
	{
		using namespace ushine::math;
		vec2 p = x.integral();
		vec2 f = x.fractional();

		f = f*f*(3.0f-f*2.0f);

		float n = p.x + p.y*57.0;

		float res = mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
						mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y);
		return res;
	}


	float fbm(ushine::math::vec2 _p)
	{
		ushine::math::vec2 p = _p;
		float f = 0.0f;
		ushine::math::mat2 mm;

		f +=0.50000f*abs(noise(p)-1.)*2.0f;
		mm = m*2.02f;
		p=mm*p;

		f +=0.25000f*abs(noise(p)-1.)*2.0f;
		mm = mm*2.03f;
		p=mm*p;

		f +=0.12500f*abs(noise(p)-1.)*2.0f;
		mm = mm*2.01f;
		p=mm*p;

		f +=0.06250f*abs(noise(p)-1.)*2.0f;
		mm = mm*2.04f;
		p=mm*p;

		f +=0.03125f*abs(noise(p)-1.)*2.0f;
		
		return f/.96875;

	}


    virtual ushine::math::vec4
        fragShader(
            ushine::math::vec4 input_color, 
            ushine::math::vec2 input_coords, 
            ushine::math::vec2 resolution,
            float global_time)
        {
            ushine::math::vec4 fragColor = input_color;
           
			ushine::math::vec2 q = input_coords / resolution;
			ushine::math::vec2 p = 2.0f*q-1.0f;
            float r = p.length();

			p.x *= resolution.x/resolution.y;

    		float f = fbm(p+global_time/2);
			f *= r*2.0f-.5f;
			f = (1.0f-f);

            fragColor.x= 0.1/f;
            fragColor.y= 0.2/f;
            fragColor.z= 0.3/f;
            fragColor.w= 1.5/f;

			fragColor.x=sqrt(abs(fragColor.x))*0.5;
			fragColor.y=sqrt(abs(fragColor.y))*0.5;
			fragColor.z=sqrt(abs(fragColor.z))*0.5;
			fragColor.w=sqrt(abs(fragColor.w))*0.5;

            return fragColor;
        }


	virtual int on_render(float timestamp)
	{
        if (first_tick<=0)
            first_tick=timestamp;

		int w = tex.get_width();
		int h = tex.get_height();

		int* buf = (int*)tex.lock();
		basic2d.clear(buf, 0, 0, 0, 0,w,h);



		for (int i=0; i<w; i++)
			for (int j=0; j<h; j++)
			{

				ushine::math::vec2 coords(i,j);
                ushine::math::vec2 res(w,h);
                ushine::math::vec4 input_col;
                ushine::math::vec4 output_col;

                output_col = fragShader(input_col,coords,res,additive_progress);

                ushine::renderer::color_f render_col;
                
                render_col.r = output_col.x;
                render_col.g = output_col.y;
                render_col.b = output_col.z;
                render_col.a = output_col.w;

				ushine::renderer::color_f previous;

				previous.from_argb(buf[i+w*j]);

				render_col.r=(previous.r+render_col.r)*0.8;
				render_col.g=(previous.g+render_col.g)*0.8;
				render_col.b=(previous.b+render_col.b)*0.8;
				render_col.a=(previous.a+render_col.a)*0.8;

                buf[i+w*j]=render_col.to_argb();

			}

		png.blit_to_buf(buf,0,0,w,h,250);

		tex.unlock();
		tex.blit_to_screen(this);

        additive_progress = (timestamp - first_tick)/2000;

		return 0;
	}

	virtual int on_event()
	{
		return 0;
	}

private:

	std::string getResourcePath()
	{
		return std::string("./res/");
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
