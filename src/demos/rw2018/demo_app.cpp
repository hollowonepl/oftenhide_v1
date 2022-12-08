#include <iostream>
#include <SDL2/SDL.h>
#include <random>
#include <math.h>

#include "../../core/core.h"
#include "../../math/math.h"
#include "../../renderer/renderer.h"

#include "../../renderer/basic2d_routines.h"

#define SCREEN_W 320 //320
#define SCREEN_H 240 //240

#define RESOURCE_PATH "./res/"

#include "demo_app.h"

#include "effects/effect_blossomingflower.hpp"
#include "effects/effect_wobbler.hpp"
#include "effects/effect_bluechiplights.hpp"
#include "effects/effect_twirledtunnel.hpp"

#include "effects/effect_greenlighttunnel.hpp"
#include "effects/effect_ratunel.hpp"
#include "effects/effect_lighting.hpp"
#include "effects/effect_infinitezoom.hpp"
#include "effects/effect_clouds.hpp"
#include "effects/effect_metaballs.hpp"
#include "effects/effect_cubetunnel.hpp"

#include "effects/effect_introduction.hpp"

#include "effects/effect_dancefloor.hpp"
#include "effects/effect_triangleparticles.hpp"

#include "effects/effect_plantunel_vomitman.hpp"
#include "effects/effect_oilywater.hpp"
#include "effects/effect_pictureshow.hpp"

#include "effects/effect_credits.hpp"

#include "effects/effect_fade_picture.hpp"
#include "effects/effect_fade_triangles.hpp"

#ifdef __APPLE__
#ifndef MAKEBUILD
#define RESOURCEPATH_IMPLEMENTED 1
//this is macos getResourcePath if compiled as an app bundle inside XCode only
#include <CoreFoundation/CoreFoundation.h>
std::string demo_app::getResourcePath()
{
    //this should work if it's a part of a bundle
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef url = CFBundleCopyBundleURL(mainBundle);
    UInt8 bundlePath[PATH_MAX];
    if (CFURLGetFileSystemRepresentation(url, true, bundlePath, sizeof(bundlePath)))
    {
        if (chdir((const char*)bundlePath) == 0)
        {
            return std::string("Contents/Resources/");;
        }
    }
    return std::string("./res/"); //this should work from a makefile build
}
#endif
#endif

#ifndef RESOURCEPATH_IMPLEMENTED
//this is win/macos/linux getResourcePath if compiled from cmake/makefile scripts
std::string demo_app::getResourcePath()
{
    return std::string(RESOURCE_PATH);
}
#endif

int demo_app::on_init()
{

    CreateWnd(SCREEN_W, SCREEN_H, false,true);
    basic2d.init(SCREEN_W, SCREEN_H);
    png_loader.init(this);


    bool textLoaded = back_buffer.create(this, SCREEN_W,SCREEN_H);

    for (auto i=0; i<MBLUR_BUFFERS; i++)
            textLoaded = textLoaded & mblur_buffers[i].create(this,SCREEN_W, SCREEN_H);

    effect_introduction= new class effect_introduction(this);
    effect_introduction->init();

    effect_ratunel = new class effect_ratunel(this);
    effect_ratunel->init();

    effect_lighting = new class effect_lighting(this);
    effect_lighting->init();

    effect_clouds = new class effect_clouds(this);
    effect_clouds->init(true);

    effect_metaballs= new class effect_metaballs(this);
    effect_metaballs->init(true);

    effect_cubetunnel= new class effect_cubetunnel(this);
    effect_cubetunnel->init();

    effect_infinitezoom = new class effect_infinitezoom(this);
    effect_infinitezoom->init();

    effect_greenlighttunnel = new class effect_greenlighttunnel(this);
    effect_greenlighttunnel->init();

    effect_twirledtunnel = new class effect_twirledtunnel(this);
    effect_twirledtunnel->init();

    effect_wobbler = new class effect_wobbler(this);
    effect_wobbler->init();

    effect_plantunel_vomitman = new class effect_plantunel_vomitman(this);
    effect_plantunel_vomitman->init();

    effect_oilywater = new class effect_oilywater(this);
    effect_oilywater->init();

    effect_pictureshow = new class effect_pictureshow(this);
    effect_pictureshow->init();

    effect_dancefloor = new class effect_dancefloor(this);
    effect_dancefloor->init();

    effect_triangleparticles = new class effect_triangleparticles(this);
    effect_triangleparticles->init();

    effect_credits = new class effect_credits(this);
    effect_credits->init();

    effect_fade_picture = new class effect_fade_picture (this);
    effect_fade_picture->init();

    effect_fade_triangles= new class effect_fade_triangles(this);
    effect_fade_triangles->init();

    if (!textLoaded)
    {
        get_logger()->LogLine(std::string("Texture load failed!"));
        CloseWnd();
        return -1;
    }

    musicOk = player.init(this);
    if (musicOk)
        musicOk = player.load(stream, (getResourcePath()+"Argasek - Silicon Venture.mp3").c_str());
    else
    {
        get_logger()->LogLine(std::string("BASS Music player initialization failed!"));
        return -1;
    }

    return 0;
}

