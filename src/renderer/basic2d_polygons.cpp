#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <array>
#include <functional>

#include "../core/common.h"

#include "../math/math.h"
#include "basic2d_color.h"
#include "basic3d_vertex.h"
#include "basic2d_routines.h"

void DrawTriangleLines(int* buf, int argb,
					ushine::math::base_vector_4<float> & v1,
					ushine::math::base_vector_4<float> & v2,
					ushine::math::base_vector_4<float> & v3,
							  int pitch, int height)
{
	EFLALine(buf, argb, v1.x, v1.y, v2.x,v2.y,pitch, height);
	EFLALine(buf, argb, v2.x, v2.y, v3.x,v3.y,pitch, height);
	EFLALine(buf, argb, v3.x, v3.y, v1.x,v1.y,pitch, height);
}

void horizontal_line(int* buf, int x0, int x1, int y, int argb, int pitch, int height)
{
	int sx, ex;

	if (y<0 || y>=height)
		return;

	sx = x0;
	ex = x1;

	if (sx<0) sx=0;
	if (ex>=pitch) ex=pitch-1;

	for (int x=sx; x<=ex; x++)
		buf[x+y*pitch]= argb;
}

void horizontal_line_blended(int* buf, int x0, int x1, int y, int argb, int pitch, int height)
{
	int sx, ex;

	if (y<0 || y>=height)
		return;

	sx = x0;
	ex = x1;

	if (sx<0) sx=0;
	if (ex>=pitch) ex=pitch-1;
  uint32_t* bb = reinterpret_cast<uint32_t*>(buf);
  auto c = ushine::renderer::color_f(argb);
	for (int x=sx; x<=ex; x++) {
    size_t o = x + y * pitch;
    auto b = ushine::renderer::color_f(*(bb + o));
		*(bb + o) = ushine::renderer::blend(c, b, (1.0f-c.a)).to_argb();
  }
}

void DrawTriangleFilled(int* buf, int argb,
					ushine::math::base_vector_4<float> & v1,
					ushine::math::base_vector_4<float> & v2,
					ushine::math::base_vector_4<float> & v3,
							  int pitch, int height, draw_horizontal_line_func draw_horizontal_line)
{

	ushine::math::base_vector_3<float> pv1, pv2, pv3;
	ushine::math::base_vector_3<float> s, e;
	double dx1, dx2, dx3;

	pv1.x = v1.x;
	pv1.y = v1.y;
	pv2.x = v2.x;
	pv2.y = v2.y;
	pv3.x = v3.x;
	pv3.y = v3.y;

	if (pv1.y > pv2.y) pv1.swap(pv2);
	if (pv1.y > pv3.y) pv1.swap(pv3);
	if (pv2.y > pv3.y) pv2.swap(pv3);

	s = e = pv1;

	if (pv2.y-pv1.y > 0) dx1=(pv2.x-pv1.x)/(pv2.y-pv1.y); else dx1=0;
	if (pv3.y-pv1.y > 0) dx2=(pv3.x-pv1.x)/(pv3.y-pv1.y); else dx2=0;
	if (pv3.y-pv2.y > 0) dx3=(pv3.x-pv2.x)/(pv3.y-pv2.y); else dx3=0;

	if(dx1 > dx2) {

		for(;s.y<=pv2.y;s.y++,e.y++,s.x+=dx2,e.x+=dx1)
			draw_horizontal_line(buf,s.x,e.x,s.y,argb, pitch, height);
		e=pv2;
		for(;s.y<=pv3.y;s.y++,e.y++,s.x+=dx2,e.x+=dx3)
			draw_horizontal_line(buf,s.x,e.x,s.y,argb, pitch, height);

	} else {
		for(;s.y<=pv2.y;s.y++,e.y++,s.x+=dx1,e.x+=dx2)
			draw_horizontal_line(buf,s.x,e.x,s.y,argb, pitch, height);
		s=pv2;
		for(;s.y<=pv3.y;s.y++,e.y++,s.x+=dx3,e.x+=dx2)
			draw_horizontal_line(buf,s.x,e.x,s.y,argb, pitch, height);
	}

}

