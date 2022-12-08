

class effect_dancefloor
{
private:
    demo_app* app;

    ushine::core::texture tunnel_texture, dance_texture,
		logo_arg,logo_a0;


    float arg_alpha=0.0, a0_alpha=0.0;
    int arg_dir=1, a0_dir=1;

public:
    effect_dancefloor(demo_app* _app)
    {
        app = _app;
    }

    virtual int init()
    {
        bool textLoaded = true;

        std::string imagePath = app->getResourcePath() + "az0!_envmap12_02.png";
        textLoaded = textLoaded & app->png_loader.load(tunnel_texture, imagePath);

        imagePath = app->getResourcePath() + "oft_intro_5.png";
        textLoaded = textLoaded & app->png_loader.load(logo_arg, imagePath);

        imagePath = app->getResourcePath() + "oft_intro_6.png";
        textLoaded = textLoaded & app->png_loader.load(logo_a0, imagePath);

        textLoaded = textLoaded & dance_texture.create(app, 256,256);

        if (!textLoaded)
            return -1;

        return 0;
    }

	virtual int render_dancefloor(int timestamp,ushine::core::texture& dest_buffer)
	{
        namespace um = ushine::math;
        namespace uc = ushine::core;
        namespace ur = ushine::renderer;

        float iTime = (app->additive_progress-app->first_frame_tick)/1500.0f;

        float w = dest_buffer.get_width();
        float h = dest_buffer.get_height();

        int* buf = (int*)dest_buffer.lock();

		app->basic2d.clear(buf, 0, 0, 0, 0,w,h);

        auto iResolution = um::vec2(w, h);
        for (int y=0; y<h; y++) {
        for (int x=0; x<w; x++) {
            auto fragCoord = um::vec2(x, y);
            auto px = 4.0f * (-iResolution + 2.0f * fragCoord) / iResolution.y;
            auto id = 0.5f + 0.5f*std::cos(iTime + std::sin(um::dot(um::floor(px+0.5f), um::vec2(113.1f,17.81f)))*43758.545f);
            auto co = 0.5f + 0.5f*um::cos(iTime + 3.5f*id + um::vec3(0.0f,1.57f,3.14f) );
            auto pa = um::smoothstep( 0.0f, 0.5f, (0.5f*um::cos(6.2831f * px)+0.5f)*id );
            auto color = co * pa.x * pa.y *id;
            *buf++ = ur::color_f(1.7f, color.x, color.y, color.z).to_argb(true);
        }
        }

        dest_buffer.unlock();
        return 0;


	}


	ushine::renderer::color_f deform(ushine::math::base_vector_2<float>& p, int* texture, float animation_time)
	{

				ushine::math::base_vector_2<float> uv;

				ushine::math::base_vector_2<float> q;
				q.x = sin(1.1*animation_time+p.x);
				q.y = sin(1.2*animation_time+p.y);

				float inner_radius = sqrt(q.dot(q));

				//uv.x = p.x+cos(sin(0.2*additive_progress/4)); //*sqrt(inner_radius); //sin(0.5+0.5*additive_progress)+p.x*sqrt(inner_radius);
				//uv.y = p.y-sin(cos(0.6*additive_progress)); //*sqrt(inner_radius)); //sin(0.6+0.22*additive_progress)+p.y*sqrt(inner_radius);

				uv.x = p.x*sqrt(inner_radius);//+cos(inner_radius);
				uv.y = p.y*sqrt(inner_radius);//+sin(inner_radius);

				//uv.x*=0.5;
				//uv.y*=0.5;


				int uvx = int(fabs(uv.x*256))%255;
				int uvy = int(fabs(uv.y*256))%255;

				float r = 1.5/(1.0+p.dot(p));

				int argb = texture[uvx+uvy*256];

				ushine::renderer::color_f src_col;
				src_col.from_argb(argb);

				return src_col;
	}