int demo_app::on_close()
{
    if (playingOk)
        player.stop(stream);

    player.close();

    //delete effect_blossomingflower;
    //above is just to remind that we should delete all the effects instances at the end to clean up

    back_buffer.release();

    CloseWnd();
    return 0;
}

int demo_app::merge_buffers(int buffers_count, float bloom)
{

    mblur_clear=1;

    int* mblur_buf[5];

    for (auto i=0; i<buffers_count; i++)
        mblur_buf[i] = (int*)mblur_buffers[i].lock();

    int* bbuf = (int*)back_buffer.lock();

    for (auto p=0; p<SCREEN_W*SCREEN_H; p++)
    {
        ushine::renderer::color_f source_pixel[5];
        ushine::renderer::color_f final_pixel;
        for (auto i=0; i<buffers_count; i++)
        {
            source_pixel[i].from_argb(mblur_buf[i][p]);
            final_pixel.r+=source_pixel[i].r;
            final_pixel.g+=source_pixel[i].g;
            final_pixel.b+=source_pixel[i].b;
        }


        final_pixel.r/=buffers_count;
        final_pixel.g/=buffers_count;
        final_pixel.b/=buffers_count;

        final_pixel.r*=bloom;
        final_pixel.g*=bloom;
        final_pixel.b*=bloom;        
        final_pixel.a=1.0;

        bbuf[p] = final_pixel.to_argb();

    }

    for (auto i=0; i<buffers_count; i++)
        mblur_buffers[i].unlock();

    back_buffer.unlock(); 

    return 0;
}

int demo_app::clear_buffers(int buffers_count)
{
    if (mblur_clear==1)
    {
        mblur_clear=0;
        int* mblur_buf;

        for (auto i=0; i<buffers_count; i++)
        {
            mblur_buf = (int*)mblur_buffers[i].lock();
            basic2d.clear(mblur_buf, 255, 0, 0, 0,
                            mblur_buffers[i].get_width(), 
                            mblur_buffers[i].get_height());
            mblur_buffers[i].unlock();
        }
    }

    return 0;
}


