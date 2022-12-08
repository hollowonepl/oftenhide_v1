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
		set triangle points
		*/
        points[0].vector.set(-1, 1, -1);
		points[0].argb = ushine::renderer::color_f(1.0,0.7,0.3,0.4);
        points[1].vector.set(1, -1, -1);	
		points[1].argb = ushine::renderer::color_f(1.0,0.7,0.3,0.4);	
        points[2].vector.set(-1, -1, -1);
		points[2].argb = ushine::renderer::color_f(1.0,0.7,0.3,0.4);

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
		//out.z = origin.z;

		return out;
	}

	virtual int on_render(float timestamp)
	{

		int* buf = (int*)back_buffer.lock();
		if (buf == nullptr)
			return -1;

		basic2d.clear(buf, 255, 255, 255, 255, SCREEN_W,SCREEN_H);

		auto mat_rotate3d_x = ushine::renderer::matrix_rotate_x(
			ushine::math::to_radians(timestamp/80));
		auto mat_rotate3d_y = ushine::renderer::matrix_rotate_y(
			ushine::math::to_radians(timestamp/80));
		auto mat_rotate3d_z = ushine::renderer::matrix_rotate_z(
			ushine::math::to_radians(timestamp/80));
		auto mat_scale = ushine::renderer::matrix_scale(
			2.0+sin(timestamp/400),2.0+cos(timestamp/400), 1.0+sin(cos(timestamp/400)));

		auto transformation_matrix = mat_scale* mat_rotate3d_x*mat_rotate3d_y*mat_rotate3d_z;

		ushine::renderer::basic3d_vertex transformed[8];

		for (auto i=0; i<3; i++)
		{
			transformed[i] = points[i];
			transformed[i].vector = transformation_matrix*points[i].vector;
			
			transformed[i].transform_to_2d(SCREEN_W,SCREEN_H,256,10.0);
			/*transformed[i].coords2d = 
				project(transformed[i].vector,
						SCREEN_W,SCREEN_H,
						256,10.0);
			*/
			
		}

		/*
		basic2d.draw_triangle_lines(
				buf, TO_ARGB(255,0,0,0),
				transformed[0].coords2d,
				transformed[1].coords2d,
				transformed[2].coords2d
			);
		*/

		
		basic2d.draw_triangle_fill(
				buf, TO_ARGB(255,0,0,0),
				transformed[0].coords2d,
				transformed[1].coords2d,
				transformed[2].coords2d
			);
		
		/*
		basic2d.draw_triangle_shade(
				buf, transformed[0],transformed[1],transformed[2]);
		*/

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
