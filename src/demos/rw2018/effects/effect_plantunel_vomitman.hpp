

class effect_plantunel_vomitman
{
private:
    demo_app* app;

	ushine::core::texture tunel_texture;
    ushine::core::texture vomitmantexture;

public:
    effect_plantunel_vomitman(demo_app* _app)
    {
        app = _app;
    }

    virtual int init()
    {
        bool textLoaded = true;

        std::string imagePath = app->getResourcePath() + "az0!_envmap12_05.png";
        textLoaded = textLoaded & app->png_loader.load(tunel_texture, imagePath);

        imagePath = app->getResourcePath() + "beksinski_rzyg.png";
        textLoaded = textLoaded & app->png_loader.load(vomitmantexture, imagePath);

        if (!textLoaded)
            return -1;

        return 0;
    }

	virtual int render(int timestamp,ushine::core::texture& dest_buffer)
	{
        //app->additive_progress = timestamp;

        float animation_time = (app->additive_progress-app->first_frame_tick)/2000;

        float w = dest_buffer.get_width();
        float h = dest_buffer.get_height();
        float png_w = tunel_texture.get_width();
        float png_h = tunel_texture.get_height();

        int* buf = (int*)dest_buffer.lock();
        int* texture = (int*)tunel_texture.lock();

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

                float an = animation_time*.25;

                float x = p.x*cos(an)-p.y*sin(an);
                float y = p.x*sin(an)+p.y*cos(an);

                uv.x = 0.25*x/fabs(y)+an;
                uv.y = 0.25*1/fabs(y)-an;

                int uvx = abs((int)(uv.x*(png_w-1))%255);
                int uvy = abs((int)(uv.y*(png_h-1))%255);

                ushine::renderer::color_f src_col;

                int src_argb = texture[uvx+uvy*tunel_texture.get_width()];

                src_col.from_argb(src_argb);
                ////image process the effect

                    src_col.r*=fabs(y);
                    src_col.g*=fabs(y);
                    src_col.b*=fabs(y);


                int dst_argb = src_col.to_argb();
                buf[i+j*(int)w]=dst_argb;
            }
        tunel_texture.unlock();

        vomitmantexture.blit_to_buf(buf, 0,0, w,h, 255);

        dest_buffer.unlock();
        return 0;


	}

};