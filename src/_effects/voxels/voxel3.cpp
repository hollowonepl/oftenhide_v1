// https://www.shadertoy.com/view/MdVSDh
#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include <memory>
#include <iterator>

#include "core/core.h"
#include "math/math.h"
#include "renderer/renderer.h"
#include "renderer/basic2d_routines.h"

#define SCREEN_W 320
#define SCREEN_H 240

#define RESOURCE_PATH "./textures/"

namespace um = ushine::math;
namespace uc = ushine::core;
namespace ur = ushine::renderer;

um::vec2 path(float z){ 
    //return vec2(0); // Straight.
    float a = std::sin(z * 0.11);
    float b = std::cos(z * 0.14);
    return um::vec2(a*4. -b*1.5, b*1.7 + a*1.5); 
    //return vec2(a*4. -b*1.5, 0.); // Just X.
    //return vec2(0, b*1.7 + a*1.5); // Just Y.
}
um::mat2 rot2(float a) {
  um::vec2 v = um::sin(um::vec2(1.570796, 0) + a);	
  return um::mat2(v.x, v.y, -v.y, v.x);
}

float map(um::vec3 p){
     const auto pathZ = path(p.z);
     p.x -= pathZ.x;
     p.y -= pathZ.y;
    
     // Standard tunnel. Comment out the above first.
     float n = 5.0f - um::length(p.xy()*um::vec2(1.0f, 0.8f));
    
     // Square tunnel. Almost redundant in a voxel renderer. :)
     //n = 4. - max(abs(p.x), abs(p.y)); 
     
     // Tunnel with a floor.
     return std::min(p.y + 3.0f, n); //n = min(-abs(p.y) + 3., n);
 
}

um::vec3 voxelTrace(um::vec3 ro, um::vec3 rd, um::vec3& mask){
    
  auto p = um::floor(ro) + 0.5f;
  auto dRd = 1.0f/um::abs(rd); // 1./max(abs(rd), vec3(.0001));
	rd = um::sign(rd);
  auto side = dRd*(rd * (p - ro) + 0.5f);
    
  mask = um::vec3(0);
	
	for (int i = 0; i < 30; i++) {
    if (map(p)<0.0f) break;
    mask = um::step(side, um::vec3(side.y, side.z, side.x))*(1.0f-um::step(um::vec3(side.z, side.x, side.y), side));
		side += mask*dRd;
		p += mask * rd;
	}
    
    return p;    
}


float voxShadow(um::vec3 ro, um::vec3 rd, float end){

    float shade = 1.0f;
    auto p = um::floor(ro) + 0.5f;

	auto dRd = 1.0f/abs(rd);//1./max(abs(rd), vec3(.0001));
	rd = um::sign(rd);
    auto side = dRd*(rd * (p - ro) + 0.5f);
    
    auto mask = um::vec3(0.0f);
    
    float d = 1.0f;
	
	for (int i = 0; i < 16; i++) {
		
        d = map(p);
        
        if (d<0. || um::length(p-ro)>end) break;
        
        mask = um::step(side, um::vec3(side.y, side.z, side.x))*(1.0f-um::step(um::vec3(side.z, side.x, side.y), side));
		  side += mask*dRd;
		  p += mask * rd;                
	}

    // Shadow value. If in shadow, return a dark value.
    return shade = um::step(0.0f, d)*0.7f + 0.3f;
    
}

um::vec4 voxelAO(um::vec3 p, um::vec3 d1, um::vec3 d2) {
   
    // Take the four side and corner readings... at the correct positions...
    // That's the annoying bit that I'm glad others have worked out. :)
	auto side = um::vec4(map(p + d1), map(p + d2), map(p - d1), map(p - d2));
	auto corner = um::vec4(map(p + d1 + d2), map(p - d1 + d2), map(p - d1 - d2), map(p + d1 - d2));
	
    // Quantize them. It's either occluded, or it's not, so to speak.
    side = um::step(side, um::vec4(0.0f));
    corner = um::step(corner, um::vec4(0.0f));
    
    // Use the side and corner values to produce a more honed in value... kind of.
    return 1.0f - (side + um::vec4(side.y, side.z, side.w, side.x) + um::max(corner, side*um::vec4(side.y, side.z, side.w, side.x)))/3.0f;    
	
}

