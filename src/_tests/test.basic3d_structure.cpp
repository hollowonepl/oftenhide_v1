#include <iostream>
#include <random>
#include <math.h>

#include "core/core.h"
#include "math/math.h"
#include "renderer/renderer.h"

#define SCREEN_W 320 
#define SCREEN_H 240 

class object_3d {

public:
	ushine::renderer::basic3d_vertex points[8];
	ushine::renderer::basic3d_vertex transformed[8];

	ushine::core::texture texture;

public:

	object_3d()
	{

	}

	~object_3d()
	{

	}

	int init(ushine::core::texture_loader_png& png_loader)
	{

		std::string path = std::string("./2d/checkers.png");
		png_loader.load(texture, path);
		
		/*
		set triangle points
		*/
        points[0].vector.set(-1, 1, 0,1);
		points[0].argb = ushine::renderer::color_f(0.0,1.0,1.0,1.0);
		points[0].uv.set(0,0);

        points[1].vector.set(1, -1, 0,1);	
		points[1].argb = ushine::renderer::color_f(1.0,1.0,1.0,1.0);	
		points[1].uv.set(1,1);

        points[2].vector.set(-1, -1, 0,1);
		points[2].argb = ushine::renderer::color_f(1.0,1.0,1.0,1.0);
		points[2].uv.set(0,1);

        points[3].vector.set(-1, 1, 0,1);
		points[3].argb = ushine::renderer::color_f(1.0,1.0,1.0,1.0);
		points[3].uv.set(0,0);

        points[4].vector.set(1, 1, 0,1);	
		points[4].argb = ushine::renderer::color_f(1.0,1.0,1.0,1.0);	
		points[4].uv.set(1,0);

        points[5].vector.set(1, -1, 0,1);
		points[5].argb = ushine::renderer::color_f(1.0,1.0,1.0,1.0);	
		points[5].uv.set(1,1);

		return 0;
	}


	int transform_init()
	{
		for (auto i=0; i<6; i++)
		{
			transformed[i].copy_from(points[i]);
		}
		return 0;
	}

	int transform(ushine::math::base_matrix_4<float> transformation_matrix)
	{
		for (auto i=0; i<6; i++)
		{
			transformed[i].vector = transformation_matrix*transformed[i].vector;
		}
		return 0;
	}

	int project()
	{
		for (auto i=0; i<6; i++)
		{
			transformed[i].transform_to_2d(SCREEN_W,SCREEN_H,256,10.0);
			transformed[i].vector.x = transformed[i].coords2d.x;		
			transformed[i].vector.y = transformed[i].coords2d.y;
			transformed[i].vector.z = transformed[i].coords2d.z;		
			transformed[i].vector.w = transformed[i].coords2d.w;
		}
		return 0;
	}


	int render(ushine::renderer::basic2d_context& basic2d, int*buf)
	{
		int* texbuf = (int*)texture.lock();

		basic2d.draw_triangle_textured(
		buf, transformed[0],transformed[1],transformed[2],
		texture);

		basic2d.draw_triangle_textured(
		buf, transformed[3],transformed[4],transformed[5],
		texture);

		texture.unlock();

		return 0;
	}

	int release()
	{
		texture.release();
		return 0;
	}

};

class HelloApp :
public ushine::core::application
{
public:

	ushine::core::texture back_buffer,background_texture;
	ushine::renderer::basic2d_context basic2d;
	ushine::core::texture_loader_png png_loader;

	object_3d mesh;

	virtual int on_init()
	{

		CreateWnd(SCREEN_W, SCREEN_H, false,true);
		basic2d.init(SCREEN_W, SCREEN_H);
		png_loader.init(this);

		back_buffer.create(this,SCREEN_W,SCREEN_H);


		std::string path = std::string("./textures/background_red.png");
		png_loader.load(background_texture, path);
		
		mesh.init(png_loader);

		return 0;
	}

	virtual int on_close()
	{
		mesh.release();

		back_buffer.release();
		CloseWnd();
		return 0;
	}


	virtual int on_render(float timestamp)
	{

		int* buf = (int*)back_buffer.lock();
		if (buf == nullptr)
			return -1;

		basic2d.clear(buf, 255, 25, 25, 25, SCREEN_W,SCREEN_H);

		auto mat_rotate3d_x = ushine::renderer::matrix_rotate_x(
			ushine::math::to_radians(sin(timestamp/4000)*360.0));
		auto mat_rotate3d_y = ushine::renderer::matrix_rotate_y(
			ushine::math::to_radians(180+sin(timestamp/400)*5.0));
		auto mat_rotate3d_z = ushine::renderer::matrix_rotate_z(
			ushine::math::to_radians(180.0));
		auto mat_scale = ushine::renderer::matrix_scale(
			5.0+sin(timestamp/400),5.0+cos(timestamp/400), 1.0);

		auto mat_move = ushine::renderer::matrix_translate(
							0,
							0,
							10.5);

		auto transformation_matrix = mat_scale* mat_rotate3d_x*mat_rotate3d_y*mat_rotate3d_z;

		mesh.transform_init();
		mesh.transform(transformation_matrix);
		mesh.transform(mat_move);
		mesh.project();

		background_texture.blit_to_buf(buf,0,0,background_texture.get_width(),background_texture.get_height());

		mesh.render(basic2d,buf);

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