void interpolated_line(
			int* buf,
			int x0, int x1, int y,
			ushine::renderer::color_f& argb_s, ushine::renderer::color_f& argb_e,
			int pitch, int height)
{
	int sx, ex, len;

	if (y<0 || y>=height)
		return;

	sx = x0;
	ex = x1;

	//if (sx<0) sx=0;
	if (ex>=pitch) ex=pitch-1;

	len = ex-sx;
	if (len<=0)
		return;

	ushine::renderer::color_f color_delta;
	color_delta.r = (argb_e.r - argb_s.r)/len;
	color_delta.g = (argb_e.g - argb_s.g)/len;
	color_delta.b = (argb_e.b - argb_s.b)/len;

	ushine::renderer::color_f color = argb_s;

	for (int x=sx; x<=ex; x++)
	{
		if (x>=0)
			buf[x+y*pitch]= color.to_argb();

		color.r += color_delta.r;
		color.g += color_delta.g;
		color.b += color_delta.b;

	}
}


void DrawTriangleShaded(int* buf,
					ushine::renderer::basic3d_vertex & v1,
					ushine::renderer::basic3d_vertex & v2,
					ushine::renderer::basic3d_vertex & v3,
							  int pitch, int height)
{
	using namespace ushine::renderer;

	ushine::math::base_vector_3<float> pv1, pv2, pv3;
	ushine::renderer::color_f pc1, pc2, pc3;

	ushine::math::base_vector_3<float> s, e;
	ushine::renderer::color_f sc, ec;

	float dx1, dx2, dx3;
	float dr1, dr2, dr3;
	float dg1, dg2, dg3;
	float db1, db2, db3;

	pv1.x = v1.coords2d.x;
	pv1.y = v1.coords2d.y;
	pc1   = v1.argb;

	pv2.x = v2.coords2d.x;
	pv2.y = v2.coords2d.y;
	pc2   = v2.argb;

	pv3.x = v3.coords2d.x;
	pv3.y = v3.coords2d.y;
	pc3	  = v3.argb;

	if (pv1.y > pv2.y) 
	{
		pv1.swap(pv2);			pc1.swap(pc2);
	}
	if (pv1.y > pv3.y) 
	{
		pv1.swap(pv3);			pc1.swap(pc3);
	}
	if (pv2.y > pv3.y) 
	{
		pv2.swap(pv3);			pc2.swap(pc3);
	}

	s = e = pv1;
	sc = ec = pc1;

	/*
		calculating vertical deltas for interpolation on the left/right edges;
	*/

	if (pv2.y-pv1.y > 0)
	{
		dx1=(pv2.x-pv1.x)/(pv2.y-pv1.y);
		dr1=(pc2.r-pc1.r)/(pv2.y-pv1.y);
		dg1=(pc2.g-pc1.g)/(pv2.y-pv1.y);
		db1=(pc2.b-pc1.b)/(pv2.y-pv1.y);
	} else dx1=dr1=dg1=db1=0;

	if (pv3.y-pv1.y > 0)
	{
		dx2=(pv3.x-pv1.x)/(pv3.y-pv1.y);
		dr2=(pc3.r-pc1.r)/(pv3.y-pv1.y);
		dg2=(pc3.g-pc1.g)/(pv3.y-pv1.y);
		db2=(pc3.b-pc1.b)/(pv3.y-pv1.y);
	} else dx2=dr2=dg2=db2=0;

	if (pv3.y-pv2.y > 0) {
		dx3=(pv3.x-pv2.x)/(pv3.y-pv2.y);
		dr3=(pc3.r-pc2.r)/(pv3.y-pv2.y);
		dg3=(pc3.g-pc2.g)/(pv3.y-pv2.y);
		db3=(pc3.b-pc2.b)/(pv3.y-pv2.y);
	} else dx3=dr3=dg3=db3=0;

	//drawing interpolated triangle

	if(dx1 > dx2) {
		for(;s.y<=pv2.y; s.y++)
		{
			interpolated_line(
					buf,floor(s.x),ceil(e.x),s.y,
					sc, ec,
					pitch, height);

			e.y++;
			s.x+=dx2; e.x+=dx1;
			sc.r+=dr2; ec.r+=dr1;
			sc.g+=dg2; ec.g+=dg1;
			sc.b+=db2; ec.b+=db1;
		}
		e=pv2;

		for(;s.y<=pv3.y;s.y++)
		{
			interpolated_line(
					buf,floor(s.x),ceil(e.x),s.y,
					sc, ec,
					pitch, height);

			e.y++;
			s.x+=dx2; e.x+=dx3;
			sc.r+=dr2; ec.r+=dr3;
			sc.g+=dg2; ec.g+=dg3;
			sc.b+=db2; ec.b+=db3;
		}
	} else {

		for(;s.y<=pv2.y;s.y++)
		{
			interpolated_line(
					buf,floor(s.x),ceil(e.x),s.y,
					sc, ec,
					pitch, height);

			e.y++;
			s.x+=dx1; e.x+=dx2;
			sc.r+=dr1; ec.r+=dr2;
			sc.g+=dg1; ec.g+=dg2;
			sc.b+=db1; ec.b+=db2;
		}
		s=pv2;
		for(;s.y<=pv3.y; s.y++)
		{
			interpolated_line(
					buf,floor(s.x),ceil(e.x),s.y,
					sc, ec,
					pitch, height);

			e.y++;
			s.x+=dx3; e.x+=dx2;
			sc.r+=dr3; ec.r+=dr2;
			sc.g+=dg3; ec.g+=dg2;
			sc.b+=db3; ec.b+=db2;

		}
	}
}



