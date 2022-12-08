

class effect_blossomingflower  
{
private:
    demo_app* app;

	ushine::core::texture demo_effect_01_tex_01;


    int pic_alpha_enabled=0;
    float pic_animation_time=0;
    float pic_alpha=0.0;


public:
    effect_blossomingflower(demo_app* _app)
    {
        app = _app;
    }


    virtual int init()
    {
        bool textLoaded = true;

        std::string imagePath = app->getResourcePath() + "az0!_envmap12_01.png";
        textLoaded = textLoaded & app->png_loader.load(demo_effect_01_tex_01, imagePath);


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

        float png_w = demo_effect_01_tex_01.get_width();
        float png_h = demo_effect_01_tex_01.get_height();

        int* buf = (int*)dest_buffer.lock();
        int* texture = (int*)demo_effect_01_tex_01.lock();

        pic_alpha=1.0;
 
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

                float a = atan2(p.y, p.x);
                float r = sqrt(p.dot(p))/2;

                ////twist
                uv.x = r - 0.25*animation_time;
                uv.y = cos(a*5.0+2.0*sin(animation_time+2.2*r));

                /////normalizing the uv
                int uvx = abs((int)(uv.x*(png_w-1))%255);
                int uvy = abs((int)(uv.y*(png_h-1))%255);

                ushine::renderer::color_f src_col;

                int src_argb = texture[uvx+uvy*demo_effect_01_tex_01.get_height()];

                src_col.from_argb(src_argb);

                if (r>1.0)
                {
                    ////image process the effect
                    src_col.r*=(-.5*uv.y);
                    src_col.r*=(-.5*uv.y);
                    src_col.r*=(-.5*uv.y);
                } else
                {
                    ////image process the effect
                    src_col.r*=(-(1.5+r)*uv.y);
                    src_col.r*=(-(1.5+r)*uv.y);
                    src_col.r*=(-(1.5+r)*uv.y);
                }

                if (pic_alpha<=1.0)
                    src_col.apply_blend_alpha(pic_alpha);
                else 
                {
                    src_col.a=1.0;
                    src_col.r*=pic_alpha;
                    src_col.g*=pic_alpha;
                    src_col.b*=pic_alpha;

                    if (pic_alpha>2.0)
                    {  src_col.r=1.0;
                        src_col.g=1.0;
                        src_col.b=1.0;}
                }

                int dst_argb = src_col.to_argb(true);

                buf[i+j*(int)w]=dst_argb;
            }

        demo_effect_01_tex_01.unlock();



        dest_buffer.unlock();
        return 0;

	}

};