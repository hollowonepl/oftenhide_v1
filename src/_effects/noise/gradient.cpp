#include <iostream>
#include <random>
#include <math.h>

#include "core/core.h"
#include "math/math.h"
#include "renderer/renderer.h"

#define SCREEN_W 320
#define SCREEN_H 240

#define RESOURCE_PATH "./textures/"

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

    auto iResolution = um::vec2(w, h);
    for (int y=0; y<h; y++) {
      for (int x=0; x<w; x++) {
        auto uv = um::vec2(x, y) / iResolution;
        auto color = 0.1f + 0.3f * um::cos(iTime + um::vec3(uv.x, uv.y, uv.x) + um::vec3(1.0f, 2.0f, 3.0f));
        *buf++ = ur::color_f(1.0f, color.x, color.y, color.z).to_argb();
      }
    }

    //vec2 uv = fragCoord/iResolution.xy;
    //vec3 col = 0.1 + 0.3*cos(iTime+uv.xyx+vec3(1,2,3));
    //fragColor = vec4(col,1.0);

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