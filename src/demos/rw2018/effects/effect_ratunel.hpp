

class effect_ratunel 
{
private:
    demo_app* app;
    bool nologo=false;

	ushine::core::texture demo_effect_03_tex_01;
    ushine::core::texture logo;

    int pic_alpha_enabled=0;
    float pic_animation_time=0;
    float pic_alpha=0.0;

public:
    effect_ratunel(demo_app* _app)
    {
        app = _app;
    }

    virtual int init(bool _nologo=false)
    {
        bool textLoaded = true;
        nologo = _nologo;

        std::string imagePath = app->getResourcePath() + "az0!_envmap12_02.png";
        textLoaded = textLoaded & app->png_loader.load(demo_effect_03_tex_01, imagePath);

        if (!nologo)
        {
            imagePath = app->getResourcePath() + "oft_intro_2.png";
            textLoaded = textLoaded & app->png_loader.load(logo, imagePath);
        }

        if (!textLoaded)
            return -1;

        return 0;
    }

	virtual int render(int timestamp,ushine::core::texture& dest_buffer, float music_time)
	{
        //app->additive_progress = timestamp;

        float animation_time = (app->additive_progress-app->first_frame_tick)/2000;

        float w = dest_buffer.get_width();
        float h = dest_buffer.get_height();
        float png_w = demo_effect_03_tex_01.get_width();
        float png_h = demo_effect_03_tex_01.get_height();

        int* buf = (int*)dest_buffer.lock();
        int* texture = (int*)demo_effect_03_tex_01.lock();

        //basic2d.clear(buf, 255, 0, 0, 0);

        if (buf == nullptr)
            return -1;

        ushine::math::base_vector_2<float>
                    move1;
        move1.x = cos(animation_time*1)*0.4;
        move1.y = sin(animation_time*1.5)*0.4;

        if (music_time>=7)
        {
            if (pic_alpha_enabled==0) {
                pic_alpha_enabled=1;
                pic_animation_time=animation_time;
            }
        }

        if (pic_alpha_enabled==1 || pic_alpha_enabled==2)
        {
            float alpha_growth = (float)((animation_time-pic_animation_time)/50);

            if (pic_alpha_enabled==1 && pic_alpha<1.0)
                pic_alpha+= alpha_growth;
            if (pic_alpha_enabled==2 && pic_alpha>0.0)
                pic_alpha-= alpha_growth;

            if (pic_alpha>=1.0 && music_time>=11.5) pic_alpha_enabled=2;

            if (pic_alpha_enabled==2 && pic_alpha<=0.01)
                pic_alpha=0;
        }



        for (int i=0; i<w; i++)
            for (int j=0; j<h; j++)
            {

                ushine::math::base_vector_2<float> p, uv;
                p.x = -1+2.0*i/w+move1.x;
                p.y = -1+2.0*j/h+move1.y;

                float a = atan2(p.y, p.x);
                float r = sqrt(p.dot(p))/2;

                ////classic tunnel
                uv.x = 0.25*animation_time+.1/r;
                uv.y = 0.25*animation_time+a/3.1415;

                /////normalizing the uv
                int uvx = abs((int)(uv.x*(png_w-1))%255);
                int uvy = abs((int)(uv.y*(png_h-1))%255);

                ushine::renderer::color_f src_col;

                int src_argb = texture[uvx+uvy*demo_effect_03_tex_01.get_width()];

                src_col.from_argb(src_argb);

                ///dim the center
                src_col.r*=(r*10);
                src_col.g*=(r*10);
                src_col.b*=(r*10);

                if (!nologo)
                    src_col.a=pic_alpha;
                else 
                    src_col.a=1.0;

                int dst_argb = src_col.to_argb(true);

                buf[i+j*(int)w]=dst_argb; //src_argb;
            }

        demo_effect_03_tex_01.unlock();

        if (!nologo)
        {
        logo.blit_to_buf(buf, 0,0, w,h, pic_alpha*256);
        }

        dest_buffer.unlock();
        return 0;

	}

};