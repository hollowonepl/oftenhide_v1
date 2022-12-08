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

class Particle {
private:
  um::vec2 speed;
  float radius;
  float bounce;
  size_t width;
  size_t height;
  std::vector<Particle*> neighbors;

public:
  um::vec2 position;

  Particle(um::vec2 p, um::vec2 s, float r, float b, size_t w, size_t h) 
    : position(p), speed(s), radius(r), bounce(b), width(w), height(h) {}

  void move() {
    position += speed;
    
    if (position.x < 0.0f) {
      position.x = 0.0f;
      speed.x *= bounce;
    } else if (position.x > width) {
      position.x = static_cast<float>(width);
      speed.x *= bounce;
    }

    if (position.y < 0.0f) {
      position.y = 0.0f;
      speed.y *= bounce;
    } else if (position.y >= height) {
      position.y = height-1;
      speed.y *= bounce;
    }
  }

  void clear_neighbors() {
    this->neighbors.clear();
  }

  void add_neighbor(Particle* p) {
    this->neighbors.push_back(p);
  }

  void draw_triangles(int* buf, ur::color_f& color) {
    const size_t s = this->neighbors.size();
    if (s <= 1) return;

    auto argb = color.to_argb();
    for (size_t i = 1; i < s -1; i++) {
      for (size_t j = i+1; j < s; j++) {
        um::vec3 pos, npos, nnpos;

        pos.x = this->position.x; pos.y = this->position.y;
        npos.x = this->neighbors[i]->position.x; npos.y = this->neighbors[i]->position.y;
        nnpos.x = this->neighbors[j]->position.x; nnpos.y = this->neighbors[j]->position.y;
    
        auto v4_pos = um::vec4(pos.x, pos.y, pos.z, 1);
        auto v4_npos = um::vec4(npos.x, npos.y, npos.z, 1);
        auto v4_nnpos = um::vec4(nnpos.x, nnpos.y, nnpos.z, 1);

        DrawTriangleFilledBlended(buf, argb, 
            v4_pos, v4_npos, v4_nnpos, SCREEN_W, SCREEN_H-1);
      }
    }
  }
};

class HelloApp : public uc::application {
public:
	uc::texture backbuffer;
	ur::basic2d_context basic2d;
  std::vector<Particle*> particles;
  ushine::renderer::color_f color;
  float rs, gs, bs;

  const static size_t PARTICLES = 250;
  const static size_t DISTANCE = 28;
  const static size_t DISTANCE_SQ = 28 * 28;

	virtual int on_init() {
		CreateWnd(SCREEN_W, SCREEN_H, true, true);
		basic2d.init(SCREEN_W, SCREEN_H);	

		auto loaded = backbuffer.create(this, SCREEN_W, SCREEN_H);

		if (!loaded) {
			get_logger()->LogLine(std::string("Texture load failed!"));
			CloseWnd();
			return -1;
		}

    const float radius{4};
    const float bounce{-1};
    const float speed_max{1.5};
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<float> speed_dist{-speed_max, speed_max};
    std::uniform_real_distribution<float> x_dist{0, SCREEN_W-1};
    std::uniform_real_distribution<float> y_dist{0, SCREEN_H-1};
    std::uniform_real_distribution<float> color_speed_dist {0.0007f, 0.015f};

    for (int i = 0; i < PARTICLES; i++) {
      this->particles.push_back(new Particle(
        um::vec2(
          x_dist(gen), 
          y_dist(gen)
        ),
        {speed_dist(gen), speed_dist(gen)},
        radius,
        bounce,
        SCREEN_W,
        SCREEN_H
      ));
    }

    color = ushine::renderer::color_f(0.1f, 0.5f, 0.5f, 0.2f);
    rs = color_speed_dist(gen);
    gs = color_speed_dist(gen);
    bs = color_speed_dist(gen);

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
    
    std::for_each(this->particles.begin(), this->particles.end(), [](Particle* p) { p->move(); });
    auto j = this->particles.begin();
    std::for_each(this->particles.begin(), this->particles.end(), [&](Particle* p1) { 
      j++;
      p1->clear_neighbors();
      p1->add_neighbor(p1);
      std::for_each(j, this->particles.end(), [&](Particle *p2) {
        const float d_sq = um::distance_sq(p1->position, p2->position);
        if (d_sq > 0 && d_sq <= DISTANCE_SQ) {
          p1->add_neighbor(p2);
        }
      });
      p1->draw_triangles(buf, color);
    });
    auto nr = color.r + rs;
    auto ng = color.g + gs;
    auto nb = color.b + bs;
    rs = (nr > 1.0 || nr < 0) ? -rs : rs;
    gs = (ng > 1.0 || ng < 0) ? -gs : gs;
    bs = (nb > 1.0 || nb < 0) ? -bs : bs;
    color.r = nr;
    color.g = ng;
    color.b = nb;

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