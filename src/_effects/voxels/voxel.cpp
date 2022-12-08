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


const float PI = 3.14159265359;
const float DEG_TO_RAD = PI / 180.0;

um::vec3 ray_dir_spherical(float fov, um::vec2 size, um::vec2 pos) {
	um::vec2 angle = (pos - um::vec2(0.5, 0.5) * size) * (fov / size.y * DEG_TO_RAD);

	um::vec2 c = cos(angle);
	um::vec2 s = sin(angle);
	
	return um::normalize(um::vec3(c.y * s.x, s.y, -c.y * c.x ));
}

um::mat3 rot3xy(um::vec2 angle) {
	um::vec2 c = cos(angle);
	um::vec2 s = sin(angle);
	
	return um::mat3(
		c.y      ,  0.0, -s.y,
		s.y * s.x,  c.x,  c.y * s.x,
		s.y * c.x, -s.x,  c.y * c.x
	);
}


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

        auto dir = ray_dir_spherical(60.0, iResolution, fragCoord);
        auto eye = um::vec3(0.0, 0.0, 56.0);

        auto rot = rot3xy(um::vec2(-DEG_TO_RAD*34.0, iTime*0.1));
        dir = rot * dir;
        eye = rot * eye;

        dir = normalize(dir);

        um::vec3 mask;
        um::vec3 center;

        auto color = um::vec3(1.0f, 1.0f, 0.0f);
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