

class effect_greenlighttunnel 
{
private:
    demo_app* app;
    bool nologo=false;

	ushine::core::texture png,
        logo_h1, logo_unj;

    float effect_alpha=0.0;
    float h1_alpha=0.0, unj_alpha=0.0;
    int h1_dir=1, unj_dir=1;

public:
    effect_greenlighttunnel(demo_app* _app)
    {
        app = _app;
    }

    virtual int init(bool _nologo=false)
    {
        bool textLoaded = true;
        nologo = _nologo;

        std::string imagePath = app->getResourcePath() + "az0!_envmap12_01.png";
        textLoaded = textLoaded & app->png_loader.load(png, imagePath);

        if (!nologo)
        {
        imagePath = app->getResourcePath() + "oft_intro_3.png";
        textLoaded = textLoaded & app->png_loader.load(logo_h1, imagePath);

        imagePath = app->getResourcePath() + "oft_intro_4.png";
        textLoaded = textLoaded & app->png_loader.load(logo_unj, imagePath);
        }

        if (!textLoaded)
            return -1;

        return 0;
    }

	virtual int render(int timestamp,ushine::core::texture& dest_buffer, float music_time)
	{
        float animation_time = (app->additive_progress-app->first_frame_tick)/2000;

        float w = dest_buffer.get_width();
        float h = dest_buffer.get_height();
        float png_w = png.get_width();
        float png_h = png.get_height();

        int* buf = (int*)dest_buffer.lock();
        int* texture = (int*)png.lock();
        
        if (buf == nullptr)
            return -1;

        ushine::math::base_vector_2<float>
                    move1;
        move1.x = cos(animation_time*1)*0.4;
        move1.y = sin(animation_time*1.5)*0.4;

        if (effect_alpha<1.0)
            effect_alpha+=(timestamp-app->additive_progress)/2000;
        if (effect_alpha>1.0)
            effect_alpha=1.0;



        for (int i=0; i<w; i++)
            for (int j=0; j<h; j++)
            {

				ushine::math::base_vector_2<float> p, uv;
				p.x = -1+2.0*i/w;//+1.0;
				p.y = -1+2.0*j/h;//+move1.y;

				float a = atan2(p.y, p.x);
				float r = sqrt(p.dot(p))/2;

				////

				uv.x = 0.02*p.y+0.03*cos(a*3)/r+animation_time*0.2;
				uv.y = 0.02*p.y+0.03*sin(a*3)/r+animation_time*0.2;

				/////normalizing the uv
				int uvx = abs((int)(uv.x*(png_w-1))%255);
				int uvy = abs((int)(uv.y*(png_h-1))%255);
	

				ushine::renderer::color_f src_col;

				int src_argb = texture[uvx+uvy*png.get_width()];

				src_col.from_argb(src_argb);
				
				///dim the center						
                src_col.r*=(r*10);
                src_col.g*=(r*10);
                src_col.b*=(r*10);
                src_col.a=effect_alpha;

				int dst_argb = src_col.to_argb(true);

				//if (src_col.r>0.1 && src_col.g>0.1 && src_col.b >0.1 )
				    buf[i+j*(int)w]=dst_argb; //src_argb;   
            }

        //printf("frame end\n");

        png.unlock();

        if (!nologo)
        {
            if (music_time>14 && h1_dir==1)
            {
                h1_alpha+=(timestamp-app->additive_progress)/1000.0;
                if (h1_alpha>1.0) 
                {
                    h1_alpha=1.0;
                    h1_dir=0;
                }
            }
            if (music_time>16 && h1_dir==0)
            {
                h1_alpha-=(timestamp-app->additive_progress)/1000.0;
                if (h1_alpha<0.0) 
                {
                    h1_alpha=0.0;
                    h1_dir=-1;
                }
            }

            if (music_time>16 && unj_dir==1)
            {
                unj_alpha+=(timestamp-app->additive_progress)/1000.0;
                if (unj_alpha>1.0) 
                {
                    unj_alpha=1.0;
                    unj_dir=0;
                }
            }
            if (music_time>18 && unj_dir==0)
            {
                unj_alpha-=(timestamp-app->additive_progress)/1000.0;
                if (unj_alpha<0.0) 
                {
                    unj_alpha=0.0;
                    unj_dir=-1;
                }
            }

            if (h1_dir>=0) logo_h1.blit_to_buf(buf,0,0,w,h,h1_alpha*256);
            if (unj_dir>=0) logo_unj.blit_to_buf(buf,0,0,w,h,unj_alpha*256);
        }

        dest_buffer.unlock();
        return 0;

	}

};