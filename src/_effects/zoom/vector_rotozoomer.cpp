#include <iostream>
#include <random>
#include <math.h>

#include "core/core.h"
#include "math/math.h"
#include "renderer/renderer.h"

#define SCREEN_W 320 //320
#define SCREEN_H 240 //240

//#include "hello_effect_basetest.h"

class HelloApp :
public ushine::core::application
{
public:
//	SDL_Window *win;
	ushine::core::texture tex;
	ushine::core::texture png;

	ushine::core::texture_loader_png png_loader;

	ushine::renderer::basic2d_context basic2d;

	//int vertex_type = VERTEX_VECTOR|VERTEX_ARGB;
	ushine::renderer::basic3d_vertex triangle[6];


	void init_geometry()
	{

	}


	virtual int on_init()
	{

		CreateWnd(SCREEN_W, SCREEN_H, false,true);
		basic2d.init(SCREEN_W, SCREEN_H);
		png_loader.init(this);


		bool textLoaded = tex.create(this, SCREEN_W,SCREEN_H);
		std::string imagePath = getResourcePath() + "Queen'sEntry.png";
		textLoaded = textLoaded & png_loader.load(png, imagePath);

		if (!textLoaded)
		{
			get_logger()->LogLine(std::string("Texture load failed!"));
			CloseWnd();
			return -1;
		}

		init_geometry();

		return 0;
	}

	virtual int on_close()
	{
		tex.release();
		CloseWnd();
		return 0;
	}


	float global_scale=0.4;
	int direction = 0;

	float r = 0;

	int TrianglesTest(int timestamp)
	{
		int* buf = (int*)tex.lock();
		if (buf == nullptr)
			return -1;

		basic2d.clear(buf, 255, 255, 255, 255,tex.get_width(),tex.get_height());

		ushine::math::base_matrix_2<float> scale, rotate, rotate_neg;

		float scale_x = 10.0/1/r;
		float scale_y = 10.0/1/r;

		rotate = ushine::math::base_matrix_2<float>::rotate(r);
		rotate_neg = ushine::math::base_matrix_2<float>::rotate(-r);

		scale = ushine::math::base_matrix_2<float>::scale(5.5,5.5);
		r+=3.1415/120;

		triangle[0].vector.set(0,0);
		triangle[0].argb.from_argb(0x1F00FF00);
		triangle[0].uv.set(0.0,0.0);

		triangle[1].vector.set(100,100);
		triangle[1].argb.from_argb(0x1F00A0FF);
		triangle[1].uv.set(1.0*scale_x,1.0*scale_y);

		triangle[2].vector.set(0,100);
		triangle[2].argb.from_argb(0x1FFF0000);
		triangle[2].uv.set(0,1.0*scale_y);

		triangle[3].vector.set(100,0);
		triangle[3].argb.from_argb(0x1F00FFFF);
		triangle[3].uv.set(1.0*scale_x,0.0);

		triangle[4].vector.set(100,100);
		triangle[4].argb.from_argb(0x1F00A0FF);
		triangle[4].uv.set(1.0*scale_x,1.0*scale_y);

		triangle[5].vector.set(0,0);
		triangle[5].argb.from_argb(0x1F00FF00);
		triangle[5].uv.set(.0,.0);

		png.lock();

		int mid_x = 50, mid_y = 50;

		for (int i=0; i<2; i++)
		{
			ushine::renderer::basic3d_vertex transformed_v[3];

			transformed_v[0] = triangle[i*3+0];
			transformed_v[1] = triangle[i*3+1];
			transformed_v[2] = triangle[i*3+2];


			for (int j=0; j<3; j++)
			{
				transformed_v[j].vector.x-=mid_x;
				transformed_v[j].vector.y-=mid_y;
			}

			auto t_v1 = scale*rotate*transformed_v[0].vector.xy();
			auto t_v2 = scale*rotate*transformed_v[1].vector.xy();
			auto t_v3 = scale*rotate*transformed_v[2].vector.xy();

			transformed_v[0].vector.set(t_v1.x, t_v1.y);
			transformed_v[1].vector.set(t_v2.x, t_v2.y);
			transformed_v[2].vector.set(t_v3.x, t_v3.y);

			for (int j=0; j<3; j++)
			{
				transformed_v[j].vector.x+=mid_x+50;
				transformed_v[j].vector.y+=mid_y+50;
			}

			//basic2d.draw_triangle_shade(buf,transformed_v[0],transformed_v[1],transformed_v[2]);
			basic2d.draw_triangle_textured(buf,transformed_v[0],transformed_v[1],transformed_v[2],png);

		}

 		png.unlock();
		tex.unlock();
		tex.blit_to_screen(this);

		return 0;
	}


	virtual int on_render(float timestamp)
	{

		TrianglesTest(timestamp);

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
