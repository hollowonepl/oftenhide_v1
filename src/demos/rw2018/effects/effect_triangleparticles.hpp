
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


class effect_triangleparticles
{
private:
    demo_app* app;

    ushine::core::texture picture;

    float first_frame=-1.0f;


    std::vector<Particle*> particles;
    ushine::renderer::color_f color;
    float rs, gs, bs;

    const static size_t PARTICLES = 250;
    const static size_t DISTANCE = 28;
    const static size_t DISTANCE_SQ = 28 * 28;

public:
    effect_triangleparticles(demo_app* _app)
    {
        app = _app;
    }

    virtual int init()
    {
        bool textLoaded = true;

        std::string imagePath = app->getResourcePath() + "final_notes2.png";
        textLoaded = textLoaded & app->png_loader.load(picture, imagePath);


        if (!textLoaded)
            return -1;

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


	float noise_seed(ushine::math::base_vector_2<float> co){
		ushine::math::base_vector_2<float> seed_v;
		seed_v.x = 12.9898;
		seed_v.y = 78.233;
		float dot_co = co.dot(seed_v);
		//
		dot_co = sin(dot_co) * 43758.5453;
		return dot_co-floor(dot_co);
	}


	virtual int tv_snowing_effect(float timestamp, ushine::core::texture& dest_buffer)
	{
		float animation_time = (app->additive_progress-app->first_frame_tick)/2000.0f;

		float w = dest_buffer.get_width();
		float h = dest_buffer.get_height();

		int* buf = (int*)dest_buffer.lock();

		if (buf == nullptr)
			return -1;

		for (int i=0; i<w; i++)
			for (int j=0; j<h; j++)
			{
				ushine::math::base_vector_2<float> uv;

				uv.x = -1+2.0*i/w;
				uv.y = -1+2.0*j/h;

				//tv_snowing
				float ra=noise_seed(uv*animation_time)-.5;

				ushine::math::base_vector_2<float> seed_v;
				seed_v.set(uv.y, uv.y);
				float ral=noise_seed((seed_v*30.0f).integral()*animation_time+1.0f)-.5;

				float col=0.25;
				col=col*.2+(.3+ra+ral*.5)
					* ushine::math::modulo<float>(uv.y+animation_time*2.,.045); //0.45 tv snowing lines power

    			ushine::renderer::color_f total;
    			total.r = fabs(col*sin(animation_time/2)*2);
    			total.g = fabs(col*sin(animation_time/2)*2);
    			total.b = fabs(col*sin(animation_time/2)*2);
    			total.a = 1.0;

          ushine::renderer::color_f previous;
          previous.from_argb(buf[i+j*(int)w]);

          total.r=(total.r*1.3+previous.r*0.2)/2;
          total.g=(total.r*1.3+previous.g*0.7)/2;
          total.b=(total.r*1.3+previous.b*0.3)/2;

          int dst_argb = total.to_argb();
  				buf[i+j*(int)w]=dst_argb;

			}

      dest_buffer.unlock();
      return 0;
	}




	virtual int render(int timestamp,ushine::core::texture& dest_buffer)
	{
    if (first_frame==-1.0)
      first_frame = timestamp;

    float iTime = (app->additive_progress-first_frame)/5000.0f;

		float w = dest_buffer.get_width();
		float h = dest_buffer.get_height();

		int* buf = (int*)dest_buffer.lock();

		    app->basic2d.clear(buf, 0, 0, 0, 0,w,h);

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


        dest_buffer.unlock();

        tv_snowing_effect(timestamp,dest_buffer);


        return 0;


	}

};