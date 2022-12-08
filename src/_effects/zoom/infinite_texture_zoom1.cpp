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

	virtual int on_init()
	{

		CreateWnd(SCREEN_W, SCREEN_H, true,true);
		basic2d.init(SCREEN_W, SCREEN_H);

		png_loader.init(this);
		bool textLoaded = tex.create(this, SCREEN_W,SCREEN_H);

		std::string imagePath = getResourcePath() + "tex4.png";
		textLoaded = textLoaded & png_loader.load(png, imagePath);

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


    virtual ushine::math::vec4
        fragShader(
            ushine::math::vec4 input_color, 
            ushine::math::vec2 input_coords, 
            ushine::math::vec2 resolution,
            float global_time,
			ushine::core::texture& texture, int* texture_buf
			)
        {
			using namespace ushine::math;
            vec4 fragColor = input_color;

			vec2 r = resolution;
			vec2 p = (input_coords - r*.5f)/r.y;

			float c = 0.,
				  t = global_time, 
				  cs = cos(t), 
				  si = cos(sin(t)), 
				  s;

			auto m = mat2(cs, -si, si, cs);
			p = m*p;//should be p*m

    		auto o = vec4(0);

			for (float i = 0.0f; i<8.0f; i++){
				
					// Fractional time component.
					s = fract((i - t*2.)/18.);

					vec2 text_coords = p*exp2(s*8.0f)/1e2f + i*.9f;
					
					text_coords.x*=texture.get_width();
					text_coords.y*=texture.get_height();

					int u = int(text_coords.x)%texture.get_width();
					int v = int(text_coords.y)%texture.get_width();

					if (u>=0 && u<texture.get_width() &&
						v>=0 && v<texture.get_height())
						{
							ushine::renderer::color_f col;
							col.from_argb(texture_buf[u+v*texture.get_width()]);

							col.r=col.r * (0.5f - abs(s-0.5f))*1.5f;
							col.g=col.g * (0.5f - abs(s-0.5f))*1.5f;
							col.b=col.b * (0.5f - abs(s-0.5f))*1.5f;
							col.a=col.a * (0.5f - abs(s-0.5f))*1.5f;

							fragColor.x+=col.r;
							fragColor.y+=col.g;
							fragColor.z+=col.b;
							fragColor.w+=col.a;
						}      

				}

				

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

		int* png_buf = (int*)png.lock();

		for (int i=0; i<w; i++)
			for (int j=0; j<h; j++)
			{

				ushine::math::vec2 coords(i,j);
                ushine::math::vec2 res(w,h);
                ushine::math::vec4 input_col;
                ushine::math::vec4 output_col;

                output_col = fragShader(input_col,coords,res,additive_progress,png, png_buf);

                ushine::renderer::color_f render_col;
                
                render_col.r = output_col.x;
                render_col.g = output_col.y;
                render_col.b = output_col.z;
                render_col.a = output_col.w;

                buf[i+w*j]=render_col.to_argb();

			}

		png.unlock();
		tex.unlock();
		tex.blit_to_screen(this);

        additive_progress = (timestamp - first_tick)/1000;

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