	virtual int render_tunel(int timestamp,ushine::core::texture& dest_buffer,float music_time)
	{
        //app->additive_progress = timestamp;

        float animation_time = (app->additive_progress-app->first_frame_tick)/2000;

        float w = dest_buffer.get_width();
        float h = dest_buffer.get_height();
        float png_w = dance_texture.get_width();
        float png_h = dance_texture.get_height();

        int* buf = (int*)dest_buffer.lock();
        int* texture = (int*)dance_texture.lock();

        //basic2d.clear(buf, 255, 0, 0, 0);

        if (buf == nullptr)
            return -1;

        ushine::math::base_vector_2<float>
                    move1;
        move1.x = cos(animation_time*1)*0.4;
        move1.y = sin(animation_time*1.5)*0.4;


		ushine::math::base_vector_2<float> zero;
		zero.zero();


        for (int i=0; i<w; i++)
            for (int j=0; j<h; j++)
            {
				ushine::math::base_vector_2<float> p, s, distance, uv;
				ushine::renderer::color_f total;


				p.x = -1+2.0*i/w;
				p.y = -1+2.0*j/h;

				s=p;

				//s.x = i*2; // p.x;
				//s.y = j*2; //p.y;

				distance.x = (zero.x-p.x)/10.0f;
                distance.y = (zero.y-p.y)/10.0f;
				float wobble = 2.0;

				for (int iterations=0; iterations<4; iterations++)
				{

					ushine::renderer::color_f deformed_col = deform(s,texture,animation_time);
					deformed_col.r = ushine::math::smoothstep<float>(0.1, 1.5, deformed_col.r)*wobble;
					deformed_col.g = ushine::math::smoothstep<float>(0.1, 1.5, deformed_col.g)*wobble;
					deformed_col.b = ushine::math::smoothstep<float>(0.1, 1.5, deformed_col.b)*wobble;

					total.r+=deformed_col.r*wobble;
					total.g+=deformed_col.g*wobble;
					total.b+=deformed_col.b*wobble;

					wobble*=0.99;
					s+=distance;
					s.x=s.x+sin(distance.x);
					s.y=s.y+sin(distance.y);
				}

				total.r=total.r/4.0;
				total.g=total.g/4.0;
				total.b=total.b/4.0;

				float r = 1.5/(1.0+p.dot(p));

				ushine::renderer::color_f src_col;
				src_col = total;

				src_col.r*=r/4;
				src_col.g*=r/4;
				src_col.b*=r/4; //total.z*r;
				src_col.a=1.0;

				int dst_argb = src_col.to_argb();
				buf[i+j*(int)w]=dst_argb;
            }

        tunnel_texture.unlock();

        if (music_time>20.5 && arg_dir==1)
        {
            arg_alpha+=(timestamp-app->additive_progress)/1000.0;
            if (arg_alpha>1.0) 
            {
                arg_alpha=1.0;
                arg_dir=0;
            }
        }
        if (music_time>23 && arg_dir==0)
        {
            arg_alpha-=(timestamp-app->additive_progress)/1000.0;
            if (arg_alpha<0.0) 
            {
                arg_alpha=0.0;
                arg_dir=-1;
            }
        }

        if (music_time>23 && a0_dir==1)
        {
            a0_alpha+=(timestamp-app->additive_progress)/1000.0;
            if (a0_dir>1.0) 
            {
                a0_alpha=1.0;
                a0_dir=0;
            }
        }
        if (music_time>25 && a0_dir==0)
        {
            a0_alpha-=(timestamp-app->additive_progress)/1000.0;
            if (a0_alpha<0.0) 
            {
                a0_alpha=0.0;
                a0_dir=-1;
            }
        }

        if (arg_dir>=0) logo_arg.blit_to_buf(buf,0,0,w,h,arg_alpha*256);
        if (a0_dir>=0) logo_a0.blit_to_buf(buf,0,0,w,h,a0_alpha*256);

        dest_buffer.unlock();
        return 0;

	}

virtual int render(int timestamp,ushine::core::texture& dest_buffer, float music_time)
{
    
    render_dancefloor(timestamp,dance_texture);
    return render_tunel(timestamp,dest_buffer,music_time);
}


};