void interpolated_texture_line(
			int* buf,
			int x0, int x1, int y,
			ushine::renderer::basic3d_vertex& s, ushine::renderer::basic3d_vertex& e,
			int pitch, int height,
			int* tex_buf, int tex_w, int tex_h,
			int * depth_buffer, float global_alpha=1.0)
{
	int sx, ex, len;

	if (y<0 || y>=height)
		return;

	sx = x0;
	ex = x1;

	//if (sx<0) {
	//	sx=0;
	//}
	//if (ex>=pitch) ex=pitch-1;

	len = ex-sx;
	if (len<=0)
		return;

	ushine::renderer::basic3d_vertex delta; //horizontal interpolation delta
	delta.argb.r = (e.argb.r - s.argb.r)/len;
	delta.argb.g = (e.argb.g - s.argb.g)/len;
	delta.argb.b = (e.argb.b - s.argb.b)/len;
	delta.vector.z = ((e.vector.z - s.vector.z)/len);
	delta.uv.x = (e.uv.x - s.uv.x)/len;
	delta.uv.y = (e.uv.y - s.uv.y)/len;

	std::cout << "horizontal_line, s.z ==" << s.vector.z << "; e.z ==" << e.vector.z << "; " << std::endl;
	std::cout << "				   delta.z ==" << delta.vector.z << std::endl;



	ushine::renderer::color_f color = s.argb;
	ushine::math::vector_2f uv = s.uv;
	float zval = s.vector.z;

	for (int x=sx; x<=ex; x++)
	{

		if (x>=0 && x<pitch)
		{

			//if (zval<=depth_buffer[x+y*pitch])
			//{

				int sample_x = int(floor(uv.x*(tex_w-1)))%(tex_w);
				int sample_y = int(floor(uv.y*(tex_h-1)))%(tex_w);

				ushine::renderer::color_f sample_color;

				int sample_argb = tex_buf[sample_x + sample_y*tex_w];

				sample_color.from_argb(sample_argb);

				sample_color.r = (sample_color.r * color.r);
				sample_color.g = (sample_color.g * color.g);
				sample_color.b = (sample_color.b * color.b);


					int alpha = global_alpha*255; //this is left as it has to go to the params
					int pixel_alpha = alpha;

					int src_col = sample_color.to_argb();
					int src_alpha = (src_col >> 24) & 255;

					if (src_alpha<255)
					{
						pixel_alpha = (src_alpha*alpha)/255;
					}

					if (pixel_alpha<255)
					{
						int dst_col = buf[x+y*pitch];
						int dst_alpha = 255-pixel_alpha;

						int src_b = (src_col & 0xff)* pixel_alpha >> 8;
						src_b += (dst_col & 0xff)* dst_alpha >> 8;
						int src_g = ((src_col >>8) & 0xff)* pixel_alpha >> 8;
						src_g += ((dst_col >>8) & 0xff)* dst_alpha >> 8;
						int src_r = ((src_col >>16) & 0xff)* pixel_alpha >> 8;
						src_r += ((dst_col >>16) & 0xff)* dst_alpha >> 8;
						int src_a = ((src_col >>24) & 0xff)* pixel_alpha >> 8;

						int melted_col = TO_ARGB(255, src_r, src_g,src_b);
						buf[x+y*pitch] = melted_col;
					} else buf[x+y*pitch] =src_col;

		}

		color.r += delta.argb.r;
		color.g += delta.argb.g;
		color.b += delta.argb.b;
		uv.x +=(delta.uv.x);
		uv.y +=(delta.uv.y);

		zval += delta.vector.z;

	}
}




