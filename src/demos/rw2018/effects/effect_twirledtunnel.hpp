

class effect_twirledtunnel
{
private:
    demo_app* app;

	ushine::core::texture tunnel_texture;

public:
    effect_twirledtunnel(demo_app* _app)
    {
        app = _app;
    }

    virtual int init()
    {
        bool textLoaded = true;

        std::string imagePath = app->getResourcePath() + "az0!_envmap12_06.png";
        textLoaded = textLoaded & app->png_loader.load(tunnel_texture, imagePath);

        if (!textLoaded)
            return -1;

        return 0;
    }

	virtual int render(int timestamp,ushine::core::texture& dest_buffer)
	{
        //app->additive_progress = timestamp;
        float animation_time = (app->additive_progress-app->first_frame_tick)/500;

        float w = dest_buffer.get_width();
        float h = dest_buffer.get_height();
        float png_w = tunnel_texture.get_width();
        float png_h = tunnel_texture.get_height();

        int* buf = (int*)dest_buffer.lock();
        int* texture = (int*)tunnel_texture.lock();

        //basic2d.clear(buf, 255, 0, 0, 0);

        if (buf == nullptr)
            return -1;

        ushine::math::base_vector_2<float>
                    move1;
        move1.x = cos(sin(animation_time))*0.4;
        move1.y = sin(animation_time*1.5)*0.4;


        for (int i=0; i<w; i++)
            for (int j=0; j<h; j++)
            {
                ushine::math::base_vector_2<float> p, uv;

                p.x = -1+2.0*i/w+move1.x;
                p.y = -1+2.0*j/h+move1.y;

                float r = sqrt( p.dot(p) );
                float a = atan2(p.y,p.x) + 1.5*sin(0.5*r-0.5*animation_time);


                float s = 0.5 + 0.5*cos(2*a);
                s = ushine::math::smoothstep<float>(0.0,1.0,s);
                s = ushine::math::smoothstep<float>(0.0,1.0,s);
                s = ushine::math::smoothstep<float>(0.0,1.0,s);
                s = ushine::math::smoothstep<float>(0.0,1.0,s);

                uv.x = animation_time + 1.0/( r + .2*s);
                uv.y = 3.0*a/3.1416;

                int uvx = abs((int)(uv.x*(png_w-1))%255);
                int uvy = abs((int)(uv.y*(png_h-1))%255);

                ushine::renderer::color_f src_col;

                int src_argb = texture[uvx+uvy*tunnel_texture.get_width()];

                src_col.from_argb(src_argb);
                ////image process the effect


                    float wobble = (0.5 + 0.5*s)*r*r;

                    float ao = 0.5 + 0.5*cos(7.0*a);
                    ao = ushine::math::smoothstep<float>(0.0,0.4,ao)-ushine::math::smoothstep<float>(0.4,0.7,ao);
                    ao = 1.0-0.5*ao;

                    src_col.r*=wobble*ao*2;
                    src_col.g*=wobble*ao*2;
                    src_col.b*=wobble*ao*2;


                int dst_argb = src_col.to_argb();
                buf[i+j*(int)w]=dst_argb;
            }
        tunnel_texture.unlock();

        dest_buffer.unlock();

        return 0;


	}

};