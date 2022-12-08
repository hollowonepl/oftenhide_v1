#include <iostream>
#include <random>
#include <math.h>

#include "core/core.h"
#include "math/math.h"
#include "renderer/renderer.h"

#define SCREEN_W 320 //320
#define SCREEN_H 240 //240

#define RESOURCE_PATH "./resources/back2roots.app/data/"

class HelloApp : 
public ushine::core::application
{
public:
//	SDL_Window *win;
	ushine::core::texture tex;

	ushine::renderer::basic2d_context basic2d;

	virtual int on_init()
	{

		CreateWnd(SCREEN_W, SCREEN_H, false,true);
		basic2d.init(SCREEN_W, SCREEN_H);	

		bool textLoaded = tex.create(this, SCREEN_W,SCREEN_H);

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
	float additive_progress=0;
	float first_frame=-1;

	virtual int tunnel_test(float timestamp)
	{
		if (first_frame==-1)first_frame=timestamp;
		additive_progress = (timestamp-first_frame)/700;

		float w = tex.get_width();
		float h = tex.get_height();

		int* buf = (int*)tex.lock();

		//basic2d.clear(buf, 255, 0, 0, 0);

		if (buf == nullptr)
			return -1;

		ushine::math::base_vector_2<float> 
					move1;
		move1.x = cos(additive_progress*1)*0.4;
		move1.y = sin(additive_progress*1.5)*0.4;		


		for (int i=0; i<w; i++)
			for (int j=0; j<h; j++)
			{


				ushine::math::base_vector_2<float> p, uv;

				p.x = -1+2.0*i/w;
				p.y = -1+2.0*j/h;

			    float _a = additive_progress*40.0;
 
    			float _d,_e,_f,_g=1.0/40.0,_h,_i,_r,_q;
 				
 				_e=400.0*(p.x*0.5+0.5);
    			_f=400.0*(p.y*0.5+0.5);
    			_i=200.0+sin(_e*_g+_a/150.0)*20.0;
    			_d=200.0+cos(_f*_g/2.0)*18.0+cos(_e*_g)*7.0;

    			_r=sqrt(pow(_i-_e,2.0)+pow(_d-_f,2.0));
    			_q=_f/_r;

    			_e=(_r*cos(_q))-_a/2.0;
    			_f=(_r*sin(_q))-_a/2.0;

   				_d=sin(_e*_g)*176.0+sin(_e*_g)*164.0+_r;
    			_h=((_f+_d)+_a/2.0)*_g;
    
    			_i=cos(_h+_r*p.x/1.3)*(_e+_e+_a)+cos(_q*_g*6.0)*(_r+_h/3.0);
    
    			_h=sin(_f*_g)*144.0-sin(_e*_g)*212.0*p.x;
    			_h=(_h+(_f-_e)*_q+sin(_r-(_a+_h)/7.0)*10.0+_i/4.0)*_g;

    			_i+=cos(_h*2.3*sin(_a/350.0-_q))*184.0*sin(_q-(_r*4.3+_a/12.0)*_g)+tan(_r*_g+_h)*184.0*cos(_r*_g+_h);
    			_i=fmod(_i/5.6,256.0)/64.0;

    			if(_i<0.0) _i+=4.0;
    			if(_i>=2.0) _i=4.0-_i;
    
    			_d=_r/350.0;
    			_d+=sin(_d*_d*8.0)*0.52;

    			_f=(sin(_a*_g)+1.0)/2.0;
    
 				ushine::renderer::color_f src_col;			
				src_col.r=(_f*_i/1.6)		*_d*p.x + (_i/1.3+_d/8.0)	*_d*(1.0-p.x);
				src_col.g=(_i/2.0+_d/13.0)*_d*p.x + (_i/2.0+_d/18.0)	*_d*(1.0-p.x);
				src_col.b=_i 			*_d*p.x + _i *_d*(1.0-p.x);

				int dst_argb = src_col.to_argb();						

				buf[i+j*(int)w]=dst_argb; 
			}

		tex.unlock();
		tex.blit_to_screen(this);	
		return 0;	
	}	


	virtual int on_render(float timestamp)
	{	
		tunnel_test(timestamp);

		return 0;
	}

	virtual int on_event()
	{
		return 0;
	}

private:

	std::string getResourcePath()
	{
		return std::string(RESOURCE_PATH);
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