int demo_app::on_render(float timestamp)
{
    if (musicOk && playingOk==false && timestamp>40)
    {
        player.play(stream);
        playingOk=true;
        first_frame_tick = timestamp;
        additive_progress = timestamp;
    } else
    {
        music_timing_second = player.get_current_pos(stream);
    }

    auto playback_pos = player.get_current_pos(stream);

    /******
     * 
     *  Intro : GROUP
     * 
     * *****/
    if (music_timing_second<7.0)
    {
        effect_lighting->render(timestamp, mblur_buffers[mblur_idx],music_timing_second);

        if (music_timing_second>6.0)
        {
            effect_fade_picture->render(timestamp,mblur_buffers[mblur_idx]);
        } else 
        {
            effect_fade_picture->reset(0,1000);
        }

        if (music_timing_second-mblur_last_sec>.05)
        {
            mblur_idx++;
            mblur_last_sec=music_timing_second;
        }
        if (mblur_idx>=MBLUR_BUFFERS) mblur_idx=0;

        merge_buffers(MBLUR_BUFFERS,1.0);
        
    }
    /******
     * 
     *  Intro : DEMO TITLE
     * 
     * *****/
    if (music_timing_second>=7.0 && music_timing_second<13)
    {
        if (effect_counter==0)
        {
            effect_counter=1;
            effect_fade_picture->reset(1,1000);
            clear_buffers(MBLUR_BUFFERS);
        }

        effect_ratunel->render(timestamp, mblur_buffers[mblur_idx],music_timing_second);

        if (music_timing_second<=8.0 && effect_counter==1)
        {
            effect_fade_picture->render(timestamp,mblur_buffers[mblur_idx]);
        } 
        
        
        if (music_timing_second>=10.5)
        {
            if (effect_counter==2)
                effect_fade_picture->render(timestamp,mblur_buffers[mblur_idx]);
            else {
                effect_counter=2;
                effect_fade_picture->reset(0,1800);
            }
        }

        if (music_timing_second-mblur_last_sec>.05)
        {
            mblur_idx++;
            mblur_last_sec=music_timing_second;
        }
        if (mblur_idx>=MBLUR_BUFFERS) mblur_idx=0;
        merge_buffers(MBLUR_BUFFERS,1.0);
    }
    /*
        CODE CREDITS
    */
    if (music_timing_second>=13 && music_timing_second<20.0)
    {
        if (effect_counter==2)
        {
            //std::cout << "hit 1" << std::endl;
            effect_counter=3;
            clear_buffers(MBLUR_BUFFERS);
            effect_fade_picture->reset(1 ,1000,1);
        }

        effect_greenlighttunnel->render(timestamp,mblur_buffers[mblur_idx],music_timing_second);

        if (music_timing_second>=13.0)
        {
            if (effect_counter==3)
            {
                effect_fade_picture->render(timestamp,mblur_buffers[mblur_idx]);
            }
        }
        if (music_timing_second>=19.0)
        {
            if (effect_counter==4)
            {
                effect_fade_picture->render(timestamp,mblur_buffers[mblur_idx]);
            } else {
                effect_counter=4;
                effect_fade_picture->reset(0 ,800,1);
            }
        }

        if (music_timing_second-mblur_last_sec>.05)
        {
            mblur_idx++;
            mblur_last_sec=music_timing_second;
        }
        if (mblur_idx>=MBLUR_BUFFERS) mblur_idx=0;

        merge_buffers(MBLUR_BUFFERS,1.3);
    }
    /*
        ART+AUDIO CREDITS
    */
    if (music_timing_second>=20 && music_timing_second<27.0)
    {
        clear_buffers(MBLUR_BUFFERS);
        effect_dancefloor->render(timestamp, back_buffer, music_timing_second);
    }


    if (music_timing_second>=27.0 && music_timing_second<68.0)
    {
        effect_introduction->render(timestamp,mblur_buffers[mblur_idx], music_timing_second);
        if (music_timing_second-mblur_last_sec>.05)
        {
            mblur_idx++;
            mblur_last_sec=music_timing_second;
        }
        if (mblur_idx>=MBLUR_BUFFERS) mblur_idx=0;

        merge_buffers(MBLUR_BUFFERS,1.0);
    }   

///// dynamic section
    if (music_timing_second>=68 && music_timing_second<=96)
    {
        if (music_timing_second>=68 && music_timing_second<75)
        {
            effect_twirledtunnel->render(timestamp,mblur_buffers[mblur_idx]);
            if (music_timing_second-mblur_last_sec>.05)
            {
                mblur_idx++;
                mblur_last_sec=music_timing_second;
            }
            if (mblur_idx>=MBLUR_BUFFERS) mblur_idx=0;

            merge_buffers(MBLUR_BUFFERS,1.0);

        }
        if (music_timing_second>=75 && music_timing_second<82)
        {
            effect_metaballs->render(timestamp, mblur_buffers[mblur_idx], music_timing_second);
            if (music_timing_second-mblur_last_sec>.05)
            {
                mblur_idx++;
                mblur_last_sec=music_timing_second;
            }
            if (mblur_idx>=MBLUR_BUFFERS) mblur_idx=0;

            merge_buffers(MBLUR_BUFFERS,0.8);
        }
        if (music_timing_second>=82 && music_timing_second<89)
        {
            effect_wobbler->render(timestamp,mblur_buffers[mblur_idx]);
            if (music_timing_second-mblur_last_sec>.05)
            {
                mblur_idx++;
                mblur_last_sec=music_timing_second;
            }
            if (mblur_idx>=MBLUR_BUFFERS) mblur_idx=0;

            merge_buffers(MBLUR_BUFFERS,1.0);

        }

        if (music_timing_second>=89 && music_timing_second<96) {
            clear_buffers(MBLUR_BUFFERS);
            effect_cubetunnel->render(timestamp, back_buffer);
        }



    }
///// dynamic section:end


    if (music_timing_second>=96.0 && music_timing_second<124.0)
    {
        if (effect_counter==4)
        {
            effect_counter=5;
            effect_fade_picture->reset(1 ,500,0);
        }

        effect_plantunel_vomitman->render(timestamp,back_buffer);

        if (music_timing_second>=96.0)
        {
            if (effect_counter==5)
            {
                effect_fade_picture->render(timestamp,back_buffer);
            }
        }
        
        if (music_timing_second>=122.0)
        {
            if (effect_counter==6)
            {
                effect_fade_picture->render(timestamp,back_buffer);
            } else 
            {
                effect_counter=6;
                effect_fade_picture->reset(0,1000,0);
            }
        }

    }

    if (music_timing_second>=124 && music_timing_second<157.0)
    {
        effect_oilywater->render(timestamp,back_buffer, music_timing_second);

        if (music_timing_second>=155.0)
        {
            if (effect_counter==7)
            {
                effect_fade_picture->render(timestamp,back_buffer);
            } else 
            {
                effect_counter=7;
                effect_fade_picture->reset(0,2000,0);
                clear_buffers(MBLUR_BUFFERS);
            }
        }
    }
    if (music_timing_second>=157.0)
    {
        effect_triangleparticles->render(timestamp,mblur_buffers[mblur_idx]);
        effect_credits->render(timestamp,mblur_buffers[mblur_idx]);

        if (timestamp-mblur_last_sec>20)
        {
            mblur_idx++;
            mblur_last_sec=timestamp;
        }
        if (mblur_idx>=MBLUR_BUFFERS) mblur_idx=0;

        merge_buffers(MBLUR_BUFFERS,1.2);   

        effect_credits->render(timestamp,back_buffer);
    }

    if ((timestamp-first_frame_tick)>=265000.0f)
    {
        exit(0);
    }

    back_buffer.blit_to_screen(this);

    additive_progress = timestamp;

    return 0;
}  

int demo_app::on_event()
{
    return 0;
}







