

class effect_oilywater
{
private:
    demo_app* app;

    ushine::core::texture picture;
    ushine::core::texture ant_up;
    ushine::core::texture ant_down;

    int pic_alpha_enabled=0;
    float pic_animation_time=0;
    float pic_alpha=0.0,pic_alpha2=0.0;

    float effect_alpha=0.0;
    float effect_start_tick=-1;

    float scroll_y=0;


public:
    effect_oilywater(demo_app* _app)
    {
        app = _app;
    }

    ~effect_oilywater()
    {
       // df_texture.release();
    }

    virtual int init()
    {
        bool textLoaded = true;

        std::string imagePath = app->getResourcePath() + "head.png"; //"rw_2018_logo_small.png";
        textLoaded = textLoaded & app->png_loader.load(picture, imagePath);

        imagePath = app->getResourcePath() + "ant_white.png";
        textLoaded = textLoaded & app->png_loader.load(ant_up, imagePath);
        imagePath = app->getResourcePath() + "ant_white_down.png";
        textLoaded = textLoaded & app->png_loader.load(ant_down, imagePath);

        if (!textLoaded)
            return -1;

        pic_alpha=0;

        return 0;
    }

	virtual int render(int timestamp,ushine::core::texture& dest_buffer, float music_time)
	{
        float animation_time = (app->additive_progress-app->first_frame_tick)/2000;
        
        if (effect_start_tick<0)
            effect_start_tick=animation_time;

        float w = dest_buffer.get_width();
        float h = dest_buffer.get_height();
        int* buf = (int*)dest_buffer.lock();

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

                p.x = -1+2.0*i/w-0.2;
                p.y = -1+2.0*j/h-0.8;

                float _a = animation_time*40.0;

                float _d,_e,_f,_g=1.0/40.0,_h,_i,_r,_q;

                _e=165.0*(p.x*0.5+0.5);
                _f=160.0*(p.y*0.5+0.5);
                _i=40.0+sin(_e*_g+_a/50.0)*20.0;
                _d=40.0+cos(_f*_g/2.0)*18.0+cos(_e*_g)*18.0;

                _r=sqrt(pow(_i-_e,2.0)+pow(_d-_f,2.0));
                _q=_f/_r;

                _e=(_r*cos(_q))-_a/2.0;
                _f=(_r*sin(_q))-_a/2.0;

                _d=sin(_e*_g)*176.0+sin(_e*_g)*64.0+_r;
                _h=((_f+_d)+_a/2.0)*_g;

                _i=cos(_h+_r*p.x/1.3)*(_e+_e+_a)+cos(_q*_g*6.0)*(_r+_h/3.0);

                _h=sin(_f*_g)*144.0-sin(_e*_g)*212.0*p.x;
                _h=(_h+(_f-_e)*_q+sin(_r-(_a+_h)/7.0)*10.0+_i/4.0)*_g;

                _i+=cos(_h*2.3*sin(_a/350.0-_q))*184.0*sin(_q-(_r*4.3+_a/12.0)*_g)+tan(_r*_g+_h)*184.0*cos(_r*_g+_h);
                _i=fmod(_i/8.6,256.0)/64.0;

                if(_i<0.0) _i+=4.0;
                if(_i>=2.0) _i=4.0-_i;

                _d=_r/350.0;
                _d+=sin(_d*_d*8.0)*0.52;

                _f=(sin(_a*_g)+1.0)/2.0;

                ushine::renderer::color_f src_col;
                src_col.r=(_f*_i/2.6)		*_d*p.x + (_i/4.3+_d/18.0) *_d*(1.0-p.x);
                src_col.g=(_i/2.0+_d/13.0)*_d*p.x + (_i/2.0+_d/18.0)  *_d*(1.0-p.x);
                src_col.b=_i 			*_d*p.x + _i *_d*(1.0-p.x);


                src_col.r*=2;
                src_col.g*=2;
                //src_col.b*=2;

                src_col.a=effect_alpha;

                int dst_argb = src_col.to_argb(true);

                buf[i+j*(int)w]=dst_argb;
            }

        if (effect_alpha<1.0)
        {
            effect_alpha+=(float)((animation_time-effect_start_tick)/50);
        } else effect_alpha=1.0;


        if (music_time>=130)
        {
            if (pic_alpha_enabled==0) {
                pic_alpha_enabled=1;
                pic_animation_time=animation_time;
            }
        }

        if (pic_alpha_enabled==1 || pic_alpha_enabled==2)
        {
            float alpha_growth = (float)((animation_time-pic_animation_time)/50);

            if (pic_alpha_enabled==1)
            {
                pic_alpha+= alpha_growth;
                pic_alpha2+=alpha_growth;
            }
            if (pic_alpha_enabled==2)
            {
                pic_alpha-= alpha_growth/2;
                if (pic_alpha<0)pic_alpha=0;
            }

            picture.blit_to_buf(buf, 0,0, w,h, int(pic_alpha*256));

            if (pic_alpha>=1.0)
            {
                pic_alpha=1.0;
                pic_alpha2=1.0;

                if (music_time>142)
                    pic_alpha_enabled=2;
            } 

        
            if (pic_alpha_enabled==2 && pic_alpha<=0.0)
            {
                pic_alpha_enabled=3;
                pic_alpha=0.0;
            }
        }

        //if (pic_alpha_enabled==1 || pic_alpha_enabled==2)
        //{
         //   dance_floor(buf,128,w,h);
        //}

        animation_time = (timestamp-app->additive_progress)/20;

        scroll_y-=animation_time;
        if (scroll_y<-240) scroll_y=0;  

        float iter_y = scroll_y;

        for (float iter_y=scroll_y; iter_y<240; iter_y+=80)
        {
            ant_up.blit_to_buf(buf, w-50,0+int(iter_y),w,h, 128*pic_alpha2);//pic_alpha2*256);
        }

        for (float iter_y=scroll_y; iter_y<240; iter_y+=80)
        {
            ant_down.blit_to_buf(buf, 5,160-int(iter_y),w,h, 128*pic_alpha2);//pic_alpha2*256);
        }

        dest_buffer.unlock();

        //app->additive_progress = timestamp;
        return 0;
	}

};