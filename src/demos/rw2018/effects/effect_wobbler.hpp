

class effect_wobbler
{
private:
    demo_app* app;

	ushine::core::texture wobler_texture;

public:
    effect_wobbler(demo_app* _app)
    {
        app = _app;
    }

    virtual int init()
    {
        bool textLoaded = true;

        std::string imagePath = app->getResourcePath() + "az0!_envmap12_02.png";
        textLoaded = textLoaded & app->png_loader.load(wobler_texture, imagePath);

        if (!textLoaded)
            return -1;

        return 0;
    }

	virtual int render(int timestamp,ushine::core::texture& dest_buffer)
	{
        //app->additive_progress = timestamp;

        float animation_time = (app->additive_progress-app->first_frame_tick)/1000;

        float w = dest_buffer.get_width();
        float h = dest_buffer.get_height();
        float png_w = wobler_texture.get_width();
        float png_h = wobler_texture.get_height();

        int* buf = (int*)dest_buffer.lock();
        int* texture = (int*)wobler_texture.lock();

        if (buf == nullptr)
            return -1;

        ushine::math::base_vector_2<float>
                    move1;
        move1.x = cos(animation_time*1)*0.4;
        move1.y = sin(animation_time*1.5)*0.4;


        for (int i=0; i<w; i++)
            for (int j=0; j<h; j++)
            {


                ushine::math::base_vector_2<float> p, uv;

                p.x = -1+2.0*i/w+move1.x;
                p.y = -1+2.0*j/h+move1.y;

                float a1 = atan2(p.y-move1.y,p.x-move1.x);
                float r1 = sqrt(p.dot(p-move1));
                float a2 = atan2(p.y+move1.y,p.x+move1.x);
                float r2 = sqrt(p.dot(p+move1));

                uv.x = 0.2*animation_time + (r1-r2)*0.25;
                uv.y = sin(2.0*(a1-a2));

                /////normalizing the uv
                int uvx = abs((int)(uv.x*(png_w-1))%255);
                int uvy = abs((int)(uv.y*(png_h-1))%255);

                ushine::renderer::color_f src_col;
                int src_argb = texture[uvx+uvy*wobler_texture.get_width()];

                src_col.from_argb(src_argb);

//                float wobbler = r1*r2*0.8;

                float s = 0.5 + 0.5*cos(2.5*a1);
                s = ushine::math::smoothstep<float>(0.0,1.0,s);
                s = ushine::math::smoothstep<float>(0.0,1.0,s);
                s = ushine::math::smoothstep<float>(0.0,1.0,s);
                s = ushine::math::smoothstep<float>(0.0,1.0,s);

                float wobbler = (0.5 + 0.5*s)*r1;


                src_col.r*=wobbler;
                src_col.g*=wobbler;
                src_col.b*=wobbler;

                int dst_argb = src_col.to_argb();

                buf[i+j*(int)w]=dst_argb;
            }
        wobler_texture.unlock();

        dest_buffer.unlock();
        return 0;


	}

};