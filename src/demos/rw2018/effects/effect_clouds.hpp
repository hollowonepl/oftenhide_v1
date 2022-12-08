

class effect_clouds
{
private:
    demo_app* app;

    float nologo=false;

    ushine::core::texture png;
	float img_alpha=0.0;

public:
    effect_clouds(demo_app* _app)
    {
        app = _app;
    }

    virtual int init(bool _nologo=false)
    {
        bool textLoaded = true;
        nologo=_nologo;

		std::string imagePath = app->getResourcePath() + "a0/tex4.png";
		textLoaded = textLoaded & app->png_loader.load(png, imagePath);

        if (!textLoaded)
            return -1;

        return 0;
    }

	float mod(float x, float y)
	{
		return x - y * floor(x/y);
	}

    virtual ushine::math::vec4
        fragShader(
            ushine::math::vec4 input_color, 
            ushine::math::vec2 input_coords, 
            ushine::math::vec2 resolution,
            float global_time,
            ushine::core::texture& texture, int* texture_buf)
        {
			using namespace ushine::math;
            vec4 fragColor = input_color;

			vec2 uv = input_coords/resolution+0.8f;

#define ITERATIONS 6.0f

			for (float i = 0.0f; i<ITERATIONS; i++){
				
					// Fractional time component.
					auto s = mod(i - global_time, ITERATIONS);

					vec2 text_coords = uv * exp2(s-2.0f);
					
					text_coords.x*=texture.get_width();
					text_coords.y*=texture.get_height();

					int u = abs(int(text_coords.x)%texture.get_width());
					int v = abs(int(text_coords.y)%texture.get_width());

					if (u>=0 && u<texture.get_width() &&
						v>=0 && v<texture.get_height())
						{
							ushine::renderer::color_f col;
							col.from_argb(texture_buf[u+v*texture.get_width()]);

							col.r=col.r * s *(ITERATIONS -s) *0.5;
							col.g=col.g * s *(ITERATIONS -s) *0.5;
							col.b=col.b * s *(ITERATIONS -s) *0.5;
							col.a=col.a * s *(ITERATIONS -s) *0.5;

							fragColor.x+=col.r/ITERATIONS;
							fragColor.y+=col.g/ITERATIONS;
							fragColor.z+=col.b/ITERATIONS;
							fragColor.w+=col.a/ITERATIONS;
						}      

				}

            return fragColor;
        }



	virtual int render_clouds(
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
    return render_clouds(timestamp,dest_buffer,music_time);
}


};