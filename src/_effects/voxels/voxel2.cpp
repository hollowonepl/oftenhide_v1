// https://www.shadertoy.com/view/4dX3zl
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

#define RESOURCE_PATH "./textures/"

namespace um = ushine::math;
namespace uc = ushine::core;
namespace ur = ushine::renderer;

const bool USE_BRANCHLESS_DDA = true;
const int MAX_RAY_STEPS = 64;

um::vec2 rotate2d(um::vec2 v, float a) {
	float sinA = sin(a);
	float cosA = cos(a);
	return um::vec2(v.x * cosA - v.y * sinA, v.y * cosA + v.x * sinA);	
}

float sdSphere(um::vec3 p, float d) { return um::length(p) - d; } 

float sdBox( um::vec3 p, um::vec3 b )
{
  um::vec3 d = um::abs(p) - b;
  return std::min(
    std::max(d.x, std::max(d.y ,d.z)),
    0.0f) + um::length(
      um::max(d, 0.0f)
    );
}
	

bool getVoxel(um::ivec3 c) {
	um::vec3 p = um::vector_cast<float>(c) + um::vec3(0.5f);
	float d = std::min(std::max(-sdSphere(p, 7.5f), sdBox(p, um::vec3(6.0f))), -sdSphere(p, 25.0f));
	return d < 0.0f;
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

  float first_tick = -1;

	virtual int effect_test(float timestamp) {

    if (first_tick<0) first_tick = timestamp;

    float iTime = (timestamp -first_tick)/ 1000.0f;

		float w = backbuffer.get_width();
		float h = backbuffer.get_height();

		int* buf = (int*)backbuffer.lock();

		basic2d.clear(buf, 0, 0, 0, 0,w,h);

    auto iResolution = um::vec2(w, h);
    for (int y=0; y<h; y++) {
      for (int x=0; x<w; x++) {
        auto fragCoord = um::vec2(x, y);

        um::vec2 screenPos = ((fragCoord / iResolution) * 2.0f) - 1.0f;
        auto cameraDir = um::vec3(0.0f, 0.0f, 0.8f);
        auto cameraPlaneU = um::vec3(1.0f, 0.0f, 0.0f);
        auto cameraPlaneV = um::vec3(0.0f, 1.0f, 0.0f) * iResolution.y / iResolution.x;
        auto rayDir = cameraDir + screenPos.x * cameraPlaneU + screenPos.y * cameraPlaneV;
        auto rayPos = um::vec3(0.0f, 2.0f * sin(iTime * 2.7f), -12.0f);

        rayPos.xz(rotate2d(rayPos.xz(), iTime));
        rayDir.xz(rotate2d(rayDir.xz(), iTime));

        auto mapPos = um::vector_cast<int>(um::floor(rayPos + 0.0f));
	      auto deltaDist = um::abs(um::vec3(um::length(rayDir)) / rayDir);
	      auto rayStep = um::vector_cast<int>(um::sign(rayDir));

	      auto sideDist = (um::sign(rayDir) * (um::vector_cast<float>(mapPos) - rayPos) + (um::sign(rayDir) * 0.5f) + 0.5f) * deltaDist; 
	
	      um::bvec3 mask;
        for (int i = 0; i < MAX_RAY_STEPS; i++) {
          if (getVoxel(mapPos)) continue;
          if (sideDist.x < sideDist.y) {
            if (sideDist.x < sideDist.z) {
              sideDist.x += deltaDist.x;
              mapPos.x += rayStep.x;
              mask = um::bvec3(true, false, false);
            }
            else {
              sideDist.z += deltaDist.z;
              mapPos.z += rayStep.z;
              mask = um::bvec3(false, false, true);
            }
          }
          else {
            if (sideDist.y < sideDist.z) {
              sideDist.y += deltaDist.y;
              mapPos.y += rayStep.y;
              mask = um::bvec3(false, true, false);
            }
            else {
              sideDist.z += deltaDist.z;
              mapPos.z += rayStep.z;
              mask = um::bvec3(false, false, true);
            }
          }
        }

        um::vec3 color;
        if (mask.x) {
          color = um::vec3(03/255.0f, 14/255.0f, 24/255.0f);
        }
        if (mask.y) {
          color = um::vec3(0x68/255.0f, 0x7a/255.0f, 0x8f/255.0f); 
        }
        if (mask.z) {
          color = um::vec3(0x30/255.0f, 0x41/255.0f, 0x5d/255.0f);
        }
        
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