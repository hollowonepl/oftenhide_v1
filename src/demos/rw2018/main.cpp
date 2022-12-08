#include <iostream>
#include <SDL2/SDL.h>
#include <random>
#include <math.h>

#include "../../core/core.h"
#include "../../math/math.h"
#include "../../renderer/renderer.h"

#include "demo_app.h"

int main(int argc, char** argv)
{
	demo_app app;

	if (app.Init()!=0)
		return 0;

	app.Run();
	app.Close();

	return 0;
}
