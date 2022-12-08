#include <iostream>

#include <random>
#include <math.h>

#include "core/core.h"
#include "math/math.h"
#include "renderer/renderer.h"

#define SCREEN_W 320
#define SCREEN_H 240

#include "bass.h"

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

		return 0;
	}

	virtual int on_close()
	{
		tex.release();
		CloseWnd();

		return 0;
	}


	float additive_progress=0;
	float first_frame=-1;

	virtual int blob_test(float timestamp)
	{
		if (first_frame==-1)
			first_frame=timestamp;

		additive_progress = (timestamp-first_frame)/800;

		int w = tex.get_width();
		int h = tex.get_height();

		int* buf = (int*)tex.lock();
		basic2d.clear(buf, 255, 100, 100, 20,w,h);

		if (buf == nullptr)
			return -1;

		ushine::math::base_vector_2<float>
					move1, move2;

		move1.x = cos(additive_progress*1)*0.4;
		move1.y = sin(additive_progress*1.5)*0.4;
		move2.x = sin(additive_progress*2.0)*0.4;
		move2.y = cos(additive_progress*3.0)*0.4;

		for (int i=0; i<w; i++)
			for (int j=0; j<h; j++)
			{

				ushine::math::base_vector_2<float> p;
				p.x = -1+2.0*i/w;
				p.y = -1+2.0*j/h;

				float r1 = (p-move1).dot(p-move1)*8;
				float r2 = (p+move2).dot(p+move2)*12;

				double metaball = (20/r1+50/r2);
				double col = metaball; //pow(metaball,2);

				if (col>255) col = 255;
				if (col>1)
					buf[i+j*w]=TO_ARGB(0xFF,(int)col,(int)col,(int)col);
				else
					buf[i+j*w]=TO_ARGB(0xFF,0,0,0);
			}

		tex.unlock();
		//sleep(1);
		tex.blit_to_screen(this);

		return 0;
	}


	virtual int on_render(float timestamp)
	{
		blob_test(timestamp);
		return 0;
	}

	virtual int on_event()
	{
		return 0;
	}

private:

	std::string getResourcePath()
	{
		return std::string("./resources/hello.app/data/");
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
