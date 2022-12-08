
#include <iostream>

#include "../math/math.h"
#include "basic2d_color.h"
#include "basic3d_vertex.h"


#include "basic2d_context.h"

#include "basic2d_routines.h"

namespace ushine
{
	namespace renderer
	{

		class basic2d_context::pimpl
		{
		public:
			int width;
			int height;
			int pitch; //currently not used

			int* depth_buffer;
			//
			inline void init_depthbuffer()
			{
				depth_buffer= new int[width*height];
			}

			inline void pixel(int* buf, int argb, int x, int y)
			{
				PutPixelGeneric(buf, argb, x, y, width, height);
			}

			inline void line(int* buf, int argb, int x1, int y1, int x2, int y2)
			{
				EFLALine(buf, argb, x1, y1, x2, y2, width, height);
			}

			inline void triangle_lines(int* buf, int argb,
					math::base_vector_4<float> & v1,
					math::base_vector_4<float> & v2,
					math::base_vector_4<float> & v3)
			{
				DrawTriangleLines(buf, argb, v1, v2, v3, width, height);
			}

			inline void triangle_fill(int* buf, int argb,
					math::base_vector_4<float> & v1,
					math::base_vector_4<float> & v2,
					math::base_vector_4<float> & v3)
			{
				DrawTriangleFilled(buf, argb, v1, v2, v3, width, height, horizontal_line);
			}

			inline void triangle_shade(int* buf,
					basic3d_vertex & v1,
					basic3d_vertex & v2,
					basic3d_vertex & v3)
			{
				DrawTriangleShaded(buf, v1, v2, v3, width, height);
			}

			inline void triangle_textured(int* buf,
					basic3d_vertex & v1,
					basic3d_vertex & v2,
					basic3d_vertex & v3,
					ushine::core::texture& texture)
			{
				DrawTriangleTextured(buf, v1, v2, v3, width, height,
					(int*)texture.get_buffer(), texture.get_width(), texture.get_height(), depth_buffer);
			}

		protected:

			inline int static _argb(int a, int r, int g, int b)
			{
				return TO_ARGB(a,r,g,b);
			}

		};

		basic2d_context::basic2d_context() : impl(new pimpl)
		{

		}

		basic2d_context::~basic2d_context()
		{

		}

		void basic2d_context::init(int w, int h)
		{
			impl->width = w;
			impl->height = h;
			impl->init_depthbuffer();
		}

		void basic2d_context::clear(int* buf, int a, int r, int g, int b, int w, int h)
		{
			for (auto i =0; i<w*h; i++)
			{
				buf[i] = TO_ARGB(a,r,g,b);
			}
		}

		void basic2d_context::clear_depth(int w, int h)
		{
			for (auto i=0; i< w*h; i++)
				impl->depth_buffer[i]=std::numeric_limits<int>::max();
		}

		int* basic2d_context::get_depth_buffer()
		{
			return impl->depth_buffer;
		}


		void basic2d_context::draw_rect(
			int* buf,
			int a, int r, int g, int b,
			int x, int y,
			int w, int h)
		{

			unsigned col = TO_ARGB(a,r,g,b);
			int x0 =x,
				y0 =y,
				x1 =x+w,
				y1 =y+h;

			if (x0<0) x0=0;
			if (y0<0) y0=0;
			if (x1>impl->width) x1=impl->width;
			if (y1>impl->height) y1=impl->height;

			for (auto i = x0; i< x1; i++)
				for (auto j = y0; j<y1; j++)
				{
					buf[i+j*impl->width] = col;
				}
		}

		void basic2d_context::put_pixel(int* buf, int argb, int x, int y)
		{
			impl->pixel(buf, argb, x, y);
		}

		void basic2d_context::draw_line(
			int* buf, int argb,
			int x1, int y1, int x2, int y2
			)
		{
			impl->line(buf, argb, x1, y1, x2, y2);
		}

		void basic2d_context::draw_triangle_lines(int*buf, int argb,
			math::base_vector_4<float> v1,
			math::base_vector_4<float> v2,
			math::base_vector_4<float> v3)
		{
			impl->triangle_lines(buf, argb,v1, v2, v3);
		}

		void basic2d_context::draw_triangle_fill(int*buf, int argb,
			math::base_vector_4<float> v1,
			math::base_vector_4<float> v2,
			math::base_vector_4<float> v3)
		{
			impl->triangle_fill(buf, argb,v1, v2, v3);
		}

		void basic2d_context::draw_triangle_shade(int* buf,
			basic3d_vertex& v1,
			basic3d_vertex& v2,
			basic3d_vertex& v3)
		{
			impl->triangle_shade(buf, v1, v2, v3);
		}

		void basic2d_context::draw_triangle_textured(int* buf,
			basic3d_vertex& v1,
			basic3d_vertex& v2,
			basic3d_vertex& v3,
			ushine::core::texture& texture)
		{
			impl->triangle_textured(buf, v1, v2, v3,texture);
		}


	};
};