void DrawTriangleTextured(int* buf,
					ushine::renderer::basic3d_vertex & v0,
					ushine::renderer::basic3d_vertex & v1,
					ushine::renderer::basic3d_vertex & v2,
					int pitch, int height, int* tex_buf, int tex_w, int tex_h,
					int * depth_buffer
				)
{
		using namespace ushine::renderer;

		basic3d_vertex pv1, pv2, pv3;
		basic3d_vertex s, e;

		float dx1, dx2, dx3;
		float dr1, dr2, dr3;
		float dg1, dg2, dg3;
		float db1, db2, db3;
		float da1, da2, da3; //interpolating alpha channel	
		float du1, du2, du3;
		float dv1, dv2, dv3;

		float dz1, dz2, dz3; //for perspective_correction

		pv1.copy_from(v0);
		pv2.copy_from(v1);
		pv3.copy_from(v2);

		if (pv1.vector.y > pv2.vector.y) pv1.swap(pv2);
		if (pv1.vector.y > pv3.vector.y) pv1.swap(pv3);
		if (pv2.vector.y > pv3.vector.y) pv2.swap(pv3);

		s.copy_from(pv1);
		e.copy_from(pv1);

		if (pv2.vector.y-pv1.vector.y > 0)
		{
			dx1=(pv2.vector.x-pv1.vector.x)/(pv2.vector.y-pv1.vector.y);

			dz1=(pv2.vector.z-pv1.vector.z)/(pv2.vector.y-pv1.vector.y);

			dr1=(pv2.argb.r-pv1.argb.r)/(pv2.vector.y-pv1.vector.y);
			dg1=(pv2.argb.g-pv1.argb.g)/(pv2.vector.y-pv1.vector.y);
			db1=(pv2.argb.b-pv1.argb.b)/(pv2.vector.y-pv1.vector.y);
			da1=(pv2.argb.a-pv1.argb.a)/(pv2.vector.y-pv1.vector.y);

			du1=(pv2.uv.x-pv1.uv.x)/(pv2.vector.y-pv1.vector.y);
			dv1=(pv2.uv.y-pv1.uv.y)/(pv2.vector.y-pv1.vector.y);

		} else dx1=dz1=dr1=dg1=db1=du1=dv1=0;

		if (pv3.vector.y-pv1.vector.y > 0)
		{
			dx2=(pv3.vector.x-pv1.vector.x)/(pv3.vector.y-pv1.vector.y);

			dz2=(pv3.vector.z-pv1.vector.z)/(pv3.vector.y-pv1.vector.y);

			dr2=(pv3.argb.r-pv1.argb.r)/(pv3.vector.y-pv1.vector.y);
			dg2=(pv3.argb.g-pv1.argb.g)/(pv3.vector.y-pv1.vector.y);
			db2=(pv3.argb.b-pv1.argb.b)/(pv3.vector.y-pv1.vector.y);
			da2=(pv3.argb.a-pv1.argb.a)/(pv3.vector.y-pv1.vector.y);

			du2=(pv3.uv.x-pv1.uv.x)/(pv3.vector.y-pv1.vector.y);
			dv2=(pv3.uv.y-pv1.uv.y)/(pv3.vector.y-pv1.vector.y);

		} else dx2=dz2=dr2=dg2=db2=du2=dv2=0;

		if (pv3.vector.y-pv2.vector.y > 0) {
			dx3=(pv3.vector.x-pv2.vector.x)/(pv3.vector.y-pv2.vector.y);

			dz3=(pv3.vector.z-pv2.vector.z)/(pv3.vector.y-pv2.vector.y);

			dr3=(pv3.argb.r-pv2.argb.r)/(pv3.vector.y-pv2.vector.y);
			dg3=(pv3.argb.g-pv2.argb.g)/(pv3.vector.y-pv2.vector.y);
			db3=(pv3.argb.b-pv2.argb.b)/(pv3.vector.y-pv2.vector.y);
			da3=(pv3.argb.a-pv2.argb.a)/(pv3.vector.y-pv2.vector.y);

			du3=(pv3.uv.x-pv2.uv.x)/(pv3.vector.y-pv2.vector.y);
			dv3=(pv3.uv.y-pv2.uv.y)/(pv3.vector.y-pv2.vector.y);


		} else dx3=dz3=dr3=dg3=db3=du1=du2=0;

		if(dx1 > dx2) {
			for(;s.vector.y<=pv2.vector.y; s.vector.y++)
			{
				int start_x = floor(s.vector.x);
				int end_x = ceil(e.vector.x);

				interpolated_texture_line(
						buf,start_x,end_x,s.vector.y,
						s, e,
						pitch, height,
						tex_buf, tex_w, tex_h, depth_buffer);

				e.vector.y++;

				s.vector.x+=dx2; e.vector.x+=dx1;
				s.vector.z+=dz2; e.vector.z+=dz1;

				s.argb.r+=dr2; e.argb.r+=dr1;
				s.argb.g+=dg2; e.argb.g+=dg1;
				s.argb.b+=db2; e.argb.b+=db1;
				s.argb.a+=da2; e.argb.a+=da1;

				s.uv.x+=du2; e.uv.x+=du1;
				s.uv.y+=dv2; e.uv.y+=dv1;

			}
			e=pv2;

			for(;s.vector.y<=pv3.vector.y;s.vector.y++)
			{
				int start_x = floor(s.vector.x);
				int end_x = ceil(e.vector.x);

				interpolated_texture_line(
						buf,start_x,end_x,s.vector.y,
						s, e,
						pitch, height,
						tex_buf, tex_w, tex_h, depth_buffer);

				e.vector.y++;

				s.vector.x+=dx2; e.vector.x+=dx3;
				s.vector.z+=dz2; e.vector.z+=dz3;

				s.argb.r+=dr2; e.argb.r+=dr3;
				s.argb.g+=dg2; e.argb.g+=dg3;
				s.argb.b+=db2; e.argb.b+=db3;
				s.argb.a+=da2; e.argb.a+=da3;

				s.uv.x+=du2; e.uv.x+=du3;
				s.uv.y+=dv2; e.uv.y+=dv3;

			}
		} else {

			for(;s.vector.y<=pv2.vector.y;s.vector.y++)
			{
				int start_x = floor(s.vector.x);
				int end_x = ceil(e.vector.x);

				interpolated_texture_line(
						buf,start_x,end_x,s.vector.y,
						s, e,
						pitch, height,
						tex_buf, tex_w, tex_h, depth_buffer);

				e.vector.y++;

				s.vector.x+=dx1; e.vector.x+=dx2;
				s.vector.z+=dz1; e.vector.z+=dz2;

				s.argb.r+=dr1; e.argb.r+=dr2;
				s.argb.g+=dg1; e.argb.g+=dg2;
				s.argb.b+=db1; e.argb.b+=db2;
				s.argb.a+=da1; e.argb.a+=da2;

				s.uv.x+=du1; e.uv.x+=du2;
				s.uv.y+=dv1; e.uv.y+=dv2;
			}
			s=pv2;
			for(;s.vector.y<=pv3.vector.y; s.vector.y++)
			{
				int start_x = floor(s.vector.x);
				int end_x = ceil(e.vector.x);

				interpolated_texture_line(
						buf,start_x,end_x,s.vector.y,
						s, e,
						pitch, height,
						tex_buf, tex_w, tex_h, depth_buffer);

				e.vector.y++;

				s.vector.x+=dx3; e.vector.x+=dx2;
				s.vector.z+=dz3; e.vector.z+=dz2;

				s.argb.r+=dr3; e.argb.r+=dr2;
				s.argb.g+=dg3; e.argb.g+=dg2;
				s.argb.b+=db3; e.argb.b+=db2;
				s.argb.a+=db3; e.argb.a+=db2;

				s.uv.x+=du3; e.uv.x+=du2;
				s.uv.y+=dv3; e.uv.y+=dv2;

			}
		}

}
