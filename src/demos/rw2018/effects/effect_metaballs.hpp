

namespace um = ushine::math;
namespace uc = ushine::core;
namespace ur = ushine::renderer;

class effect_metaballs
{
private:
    demo_app* app;

    #define object_count 4
    const size_t occlusion_quality = 4;
    const float offset1 = 4.2f;
    const float offset2 = 4.6f;
    const float render_steps = 15.0f;
    const um::vec3 light_color = um::vec3(0.3f, 0.3f, 0.3f);
    const um::vec3 light_direction = um::normalize(um::vec3(0.2f, -1.0f, -0.2f));
    const um::vec3 object_color = um::vec3(0.50f, 0.492f, 0.206f);
    const um::vec3 light = light_direction;
    const float light_speed_modifier = 2.0f;

um::vec3 rotate_y(um::vec3 v, float angle)
{
	float ca = cos(angle); float sa = sin(angle);
	return v*um::mat3(
		+ca, +.0, -sa,
		+.0,+1.0, +.0,
		+sa, +.0, +ca);
}

um::vec3 rotate_x(um::vec3 v, float angle)
{
	float ca = cos(angle); float sa = sin(angle);
	return v*um::mat3(
		+1.0, +.0, +.0,
		+.0, +ca, -sa,
		+.0, +sa, +ca);
}

um::vec3 bpos[object_count];

float dist(um::vec3 p)//distance function
{
	float d=1024.0f;
	float nd;
	for (int i=0 ;i<object_count; i++)
	{
		um::vec3 np = p+bpos[i];
		float shape0 = um::max3(std::abs(np.x), std::abs(np.y), std::abs(np.z))-1.0f;
		float shape1 = um::length(np)-1.0f;
		nd = shape0+(shape1-shape0)*1.0f;
		d = um::mix(d,nd,um::smoothstep(-0.10f,+0.10f,d-nd));
	}
	return d;
}

um::vec3 cc(um::vec3 color, float factor, float factor2) //a wierd color modifier
{
	float w = color.x+color.y+color.z;
	return um::mix(color,um::vec3(w)*factor,w*factor2);
}

float hash(float x)
{
	return um::fract(sin(x*0.0127863f)*17143.321f);
}

float hash(um::vec2 x)
{
	return um::fract(std::cos(dot(x,um::vec2(2.31f,53.21f))*124.123f)*412.0f); 
}

um::vec3 normal(um::vec3 p,float e) //returns the normal, uses the distance function
{
	float d=dist(p);
	return um::normalize(um::vec3(dist(p+um::vec3(e,0.0f,0.0f))-d,dist(p+um::vec3(0.0f,e,0.0f))-d,dist(p+um::vec3(0.0f,0.0f,e))-d));
}

float occlusion(um::vec3 p, um::vec3 d)//returns how much a point is visible from a given direction
{
	float occ = 1.0f;
	p=p+d;
	for (int i=0; i<occlusion_quality; i++)
	{
		float dd = dist(p);
		p+=d*dd;
		occ = std::min(occ,dd);
	}
	return std::max(0.0f,occ);
}
float mod(float x, float y) {
  return x - y * std::floor(x/y);
}
float hashmix(float x0, float x1, float interp)
{
	x0 = hash(x0);
	x1 = hash(x1);
	//#ifdef noise_use_smoothstep
	interp = um::smoothstep(0.0f,1.0f,interp);
	//#endif
	return um::mix(x0,x1,interp);
}

float noise(float p) // 1D noise
{
	float pm = mod(p,1.0f);
	float pd = p-pm;
	return hashmix(pd,pd+1.0,pm);
}

um::vec3 background(um::vec3 d, float iTime)//render background
{
	float t=iTime*0.5f*light_speed_modifier;
	float qq = um::dot(d,light)*0.5f+0.5f;
	float bgl = qq;
	float q = (bgl+noise(bgl*6.0f+t)*.85+noise(bgl*12.0f+t)*0.85f);
	q+= std::pow(qq,32.0f)*2.0f;
	um::vec3 sky = um::vec3(0.071f, 0.275f, 0.185f)*q;
	return sky;
}

um::vec3 object_material(um::vec3 p, um::vec3 d, float iTime)
{
	um::vec3 color = um::normalize(object_color*light_color);
	um::vec3 n = normal(p,0.1);
	um::vec3 r = um::reflect(d,n);	
	
	float reflectance = um::dot(d,r)*.5+.5; reflectance=std::pow(reflectance,2.0f);
	float diffuse = um::dot(light,n)*.5+.5; diffuse = std::max(0.0f,diffuse);
	
		float oa = occlusion(p,n)*.4+.6;
		float od = occlusion(p,light)*.95+.05;
		float os = occlusion(p,r)*.95+.05;
    
	
		color = 
		color*oa*0.2f + //ambient
		color*diffuse*od*0.7f + //diffuse
		background(r, iTime)*os*reflectance*0.7f; //reflection
		
	return color;
}

public:
    effect_metaballs(demo_app* _app)
    {
        app = _app;
    }

