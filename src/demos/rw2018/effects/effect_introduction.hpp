

class effect_introduction
{
private:
    demo_app* app;

	ushine::core::texture demo_effect_04_tex_01;

    ushine::core::texture container60;

    ushine::core::texture ant, moth, mosquito, fly,rw2018;

    int box[4][3];

	effect_blossomingflower* effect_blossomingflower;
    effect_wobbler* effect_wobbler;
    effect_bluechiplights* effect_bluechiplights;
    effect_lighting* effect_lighting;
    effect_clouds* effect_clouds;
    effect_greenlighttunnel* effect_greenlighttunnel;
    effect_ratunel* effect_ratunel;
    effect_twirledtunnel* effect_twirledtunnel;

public:
    effect_introduction(demo_app* _app)
    {
        app = _app;

        for (auto i=0; i<4; i++)
            for (auto j=0; j<3; j++)
                box[i][j]=0;



    }


    virtual int init()
    {
        bool textLoaded = true;

        std::string imagePath = app->getResourcePath() + "a0/tex2.png";
        textLoaded = textLoaded & app->png_loader.load(demo_effect_04_tex_01, imagePath);

        imagePath = app->getResourcePath() + "ant_black.png";
        textLoaded = textLoaded & app->png_loader.load(ant, imagePath);
        imagePath = app->getResourcePath() + "moth_black.png";
        textLoaded = textLoaded & app->png_loader.load(moth, imagePath);
        imagePath = app->getResourcePath() + "mosquito_black.png";
        textLoaded = textLoaded & app->png_loader.load(mosquito, imagePath);
        imagePath = app->getResourcePath() + "fly_black.png";
        textLoaded = textLoaded & app->png_loader.load(fly, imagePath);

        imagePath = app->getResourcePath() + "rw2018_60_2.png";
        textLoaded = textLoaded & app->png_loader.load(rw2018, imagePath);

        textLoaded = textLoaded & container60.create(app,60,60);

        if (!textLoaded)
            return -1;

        effect_blossomingflower= new class effect_blossomingflower(app);
        effect_blossomingflower->init();

        effect_wobbler = new class effect_wobbler(app);
        effect_wobbler->init();

        effect_bluechiplights= new class effect_bluechiplights(app);
        effect_bluechiplights->init();

        effect_lighting = new class effect_lighting(app);
        effect_lighting->init(true);

        effect_clouds = new class effect_clouds(app);
        effect_clouds->init(true);

        effect_greenlighttunnel = new class effect_greenlighttunnel(app);
        effect_greenlighttunnel->init(true);

        effect_ratunel = new class effect_ratunel(app);
        effect_ratunel->init(true);

        effect_twirledtunnel = new class effect_twirledtunnel(app);
        effect_twirledtunnel->init();

        return 0;
    }

