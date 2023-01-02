# Oftenhide Demo Engine v1

First version of the Oftenhide Demo engine, initiated around 2015 as an attempt to:
- come back to the world of C++ (with C++11/14 features to be tested in, primarily the way memory is addressed, pimpl and few more goodies that you can spot in
- check if software rendering can be reintroduced on modern platforms that still can enable virtualized frame buffer access via texture rendering
- release a demo when the complete team is ready (and assembled in the first place), which I managed in 2018, for the Memory Remains demo that was released at Riverwash 2018

Then abandoned in unfinished state, which I'm publishing here. Lot of pre-mature experiments built into this code regarding 3D rasterization and effects. Only a fraction of its envisioned features was used in Memory Remains when me and Angelo realized that the quality of the code is so low, that it's better to rewrite the whole concept from the beginnign for another production.

We we did with Simulacrum (that co-won Xenium 2019 a year later) with Oftenhide Demo Engine v2.

## How to read it

Complete source code is available in the 'src' folder. All example effects and some other tests are placed in ./src/_effects and ./src/_tests folders. All such examples represent independent programs that ware visually testing some concepts derived from the engine. Names should suggest the visual outcome defined by the well known or popular demoscene effect I've tried to recreate.

Other folders in the 'src' represent various components of the engine itself like
- core : core functions to enable windowing system, full screen modes for both macOs and Windows as well as some basic logging and debugging helpers. It also includes handlers for png file formats via libpng
- demos: complete source code for 'Memory Remains' demo, released at Riverwash 2018 and available for download via Pouet.net: https://www.pouet.net/prod.php?which=77903
- io: advanced file io operations, early stage, not used too much in the demo/tests
- math: 2D/3D math functions as separate library component
- renderer: 2D/3D renderer that handles frame buffer operations, color operations, enpsulates effect across its lifecycle (load, release, render every frame defined by a timestamp), 3D projections code and triangle rasterizers (flat, goraud and perspective correct texture mapper, no subpixel correction though)
- tools: packager introduced after demo released to prepare final version that could consist all the resources being packed into a single .data file, assumed not completed after years of not looking at it at all

## How to compile it

This whole project includes manually managed makefiles for each core component to be launched in order to create whole stack. Me and Angelo had experimented with cmake in some development branch, but for various reasons we also had very quickly decided to abandon this version and rewrite the concept from the beginning, using V1 only as a learning point.

Whole project is more GCC/MINGW friendly than other compilers to be used to retrieve any executable, please do mind that.

## Prerequisites:

- libpng15: available in ./vendors folder for MacOS, on Windows it needs to be manually added to the stack
- libSDL2: available in ./vendors folder for MacOS, on Windows it needs to be manually added to the stack
- libBASS: available in ./vendors folder considering both MacOS and Windows consumptions (both versions of the binary dlls are placed there)

## Example compilations

- make -f makefile.corelibs all : creates libushinegfx.a static library that is refered by other components
- make -f makefile.effects all : once corelibs compiled, you can compile all the effects and the executables will be placed in ./resources folder to test against available content that is hardcoded in the effects'code

## Last words

This engine was designed to compile on MacOS first. Originally to support Intel platform, before publishing on github ensured it treats M1 (ARM) plaform as first citizen. Legacy Intel Macs and Windows machines may require some additional attention. Perhaps some future updates to better maintain code's compatibility with various systems will be added to this repo, but most likely you should fork it and play by yourself.
