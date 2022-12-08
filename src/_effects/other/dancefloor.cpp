#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include <memory>
#include <iterator>

#include "core/core.h"
#include "math/math.h"
#include "renderer/renderer.h"
#include "renderer/basic2d_routines.h"

#define SCREEN_W 320
#define SCREEN_H 240

#define RESOURCE_PATH "./resources/back2roots.app/data/"

namespace um = ushine::math;
namespace uc = ushine::core;
namespace ur = ushine::renderer;



class HelloApp : public uc::application {
public:
	uc::texture backbuffer;
	ur::basic2d_context basic2d;


	virtual int on_init() {
		CreateWnd(SCREEN_W, SCREEN_H, true, true);
		basic2d.init(SCREEN_W, SCREEN_H);	

		auto loaded = backbuffer.create(this, SCREEN_W, SCREEN_H);

		if (!loaded) {
			get_logger()->LogLine(std::string("Texture load failed!"));
			CloseWnd();
			return -1;
		}

		return 0;
	}

	virtual int on_close() {
		backbuffer.release();				
		CloseWnd();
		return 0;
	}

	virtual int effect_test(float timestamp) {
    float iTime = timestamp / 750.0f;
		float w = backbuffer.get_width();
		float h = backbuffer.get_height();

		int* buf = (int*)backbuffer.lock();

		basic2d.clear(buf, 0, 0, 0, 0,w,h);

    auto iResolution = um::vec2(w, h);
    for (int y=0; y<h; y++) {
      for (int x=0; x<w; x++) {
        auto fragCoord = um::vec2(x, y);
        auto px = 4.0f * (-iResolution + 2.0f * fragCoord) / iResolution.y;
        auto id = 0.5f + 0.5f*std::cos(iTime + std::sin(um::dot(um::floor(px+0.5f), um::vec2(113.1f,17.81f)))*43758.545f);
        auto co = 0.5f + 0.5f*um::cos(iTime + 3.5f*id + um::vec3(0.0f,1.57f,3.14f) );
        auto pa = um::smoothstep( 0.0f, 0.5f, (0.5f*um::cos(6.2831f * px)+0.5f)*id );
        auto color = co * pa.x * pa.y;
        *buf++ = ur::color_f(1.0f, color.x, color.y, color.z).to_argb();
      }
    }

		backbuffer.unlock();
		backbuffer.blit_to_screen(this);	
		return 0;	
	}	


	virtual int on_render(float timestamp) {	
		effect_test(timestamp);
		return 0;
	}

	virtual int on_event() { return 0; }

private:
	std::string getResourcePath() { return std::string(RESOURCE_PATH); }
};



int main(int argc, char** argv) {
	HelloApp app;

	if (app.Init() != 0)
		return 0;

	app.Run();
	app.Close();

	return 0;
}