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

	float mod(float x, float y)
	{
		return x - y * floor(x/y);
	}

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


			vec2 uv = input_coords/resolution+0.8f;


#define ITERATIONS 6.0f


			for (float i = 0.0f; i<ITERATIONS; i++){
				
					// Fractional time component.
					auto s = mod(i - global_time, ITERATIONS);

					vec2 text_coords = uv * exp2(s-2.0f);
					
					text_coords.x*=texture.get_width();
					text_coords.y*=texture.get_height();

					int u = abs(int(text_coords.x)%texture.get_width());
					int v = abs(int(text_coords.y)%texture.get_width());

					if (u>=0 && u<texture.get_width() &&
						v>=0 && v<texture.get_height())
						{
							ushine::renderer::color_f col;
							col.from_argb(texture_buf[u+v*texture.get_width()]);

							col.r=col.r * s *(ITERATIONS -s) *0.5;
							col.g=col.g * s *(ITERATIONS -s) *0.5;
							col.b=col.b * s *(ITERATIONS -s) *0.5;
							col.a=col.a * s *(ITERATIONS -s) *0.5;

							fragColor.x+=col.r/ITERATIONS;
							fragColor.y+=col.g/ITERATIONS;
							fragColor.z+=col.b/ITERATIONS;
							fragColor.w+=col.a/ITERATIONS;
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

        additive_progress = (timestamp - first_tick)/1800;

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
