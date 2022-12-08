

#ifndef USHINE_RENDERER_BASIC
#define USHINE_RENDERER_BASIC

#include "../core/common.h"
#include "../core/texture.h"
//#include "./basic3d_vertex.h"

#ifndef TO_ARGB
#define TO_ARGB(a,r,g,b) (a << 24) + (r << 16) + (g << 8) + b
#endif

namespace ushine
{
	namespace renderer
	{

		class DYNLIB_EXPORT basic2d_context
		{
		public:
			basic2d_context();
			virtual ~basic2d_context();

			void init(int w, int h);
			void clear(int* buf, int a, int r, int g, int b, int w, int h);
			void clear_depth(int w, int h);

			int* get_depth_buffer();

			void draw_rect(
				int* buf,
				int a, int r, int g, int b,
				int x, int y,
				int w, int h);

			void put_pixel(int* buf, int argb, int x, int y);
			void draw_line(int* buf, int argb, int x1, int y1, int x2, int y2);

			void draw_triangle_lines(int*buf, int argb,
				ushine::math::base_vector_4<float> v1,
				ushine::math::base_vector_4<float> v2,
				ushine::math::base_vector_4<float> v3);

			void draw_triangle_fill(int*buf, int argb,
				ushine::math::base_vector_4<float> v1,
				ushine::math::base_vector_4<float> v2,
				ushine::math::base_vector_4<float> v3);

			void draw_triangle_shade(int*buf,
				basic3d_vertex& v1,
				basic3d_vertex& v2,
				basic3d_vertex& v3);

			void draw_triangle_textured(int*buf,
				basic3d_vertex& v1,
				basic3d_vertex& v2,
				basic3d_vertex& v3,
				ushine::core::texture& texture);

		private:

			class pimpl;
			std::unique_ptr<pimpl> impl;

		};
	};
};


#endif
