#ifndef BASIC3D_VERTEX
#define BASIC3D_VERTEX

#include <iostream>

namespace ushine
{
	namespace renderer
	{

		#define VERTEX_VECTOR 	1
		#define VERTEX_NORMAL 	2
		#define VERTEX_UV1		4
		#define VERTEX_UV2		8
		#define VERTEX_UV3		16
		#define VERTEX_ARGB		32

		class color_f;

		class basic3d_vertex
		{
		public:

			math::vector_4f vector;
			math::vector_4f normal;
			math::vector_2f uv;
			color_f 		argb;

			int 			uv_index;
			int 			type;

			math::vector_4f coords2d;

			basic3d_vertex() {

			}

			basic3d_vertex(basic3d_vertex& vertex) {

				copy_from(vertex);
			}

			inline void copy_from(basic3d_vertex& vertex)
			{
				this->vector = vertex.vector;
				this->normal = vertex.normal;
				this->uv     = vertex.uv;
				this->argb	 = vertex.argb;
				this->uv_index=vertex.uv_index;
				this->type   = vertex.type;
				
				this->coords2d.x=vertex.coords2d.x;
				this->coords2d.y=vertex.coords2d.y;
				this->coords2d.z=vertex.coords2d.z;
			}

			virtual ~basic3d_vertex() {

			}

			inline void swap(basic3d_vertex & right)
			{
				basic3d_vertex temp = right;

				right.vector = this->vector;
				right.uv = this->uv;
				right.argb = this->argb;
				this->vector = temp.vector;
				this->uv = temp.uv;
				this->argb = temp.argb;
			}

			inline void transform_to_2d(
						int screen_w, int screen_h,
						float fov, float view_distance)
			{

				const float zInv = 1.0f / vector.z;
				const float xFactor = float( screen_w ) / 2.0f;
				const float yFactor = float( screen_h ) / 2.0f;

				coords2d.x = vector.x*zInv;
				coords2d.y = vector.y*zInv;
				coords2d.z = vector.z;
				coords2d.w = zInv;

				coords2d.x = (coords2d.x + 1.0f) * xFactor;
				coords2d.y = (-coords2d.y + 1.0f) * xFactor;

				/*
				float factor = fov/(view_distance+vector.z);

				this->coords2d.x = vector.x * factor + screen_w/2.0f;
				this->coords2d.y  = vector.y * factor + screen_h/2.0f;
				*/
			}


		private:
		};


	};
};

#endif
