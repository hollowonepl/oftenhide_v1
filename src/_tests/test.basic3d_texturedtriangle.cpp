#include <iostream>
#include <random>
#include <math.h>

#include "core/core.h"
#include "math/math.h"
#include "renderer/renderer.h"

#define SCREEN_W 320 
#define SCREEN_H 240 


class HelloApp :
public ushine::core::application
{
public:

	ushine::core::texture back_buffer;
	ushine::core::texture texture;

	ushine::renderer::basic2d_context basic2d;
	ushine::core::texture_loader_png png_loader;

	ushine::renderer::basic3d_vertex points[8];

	virtual int on_init()
	{

		CreateWnd(SCREEN_W, SCREEN_H, false,true);
		basic2d.init(SCREEN_W, SCREEN_H);
		png_loader.init(this);

		back_buffer.create(this,SCREEN_W,SCREEN_H);

		std::string path = std::string("./textures/tex3.png");
		png_loader.load(texture, path);

		/*
		set triangle points
		*/
        points[0].vector.set(-1, 1, 0,1);
		points[0].argb = ushine::renderer::color_f(1.0,1.0,0.0,0.0);
		points[0].uv.set(0,0);

        points[1].vector.set(1, -1, 0,1);	
		points[1].argb = ushine::renderer::color_f(1.0,0.0,1.0,0.0);	
		points[1].uv.set(1,0);

        points[2].vector.set(-1, -1, 0,1);
		points[2].argb = ushine::renderer::color_f(1.0,0.0,0.0,1.0);
		points[2].uv.set(1,1);

		return 0;
	}

	virtual int on_close()
	{
		back_buffer.release();
		CloseWnd();
		return 0;
	}

	virtual int on_render(float timestamp)
	{

		int* buf = (int*)back_buffer.lock();
		if (buf == nullptr)
			return -1;

		basic2d.clear(buf, 255, 255, 255, 255, SCREEN_W,SCREEN_H);

		auto mat_rotate3d_x = ushine::renderer::matrix_rotate_x(
			ushine::math::to_radians(timestamp/40));
		auto mat_rotate3d_y = ushine::renderer::matrix_rotate_y(
			ushine::math::to_radians(timestamp/40));
		auto mat_rotate3d_z = ushine::renderer::matrix_rotate_z(
			ushine::math::to_radians(timestamp/40));
		auto mat_scale = ushine::renderer::matrix_scale(1,1,1);

		auto mat_move = ushine::renderer::matrix_translate(
							0,
							0,
							3.5+sin(timestamp/400)*2);

		auto transformation_matrix = mat_scale* mat_rotate3d_x*mat_rotate3d_y*mat_rotate3d_z;

		ushine::renderer::basic3d_vertex transformed[8];

		for (auto i=0; i<3; i++)
		{
			transformed[i] = points[i];
			transformed[i].vector = transformation_matrix*points[i].vector;
			transformed[i].vector = transformed[i].vector*mat_move;

			transformed[i].transform_to_2d(SCREEN_W,SCREEN_H,256,10.0);

			transformed[i].vector.x = transformed[i].coords2d.x;		
			transformed[i].vector.y = transformed[i].coords2d.y;
			transformed[i].vector.z = transformed[i].coords2d.z;	
			
		}

		int* texbuf = (int*)texture.lock();
		
		basic2d.draw_triangle_textured(
				buf, transformed[0],transformed[1],transformed[2],
				texture);

		texture.unlock();


		back_buffer.unlock();
		back_buffer.blit_to_screen(this);

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
