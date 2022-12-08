#include <iostream>
#include <random>
#include <math.h>

#include "../core/core.h"
#include "../math/math.h"
#include "../renderer/renderer.h"

#include <bass.h>

#define SCREEN_W 640 //320
#define SCREEN_H 400 //240


class HelloApp :
public ushine::core::application
{
public:

	ushine::core::audio_player player;
	ushine::core::audio_stream stream;


	bool musicOk = false;
	bool playingOk = false;

	virtual int on_init()
	{

		player.init(this);

		musicOk = player.load(stream, std::string("./music/zenial_revisq_earth_reprise.mp3"));

		CreateWnd(SCREEN_W, SCREEN_H, false,false);

		return 0;
	}

	virtual int on_close()
	{
		CloseWnd();

		if (playingOk)
			player.stop(stream);

		player.close();

		return 0;
	}


	virtual int on_render(float timestamp)
	{

		if (playingOk==false && musicOk)
		{
			player.play(stream);
			playingOk = true;
		} else {

			//std::cout << "Player Second: " << player.get_current_pos(stream) << std::endl;
		}
		return 0;
	}

	virtual int on_event()
	{
		return 0;
	}

private:

	std::string getResourcePath()
	{
		return std::string("./music/");
	}
};



int main(int argc, char** argv)
{
	HelloApp app;

	if (app.Init()!=0)
		return 0;

	app.Run();
	app.Close();

	return 0;
}