    virtual int renderChildElements(int*buf, float music_time, float timestamp)
    {

/////////////////////////// SETTING STATES ///////////////////////
            if (music_time>27.0)
                box[0][0]=1;
            
            if (music_time>29.3)
                box[0][1]=1;

            if (music_time>30.3)
                box[0][2]=1;

            if (music_time>33.8)
                box[1][2]=1;

            if (music_time>36.5)
                box[2][2]=1;
            if (music_time>37.5)
                box[3][2]=1;

            if (music_time>40.55)
                box[3][1]=1;
            if (music_time>41.05)
                box[2][1]=1;

            if (music_time>43.5)
                box[2][1]=2;
            if (music_time>44.0)
                box[0][0]=2; 
            if (music_time>44.4)
                box[1][1]=2;   

            if (music_time>47.5)
                box[1][0]=1;
            if (music_time>47.7)
                box[2][0]=1; 
            if (music_time>48.0)
                box[3][0]=1;   

            if (music_time>50.5)
                box[3][2]=2;     
            if (music_time>51.5) 
                box[0][2]=2;     

            if (music_time>54.0)
                box[3][0]=2;       

            if (music_time>54.5)
                box[1][0]=2;  
            if (music_time>55.0) //asd
                box[2][0]=2;     

            if (music_time>56.5) //here
                box[0][1]=2;  
            if (music_time>57.0)
                box[3][1]=2;    
            if (music_time>57.6)
                box[1][2]=2;  
            if (music_time>58.0)
                box[2][2]=2;                                                                                                                          

            if (music_time>60.0)
            {
                for (auto i=0; i<4; i++)
                    for (auto j=0; j<3; j++)
                        box[i][j]=3;
            }

            if (music_time>61.0)
            {
                for (auto i=0; i<4; i++)
                    for (auto j=0; j<3; j++)
                        box[i][j]=2;
            }

            if (music_time>61.5 && music_time<64.50)
            {
                for (auto i=0; i<4; i++)
                    for (auto j=0; j<3; j++) {
                        box[i][j]=4;
                        rw2018.blit_to_buf(buf,10+i*80, 10+j*80,SCREEN_W,SCREEN_H);
                    }           
            } 

            if (music_time>64.00)
            {
                for (auto i=0; i<4; i++)
                    for (auto j=0; j<3; j++) {
                        box[i][j]=2;
                    }           
            }

            if (music_time>65.0)
            {
                for (auto i=0; i<4; i++)
                    for (auto j=0; j<3; j++) {
                        box[i][j]=0;
                    }           
            }

            if (music_time>66.5)
            {
                effect_twirledtunnel->render(timestamp, container60);
                for (auto i=0; i<4; i++)
                    for (auto j=0; j<3; j++) {
                        box[i][j]=4;
                        container60.blit_to_buf(buf,10+i*80, 10+j*80,SCREEN_W,SCREEN_H);
                    }           
            }
             


/////////////////////////// RENDERING ON STATES ///////////////////////
            //row 1
            if (box[0][0]==1)
			app->basic2d.draw_rect(
				buf, 
                0xff, 0xff, 0xff, 0xff,
                10, 10, 60, 60);
            if (box[0][0]==2)
            {
                effect_bluechiplights->render(timestamp, container60, music_time);
                container60.blit_to_buf(buf,10, 10,SCREEN_W,SCREEN_H);
            }

            if (box[1][0]==1)
            app->basic2d.draw_rect(
				buf, 
                0xff, 0xff, 0xff, 0xff,
                90, 10, 60, 60);
            if (box[1][0]==2)
            {
                effect_greenlighttunnel->render(timestamp, container60, music_time);
                container60.blit_to_buf(buf,90, 10,SCREEN_W,SCREEN_H);
            }    
            if (box[2][0]==1)
            app->basic2d.draw_rect(
				buf, 
                0xff, 0xff, 0xff, 0xff,
                170, 10, 60, 60);
            if (box[2][0]==2)
            {
                effect_lighting->render(timestamp, container60, music_time);
                container60.blit_to_buf(buf,170, 10,SCREEN_W,SCREEN_H);
            }                

            if (box[3][0]==1)
            app->basic2d.draw_rect(
				buf, 
                0xff, 0xff, 0xff, 0xff,
                250, 10, 60, 60);
            if (box[3][0]==2)
            {
                moth.blit_to_buf(buf,250, 10,SCREEN_W,SCREEN_H);
            }

            //row 2
            if (box[0][1]==1)
			app->basic2d.draw_rect(
				buf, 
                0xff, 0xff, 0xff, 0xff,
                10, 90, 60, 60);
            if (box[0][1]==2)
            {
                effect_ratunel->render(timestamp, container60, music_time);
                container60.blit_to_buf(buf,10, 90  ,SCREEN_W,SCREEN_H);
            }
            if (box[1][1]==1) {
            app->basic2d.draw_rect(
				buf, 
                0xff, 0xff, 0xff, 0xff,
                90, 90, 60, 60);
            }
            if (box[1][1]==2)
            {
                effect_blossomingflower->render(timestamp, container60, music_time);
                container60.blit_to_buf(buf,90,90,SCREEN_W,SCREEN_H);
            }
            if (box[2][1]==1)
            app->basic2d.draw_rect(
				buf, 
                0xff, 0xff, 0xff, 0xff,
                170, 90, 60, 60);
            if (box[2][1]==2)
                ant.blit_to_buf(buf,170,90,SCREEN_W,SCREEN_H);

            if (box[3][1]==1)
            app->basic2d.draw_rect(
				buf, 
                0xff, 0xff, 0xff, 0xff,
                250, 90, 60, 60);
            if (box[3][1]==2)
            {
                effect_twirledtunnel->render(timestamp, container60);
                container60.blit_to_buf(buf,250, 90,SCREEN_W,SCREEN_H);
            }

            //row 3
            if (box[0][2]==1)
			app->basic2d.draw_rect(
				buf, 
                0xff, 0xff, 0xff, 0xff,
                10, 170, 60, 60);
            if (box[0][2]==2)
                mosquito.blit_to_buf(buf,10, 170,SCREEN_W,SCREEN_H);

            if (box[1][2]==1)
            app->basic2d.draw_rect(
				buf, 
                0xff, 0xff, 0xff, 0xff,
                90, 170, 60, 60);
            if (box[1][2]==2)
            {
                effect_clouds->render(timestamp, container60, music_time);
                container60.blit_to_buf(buf,90, 170,SCREEN_W,SCREEN_H);
            }

            if (box[2][2]==1)
            app->basic2d.draw_rect(
				buf, 
                0xff, 0xff, 0xff, 0xff,
                170, 170, 60, 60);
            if (box[2][2]==2)
            {
                fly.blit_to_buf(buf,170, 170,SCREEN_W,SCREEN_H);
            }
            
            if (box[3][2]==1)
            app->basic2d.draw_rect(
				buf, 
                0xff, 0xff, 0xff, 0xff,
                250, 170, 60, 60);  
            if (box[3][2]==2)
            {
                effect_wobbler->render(timestamp, container60);
                container60.blit_to_buf(buf,250, 170,SCREEN_W,SCREEN_H);
            }              

        return 0;
    }