float calcVoxAO(um::vec3 vp, um::vec3 sp, um::vec3 rd, um::vec3 mask) {
    
	um::vec4 vAO = voxelAO(vp - um::sign(rd)*mask, um::vec3(mask.z, mask.x, mask.y), um::vec3(mask.y, mask.z, mask.x));
    
    sp = um::fract(sp);
    auto uv = um::vec2(sp.y, sp.z)*mask.x + um::vec2(sp.z, sp.x)*mask.y + um::vec2(sp.x, sp.y)*mask.z;
    return um::mix(um::mix(vAO.z, vAO.w, uv.x), um::mix(vAO.y, vAO.x, uv.x), uv.y);
}

um::vec3 texture(int* tex, float tex_w, float tex_h, um::vec2 p) {
  size_t x = (size_t)(p.x * tex_w) % (size_t)tex_w;
  size_t y = (size_t)(p.y * tex_h) % (size_t)tex_h;
  auto c = ur::color_f(tex[x+y*(size_t)tex_w]);
  return um::vec3(c.r, c.g, c.b);
}

um::vec3 tex3D(int* tex, float tex_w, float tex_h, um::vec3 p, um::vec3 n ){
  
    n = um::max(um::abs(n), 0.001f);//n = max((abs(n) - 0.2)*7., 0.001); //  etc.
    n /= (n.x + n.y + n.z ); 
	  p = (texture(tex, tex_w, tex_h, p.yz())*n.x + texture(tex, tex_w, tex_h, p.zx())*n.y + texture(tex, tex_w, tex_h, p.xy())*n.z);
    return p*p;
}

class HelloApp : public uc::application {
public:
	uc::texture backbuffer;
	uc::texture iChannel0;
	ur::basic2d_context basic2d;
  uc::texture_loader_png png_loader;


	virtual int on_init() {
		CreateWnd(SCREEN_W, SCREEN_H, true, true);
		basic2d.init(SCREEN_W, SCREEN_H);	

		auto loaded = backbuffer.create(this, SCREEN_W, SCREEN_H);
		png_loader.init(this);

		std::string imagePath = getResourcePath() + "texture_brick.png";
		loaded = loaded & png_loader.load(iChannel0, imagePath);

		if (!loaded)
		{
			get_logger()->LogLine(std::string("Texture load failed!"));
			CloseWnd();
			return -1;
		}

		return 0;
	}

	virtual int on_close() {
		backbuffer.release();				
		CloseWnd();
		return 0;
	}


  float first_tick = -1;

