
namespace um = ushine::math;
namespace uc = ushine::core;
namespace ur = ushine::renderer;

class effect_credits
{
private:
    demo_app* app;

    ushine::core::texture picture;
    float scroll_y=140;

public:
    effect_credits(demo_app* _app)
    {
        app = _app;
    }

    virtual int init()
    {
        bool textLoaded = true;

        std::string imagePath = app->getResourcePath() + "final_notes2.png";
        textLoaded = textLoaded & app->png_loader.load(picture, imagePath);

        if (!textLoaded)
            return -1;

        return 0;
    }


	virtual int render(int timestamp,ushine::core::texture& dest_buffer)
	{
        float animation_time = (timestamp-app->additive_progress)/80;
        //(app->additive_progress-app->first_frame_tick)/750.0f;

		float w = dest_buffer.get_width();
		float h = dest_buffer.get_height();

		int* buf = (int*)dest_buffer.lock();
        picture.blit_to_buf(buf,0,int(scroll_y),w,h, 255);
        dest_buffer.unlock();

        scroll_y-=animation_time;

        app->additive_progress = timestamp;
    return 0;


	}

};