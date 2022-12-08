

class effect_pictureshow
{
private:
    demo_app* app;

    ushine::core::texture picture;

public:
    effect_pictureshow(demo_app* _app)
    {
        app = _app;
    }

    virtual int init()
    {
        bool textLoaded = true;

        std::string imagePath = app->getResourcePath() + "rw_2018_logo_small.png";
        textLoaded = textLoaded & app->png_loader.load(picture, imagePath);

        if (!textLoaded)
            return -1;

        return 0;
    }

	virtual int render(int timestamp,ushine::core::texture& dest_buffer)
	{
        //app->additive_progress = timestamp;

        float animation_time = (app->additive_progress-app->first_frame_tick)/2000;

        float w = dest_buffer.get_width();
        float h = dest_buffer.get_height();

        int* buf = (int*)dest_buffer.lock();

        picture.blit_to_buf(buf, 0,0, w,h, 255);

        dest_buffer.unlock();
        return 0;


	}

};