

class effect_fade_picture
{
private:
    demo_app* app;

    ushine::core::texture texture[2];

    int texture_idx=0;
    float fade_level=1.0;
    int dir=0;
    float speed=1000;



public:
    effect_fade_picture (demo_app* _app)
    {
        app = _app;
    }

    virtual int init()
    {
        bool textLoaded = true;

        std::string imagePath = app->getResourcePath() + "cross_fade_1.png";
        textLoaded = textLoaded & app->png_loader.load(texture[0], imagePath);

        imagePath = app->getResourcePath() + "cross_fade_2.png";
        textLoaded = textLoaded & app->png_loader.load(texture[1], imagePath);

        if (!textLoaded)
            return -1;

        return 0;
    }

    void reset(int _direction, float _speed, int texture_index=0)
    {
        dir=_direction;
        texture_idx=texture_index;
        speed=_speed;
        if (_direction==0)
            fade_level=1.0;
        else 
            fade_level=0.0;
        
    }

	virtual int render(int timestamp,ushine::core::texture& dest_buffer)
	{
        //app->additive_progress = timestamp;
        float animation_time = (timestamp-app->additive_progress)/speed;

        float w = dest_buffer.get_width();
        float h = dest_buffer.get_height();
        
        int* buf = (int*)dest_buffer.lock();

        int* fade_buf = (int*)texture[texture_idx].lock();

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

       // std::cout << "dir=" << dir <<"; fade_level=" << fade_level << std::endl;

        if (dir==0)
        {
            fade_level-=animation_time;
            if (fade_level<0) fade_level=0.0;
        } else 
        {
            fade_level+=animation_time;
            if (fade_level>1.0) fade_level=1.0;
        }

        dest_buffer.unlock();
        return 0;

	}

};