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

	ushine::renderer::basic2d_context basic2d;
	ushine::core::texture_loader_png png_loader;

	ushine::renderer::basic3d_vertex points[8];

	virtual int on_init()
	{

		CreateWnd(SCREEN_W, SCREEN_H, false,true);
		basic2d.init(SCREEN_W, SCREEN_H);
		png_loader.init(this);

		back_buffer.create(this,SCREEN_W,SCREEN_H);


		/*
		set cube points
		*/
        points[0].vector.set(-1, 1, -1);
        points[1].vector.set(1, 1, -1);
        points[2].vector.set(1, -1, -1);
        points[3].vector.set(-1, -1, -1);
        points[4].vector.set(-1, 1, 1);
        points[5].vector.set(1, 1, 1);
        points[6].vector.set(1, -1, 1);
        points[7].vector.set(-1, -1, 1);

		return 0;
	}

	virtual int on_close()
	{
		back_buffer.release();
		CloseWnd();
		return 0;
	}


	ushine::math::base_vector_4<float> project(
		ushine::math::base_vector_4<float> origin, 
		float view_w, float view_h, float fov, float view_dist)
	{
		ushine::math::base_vector_4<float> out;

		auto factor = fov/(view_dist+origin.z);
		out.x = origin.x * factor + view_w/2.0f;
		out.y = origin.y * factor + view_h/2.0f;
		out.z = origin.z;

		return out;
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

		auto transformation_matrix = mat_rotate3d_x*mat_rotate3d_y*mat_rotate3d_z;

		ushine::renderer::basic3d_vertex transformed[8];

		for (auto i=0; i<8; i++)
		{
			transformed[i] = points[i];
			transformed[i].vector = transformation_matrix*points[i].vector;
				
			
			transformed[i].coords2d = 
				project(transformed[i].vector,
						SCREEN_W,SCREEN_H,
						145,10.0);
			

			buf[(int)transformed[i].coords2d.x+
				(int)transformed[i].coords2d.y*SCREEN_W]
			=TO_ARGB(255, 
				0,0,0);

		}


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
