

class effect_infinitezoom
{
private:
    demo_app* app;

    ushine::core::texture png;

public:
    effect_infinitezoom(demo_app* _app)
    {
        app = _app;
    }

    virtual int init()
    {
        bool textLoaded = true;

		std::string imagePath = app->getResourcePath() + "a0/tex3.png";
		textLoaded = textLoaded & app->png_loader.load(png, imagePath);


        if (!textLoaded)
            return -1;

        return 0;
    }

    virtual ushine::math::vec4
        fragShader(
            ushine::math::vec4 input_color, 
            ushine::math::vec2 input_coords, 
            ushine::math::vec2 resolution,
            float global_time,
			ushine::core::texture& texture, int* texture_buf
			)
        {
			using namespace ushine::math;
            vec4 fragColor = input_color;

			vec2 r = resolution;
			vec2 p = (input_coords - r*.5f)/r.y;

			float c = 0.,
				  t = global_time, 
				  cs = cos(t), 
				  si = cos(sin(t)), 
				  s;

			auto m = mat2(cs, -si, si, cs);
			p = m*p;//should be p*m

    		auto o = vec4(0);

			for (float i = 0.0f; i<8.0f; i++){
				
					// Fractional time component.
					s = fract((i - t*1.2)/8.);

					vec2 text_coords = p*exp2(s*8.0f)/1e2f + i*.9f;
					
					text_coords.x*=texture.get_width();
					text_coords.y*=texture.get_height();

					int u = int(text_coords.x)%texture.get_width();
					int v = int(text_coords.y)%texture.get_width();

					if (u>=0 && u<texture.get_width() &&
						v>=0 && v<texture.get_height())
						{
							ushine::renderer::color_f col;
							col.from_argb(texture_buf[u+v*texture.get_width()]);

							col.r=col.r * (0.5f - abs(s-0.5f))*0.5f;
							col.g=col.g * (0.5f - abs(s-0.5f))*0.5f;
							col.b=col.b * (0.5f - abs(s-0.5f))*0.5f;
							col.a=col.a * (0.5f - abs(s-0.5f))*0.5f;

							fragColor.x+=col.r;
							fragColor.y+=col.g;
							fragColor.z+=col.b;
							fragColor.w+=col.a;
						}      

				}

            return fragColor;
        }



	virtual int render_lighting(
        int timestamp,
        ushine::core::texture& dest_buffer,
        float music_time)
	{
        float animation_time = (app->additive_progress-app->first_frame_tick)/250;

        float w = dest_buffer.get_width();
        float h = dest_buffer.get_height();

        int* buf = (int*)dest_buffer.lock();

        if (buf == nullptr)
            return -1;


		int* png_buf = (int*)png.lock();

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

                output_col = fragShader(input_col,coords,res,animation_time,png, png_buf);

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
		png.unlock();
        dest_buffer.unlock();
        return 0;

	}

virtual int render(int timestamp,ushine::core::texture& dest_buffer, float music_time)
{
    return render_lighting(timestamp,dest_buffer,music_time);
}


};