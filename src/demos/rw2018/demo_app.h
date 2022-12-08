#include <iostream>
#include <SDL2/SDL.h>
#include <random>
#include <math.h>

#include "../../core/core.h"
#include "../../math/math.h"
#include "../../renderer/renderer.h"

#define SCREEN_W 320 //320
#define SCREEN_H 240 //240

#define RESOURCE_PATH "./res/"

class effect_introduction;
class effect_blossomingflower;
class effect_greenlighttunnel;
class effect_bluechiplights;
class effect_ratunel;
class effect_lighting;
class effect_infinitezoom;
class effect_clouds;
class effect_metaballs;
class effect_cubetunnel;

class effect_twirledtunnel;
class effect_wobbler;
class effect_plantunel_vomitman;
class effect_oilywater;
class effect_pictureshow;

class effect_dancefloor;
class effect_triangleparticles;
class effect_credits;

class effect_fade_picture;
class effect_fade_triangles;

#define MBLUR_BUFFERS 4

class demo_app :
public ushine::core::application
{
public:
	ushine::core::audio_player player;
	ushine::core::audio_stream stream;

	double music_timing_second;

	ushine::core::texture back_buffer;



	ushine::core::texture_loader_png png_loader;
	ushine::renderer::basic2d_context basic2d;

	effect_introduction* effect_introduction;
	
	effect_ratunel* effect_ratunel;

	effect_greenlighttunnel* effect_greenlighttunnel;
	effect_lighting* effect_lighting;
	effect_infinitezoom* effect_infinitezoom;
	effect_clouds* effect_clouds;
	effect_metaballs* effect_metaballs;
	effect_cubetunnel* effect_cubetunnel;
	
	effect_twirledtunnel* effect_twirledtunnel;
	effect_wobbler* effect_wobbler;
	effect_plantunel_vomitman* effect_plantunel_vomitman;
	effect_oilywater* effect_oilywater;
	effect_pictureshow* effect_pictureshow;

	effect_dancefloor* effect_dancefloor;
	effect_triangleparticles* effect_triangleparticles;
	effect_credits* effect_credits;

	effect_fade_picture* effect_fade_picture;
	effect_fade_triangles* effect_fade_triangles;


	bool musicOk = false, playingOk=false;

	virtual int on_init();
	virtual int on_close();

	int mblur_idx=0; int mblur_clear=0;
	float mblur_last_sec=0;
	ushine::core::texture mblur_buffers[MBLUR_BUFFERS];
	int merge_buffers(int buffers_count,float bloom);
	int clear_buffers(int buffers_count);

	float first_frame_tick=0;
	float additive_progress=0;


	int effect_counter = 0;


	virtual int on_render(float timestamp);
	virtual int on_event();

	std::string getResourcePath();
};