	virtual int effect_test(float timestamp) {

    if (first_tick<0) first_tick = timestamp;

    float iTime = (timestamp -first_tick)/ 700.0f;
		float w = backbuffer.get_width();
		float h = backbuffer.get_height();

		int* buf = (int*)backbuffer.lock();
    int* iChannel0_buffer = (int*)iChannel0.lock();
    float iChannel0_w = iChannel0.get_width();
    float iChannel0_h = iChannel0.get_height();

		basic2d.clear(buf, 0, 0, 0, 0,w,h);

    auto iResolution = um::vec2(w, h);
    for (int y=0; y<h; y++) {
      for (int x=0; x<w; x++) {
        auto fragCoord = um::vec2(x, y);

      	auto uv = (fragCoord - iResolution*0.5f)/iResolution.y;
	
        auto camPos = um::vec3(0.0f, 0.5f, iTime*8.0f); 
        auto lookAt = camPos + um::vec3(0.0f, 0.0f, 0.25f);  
       	auto lightPos = camPos + um::vec3(0.0f, 2.5f, 8.0f);// Put it a bit in front of the camera.

        const auto lookAtPath = path(lookAt.z);
        const auto camPosPath = path(camPos.z);
        const auto lightPosPath = path(lightPos.z);
	      lookAt.x += lookAtPath.x;
        lookAt.y += lookAtPath.y;
	      camPos.x += camPosPath.x;
        camPos.y += camPosPath.y;
	      lightPos.x += lightPosPath.x;
        lightPos.y += lightPosPath.y;

        const float PI = 3.14159265f;
        const size_t FAR = 33;
        float FOV = PI/2.; // FOV - Field of view.
        auto forward = um::normalize(lookAt-camPos);
        auto right = um::normalize(um::vec3(forward.z, 0., -forward.x )); 
        auto up = um::cross(forward, right);

        auto rd = um::normalize(forward + FOV*uv.x*right + FOV*uv.y*up);
	      rd.xy(rot2(path(lookAt.z).x/24. )*rd.xy());
        um::vec3 mask;
	      um::vec3 vPos=voxelTrace(camPos, rd, mask);
	      auto tCube = (vPos-camPos - 0.5f*um::sign(rd))/rd;
        float t = std::max(std::max(tCube.x, tCube.y), tCube.z);
        auto sceneCol = um::vec3(0.0f);

        if(t<FAR){
            auto sp = camPos + rd*t;
            auto sn = -(mask * um::sign( rd ));
            auto snNoBump = sn;

            const float tSize0 = 1.0f/4.0f;
              //TODO:sn = doBumpMap(iChannel0, sp*tSize0, sn, 0.02);
              //TODO:sn = doBumpMap(sp, sn, .15);
              
            float ao = calcVoxAO(vPos, sp, rd, mask) ;//calculateAO(sp, sn);//*.75 + .25;
            auto ld = lightPos-sp;
            float lDist = std::max(um::length(ld), 0.001f);
            ld /= lDist;
            
            float atten = 1.0f/(1.0f + lDist*0.2f + lDist*0.1f); // + distlpsp*distlpsp*0.025
            float ambience = 0.5;
            float diff = std::max( um::dot(sn, ld), 0.0f);
          
            float spec = 0.3f; //TODO:pow(max( dot( reflect(-ld, sn), -rd ), 0.0 ), 32.);
            um::vec3 texCol = um::vec3(1.0f, 0.6f, 0.4f) + um::step(std::abs(snNoBump.y), 0.5f)*um::vec3(0.0f, 0.4f, 0.6f);
            texCol *= tex3D(iChannel0_buffer, iChannel0_w, iChannel0_h, sp*tSize0, sn);
            float shading = 1.0f;//voxShadow(sp + snNoBump*0.01f, ld, lDist);
            sceneCol = texCol*(diff + ambience) + um::vec3(0.7f, 0.9f, 1.0f)*spec;// + vec3(.5, .8, 1)*spec2;
            sceneCol *= atten*shading*ao;
        }
        sceneCol = um::mix(sceneCol, um::vec3(0.08f, 0.16f, 0.34f), um::smoothstep(0.0f, 0.95f, t/FAR)); // exp(-.002*t*t), etc.
        auto color = um::sqrt(um::clamp(sceneCol, 0.0f, 1.0f));
        *buf++ = ur::color_f(1.0f, color.x, color.y, color.z).to_argb();
      }
    }

    iChannel0.unlock();
		backbuffer.unlock();
		backbuffer.blit_to_screen(this);	
		return 0;	
	}	


	virtual int on_render(float timestamp) {	
		effect_test(timestamp);
		return 0;
	}

	virtual int on_event() { return 0; }

private:
	std::string getResourcePath() { return std::string(RESOURCE_PATH); }
};



int main(int argc, char** argv) {
	HelloApp app;

	if (app.Init() != 0)
		return 0;

	app.Run();
	app.Close();

	return 0;
}