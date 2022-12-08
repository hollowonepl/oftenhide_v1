

class effect_fade_triangles
{
private:
    demo_app* app;

    ushine::core::texture texture;
    ushine::math::vec2 triangle[3];

    float fade_level=1.0;

public:
    effect_fade_triangles(demo_app* _app)
    {
        app = _app;
    }

    virtual int init()
    {
        bool textLoaded = true;

        std::string imagePath = app->getResourcePath() + "cross_fade_1.png";
        textLoaded = textLoaded & app->png_loader.load(texture, imagePath);

        if (!textLoaded)
            return -1;

		triangle[0].x=0;    triangle[0].y=0;
        triangle[1].x=100;  triangle[1].y=0;
        triangle[2].x=160;  triangle[2].y=120;

        return 0;
    }

	virtual int render(int timestamp,ushine::core::texture& dest_buffer)
	{
        //app->additive_progress = timestamp;
        float animation_time = (timestamp-app->additive_progress)/1000.0f;

        float w = dest_buffer.get_width();
        float h = dest_buffer.get_height();
        
        int* buf = (int*)dest_buffer.lock();

        int* fade_buf = (int*)texture.lock();

        for (auto i=0; i<w; i++)
            for (auto j=0; j<h; j++)
            {
                ushine::renderer::color_f col;

                int idx = i+j*w;
                int src = fade_buf[idx];
                col.from_argb(src);
                if (col.r>fade_level)
                {
                    buf[idx]=0x0;
                }
            }

        fade_level-=animation_time;

        if (fade_level<0) fade_level=0.0;

        dest_buffer.unlock();
        return 0;

	}

};