    virtual int init(float _nologo=false)
    {
        bool textLoaded = true;

        if (!textLoaded)
            return -1;

        return 0;
    }


	virtual int render(int timestamp,ushine::core::texture& dest_buffer, float music_time)
	{
        //app->additive_progress = timestamp;

        float animation_time = (app->additive_progress-app->first_frame_tick) / 750;
        float iTime = animation_time+10000;
        float t=iTime;

        float w = dest_buffer.get_width();
        float h = dest_buffer.get_height();

        int* buf = (int*)dest_buffer.lock();


    /**effect start**/
    auto iResolution = um::vec2(w, h);
    um::vec3 mouse = um::vec3(2.0f, 3.0f, 0.5f);
    um::vec3 pp = um::vec3(0.0f,0.0f,-6.2f + std::abs(std::sin(iTime/2.5f))*3);
    pp = rotate_x(pp,mouse.y*9.0f+offset1);
    pp = rotate_y(pp,mouse.x*9.0f+offset2);
            
    for (int i=0; i<object_count; i++) //position for each metaball
    {
      bpos[i] = 1.3f*um::vec3(
        sin(t*float(i)*0.1f),
        sin(t*float(i)*0.01f),
        sin(t*float(i)*-0.2f));
    }
    auto ires =iResolution.x/iResolution.y;
    for (int y=0; y<h; y++) {
      for (int x=0; x<w; x++) {
        auto fragCoord = um::vec2(x, y);

        um::vec2 uv = fragCoord / iResolution - 0.5f;
        uv.x *= ires; //fix aspect ratio
        
        //setup the camera
        um::vec3 p =pp;
        um::vec3 d = um::vec3(uv.x, uv.y, 1.0f);
        d.z -= um::length(d)*0.3f; //lens distort
        d = um::normalize(d);
        d = rotate_x(d,mouse.y*9.0f+offset1);
        d = rotate_y(d,mouse.x*9.0f+offset2);
        
        //and action!
        float dd;
        um::vec3 color;
        for (int i=0; i<render_steps; i++) //raymarch
        {
          dd = dist(p);
          p+=d*dd*0.7f;
          if (dd<0.04f || dd>5.0f) break;
        }
        
        if (dd<5.0002f) //close enough or edge
          color = object_material(p,d, iTime) * 1.1f ;
            //color = vec3(0, 0, 255);
        else
          color = background(d, iTime);
        
        const float vignette_radius = 0.95f;
        const float vignette_softness = 0.45f;
        float vignette = um::smoothstep(vignette_radius, vignette_radius-vignette_softness, um::length(uv));

        //post procesing
        color *= 0.85f;
        color = mix(color,color*color,0.3f);
        color -= hash(color.xy()+uv)*0.015f;
        //color -= length(uv)*0.1f;
        color = cc(color,0.5f,0.6f);
        color *= vignette;
        *buf++ = ur::color_f(dd < 5.0002f ? 1.0f : 0.5f, color.x, color.y, color.z).to_argb();
      }
    }
    /**effect end**/

        //logo1.blit_to_buf(buf, 0,0, w,h, 255);
        //if (music_time>1.0 && img_alpha<1.0)
        //    img_alpha+=(timestamp-app->additive_progress)/1000;


        dest_buffer.unlock();
        return 0;


	}

};