

class effect_bluechiplights 
{
private:
    demo_app* app;

	ushine::core::texture effect_texture;

    int move_enabled=0;
    float move_x=150;

public:
    effect_bluechiplights(demo_app* _app)
    {
        app = _app;
    }

    virtual int init()
    {
        bool textLoaded = true;

        std::string imagePath = app->getResourcePath() + "BlueChipWall.png";
        textLoaded = textLoaded & app->png_loader.load(effect_texture, imagePath);

        if (!textLoaded)
            return -1;

        return 0;
    }

	virtual int render(int timestamp,ushine::core::texture& dest_buffer, float music_time)
	{
        float animation_time = (app->additive_progress-app->first_frame_tick)/2000+0.35;

        float w = dest_buffer.get_width();
        float h = dest_buffer.get_height();

        float png_w = effect_texture.get_width();
        float png_h = effect_texture.get_height();

        int* buf = (int*)dest_buffer.lock();
        int* texture = (int*)effect_texture.lock();

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

                p.x = -1+2.0*i/w+1.0;
                p.y = -1+2.0*j/h+1.0;

                float a = atan2(p.y, p.x);
                float r = sqrt(p.dot(p));

                uv.x = 7.0*a/3.1416;
                uv.y = -animation_time+ sin(7.0*r+animation_time) + .7*cos(animation_time+7.0*a);
                uv.x*=(0.5);
                uv.y*=(0.5);

                /////normalizing the uv
                int uvx = abs((int)(uv.x*(png_w-1))%255);
                int uvy = abs((int)(uv.y*(png_h-1))%255);

                ushine::renderer::color_f src_col;
                int src_argb = texture[uvx+uvy*effect_texture.get_width()];

                src_col.from_argb(src_argb);

                float wobbler = powf(r+cos(animation_time*5.0+a*0.5*3.1415),1.2);

                src_col.r*=wobbler/1.3;
                src_col.g*=wobbler/1.3;//0.8;
                src_col.b*=wobbler/1.3;//1.4;

                int dst_argb = src_col.to_argb();

                buf[i+j*(int)w]=dst_argb;
            }

        effect_texture.unlock();

        dest_buffer.unlock();
        return 0;

	}

};