	virtual int render(int timestamp, ushine::core::texture& dest_buffer, float music_time)
	{
        float animation_time = (app->additive_progress-app->first_frame_tick)/2000;

        float w = dest_buffer.get_width();
        float h = dest_buffer.get_height();
        float png_w = demo_effect_04_tex_01.get_width();
        float png_h = demo_effect_04_tex_01.get_height();

        int* buf = (int*)dest_buffer.lock();
        int* texture = (int*)demo_effect_04_tex_01.lock();

        if (buf == nullptr)
            return -1;

        ushine::math::base_vector_2<float>
                    move1;

        move1.x = cos(animation_time*1)*0.004;
        move1.y = sin(animation_time*1.5)*0.004;

        for (int i=0; i<w; i++)
            for (int j=0; j<h; j++)
            {

                ushine::math::base_vector_2<float> p, uv;
                p.x = -1+2.0*i/w;//+move1.x;
                p.y = -1+2.0*j/h;//+move1.y;

                float a = atan2(p.y, p.x);
                float r = sqrt(p.dot(p))/2;

                ////inverted z effect
                uv.x =0.002+animation_time + cos(sin(0.04*animation_time)+a)/(r);
                uv.y =0.0025*animation_time + sin(cos(0.023*animation_time)+a)/(r);
                //uv.x=0.002+timestamp;
                //uv.y=0.0025*timestamp;


                /////normalizing the uv
                int uvx = abs((int)(uv.x*(png_w-1))%255);
                int uvy = abs((int)(uv.y*(png_h-1))%255);

                ushine::renderer::color_f src_col;

                int src_argb = texture[uvx+uvy*demo_effect_04_tex_01.get_width()];

                src_col.from_argb(src_argb);

                float s = 0.5 + 0.5*cos(2.5*a);
                s = ushine::math::smoothstep<float>(0.0,1.0,s);
                s = ushine::math::smoothstep<float>(0.0,1.0,s);
                s = ushine::math::smoothstep<float>(0.0,1.0,s);
                s = ushine::math::smoothstep<float>(0.0,1.0,s);

                float wobble = (0.5 + 0.5*s)*r;

                src_col.r*=wobble;
                src_col.g*=wobble;
                src_col.b*=wobble;

                src_col.a = 1.5f;

                int dst_argb = src_col.to_argb(true);

                buf[i+j*(int)w]=dst_argb; //src_argb;
                }

        demo_effect_04_tex_01.unlock();

        renderChildElements(buf,music_time,timestamp);

        dest_buffer.unlock();

        return 0;


	}

};