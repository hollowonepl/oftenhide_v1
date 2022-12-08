

class effect_lighting
{
private:
    demo_app* app;

    ushine::math::mat2 m;

    float nologo=false;

    ushine::core::texture logo1;
	float img_alpha=0.0;

public:
    effect_lighting(demo_app* _app)
    {
        app = _app;
    }

    virtual int init(bool _nologo=false)
    {
        bool textLoaded = true;
        nologo=_nologo;

        if (!nologo)
        {
            std::string imagePath = app->getResourcePath() + "oft_intro_1.png";
            textLoaded = textLoaded & app->png_loader.load(logo1, imagePath);
        }
        if (!textLoaded)
            return -1;

		m = ushine::math::mat2( 0.8f,  0.06f, -0.6f,  0.8f );

        return 0;
    }


	float hash( float n )
	{
		using namespace ushine::math;
		return fract(sin(n)*43758.5453f);
	}

	float noise( ushine::math::vec2 x )
	{
		using namespace ushine::math;
		vec2 p = x.integral();
		vec2 f = x.fractional();

		f = f*f*(3.0f-f*2.0f);

		float n = p.x + p.y*57.0;

		float res = mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
						mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y);
		return res;
	}


	float fbm(ushine::math::vec2 _p)
	{
		ushine::math::vec2 p = _p;
		float f = 0.0f;
		ushine::math::mat2 mm;

		f +=0.50000f*abs(noise(p)-1.)*2.0f;
		mm = m*2.02f;
		p=mm*p;

		f +=0.25000f*abs(noise(p)-1.)*2.0f;
		mm = mm*2.03f;
		p=mm*p;

		f +=0.12500f*abs(noise(p)-1.)*2.0f;
		mm = mm*2.01f;
		p=mm*p;

		f +=0.06250f*abs(noise(p)-1.)*2.0f;
		mm = mm*2.04f;
		p=mm*p;

		f +=0.03125f*abs(noise(p)-1.)*2.0f;
		
		return f/.96875;

	}

    virtual ushine::math::vec4
        fragShader(
            ushine::math::vec4 input_color, 
            ushine::math::vec2 input_coords, 
            ushine::math::vec2 resolution,
            float global_time)
        {
            ushine::math::vec4 fragColor = input_color;
           
			ushine::math::vec2 q = input_coords / resolution;
			ushine::math::vec2 p = 2.0f*q-1.0f;
            float r = p.length();

			p.x *= resolution.x/resolution.y;

    		float f = fbm(p+global_time);
			f *= r*2.0f-.5f;
			f = (1.0f-f);

            fragColor.x= 0.1/f;
            fragColor.y= 0.2/f;
            fragColor.z= 0.3/f;
            fragColor.w= 1.5/f;

			fragColor.x=sqrt(abs(fragColor.x))*0.5;
			fragColor.y=sqrt(abs(fragColor.y))*0.5;
			fragColor.z=sqrt(abs(fragColor.z))*0.5;
			fragColor.w=sqrt(abs(fragColor.w))*0.5;

            return fragColor;
        }



	virtual int render_lighting(
        int timestamp,
        ushine::core::texture& dest_buffer,
        float music_time)
	{
        float animation_time = (app->additive_progress-app->first_frame_tick)/2000;

        float w = dest_buffer.get_width();
        float h = dest_buffer.get_height();

        int* buf = (int*)dest_buffer.lock();

        if (buf == nullptr)
            return -1;

        /*
        *************************
        */

		for (int i=0; i<w; i++)
			for (int j=0; j<h; j++)
			{

				ushine::math::vec2 coords(i,j);
                ushine::math::vec2 res(w,h);
                ushine::math::vec4 input_col;
                ushine::math::vec4 output_col;

                output_col = fragShader(input_col,coords,res,animation_time);

                ushine::renderer::color_f render_col;
                
                render_col.r = output_col.x;
                render_col.g = output_col.y;
                render_col.b = output_col.z;
                render_col.a = output_col.w;

                buf[int(i+w*j)]=render_col.to_argb();

			}

        /*
        *************************
        */

        if (!nologo)
        {
            logo1.blit_to_buf(buf, 0,0, w,h, img_alpha*256);
            if (music_time>1.0 && img_alpha<1.0)
                img_alpha+=(timestamp-app->additive_progress)/1000;
        }

        dest_buffer.unlock();
        return 0;

	}

virtual int render(int timestamp,ushine::core::texture& dest_buffer, float music_time)
{
    return render_lighting(timestamp,dest_buffer,music_time);
}


};