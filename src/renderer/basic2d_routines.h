#ifndef BASIC2D_ROUTINES
#define BASIC2D_ROUTINES

#ifndef TO_ARGB
#define TO_ARGB(a,r,g,b) (a << 24) + (r << 16) + (g << 8) + b
#endif

extern void PutPixelGeneric(int* buf, int col, int x, int y, int pitch, int height);
extern void MidPointLine(
	int* buf, int argb,
	int x1, int y1, int x2, int y2,
	int pitch, int height
	);
extern void EFLALine(int* buf, int argb,
			int x1, int y1, int x2, int y2,
			int pitch, int height);

extern void DrawTriangleLines(int* buf, int argb,
					ushine::math::base_vector_4<float> & v1,
					ushine::math::base_vector_4<float> & v2,
					ushine::math::base_vector_4<float> & v3,
							  int pitch, int height);

typedef void(*draw_horizontal_line_func)(int*, int, int, int, int, int, int);
extern void horizontal_line(int* buf, int x0, int x1, int y, int argb, int pitch, int height);
extern void horizontal_line_blended(int* buf, int x0, int x1, int y, int argb, int pitch, int height);

extern void DrawTriangleFilled(int* buf, int argb,
					ushine::math::base_vector_4<float> & v1,
					ushine::math::base_vector_4<float> & v2,
					ushine::math::base_vector_4<float> & v3,
							  int pitch, int height, draw_horizontal_line_func draw_horizontal_line = horizontal_line);

inline void DrawTriangleFilledBlended(int* buf, int argb,
					ushine::math::base_vector_4<float> & v1,
					ushine::math::base_vector_4<float> & v2,
					ushine::math::base_vector_4<float> & v3,
							  int pitch, int height)
{
  DrawTriangleFilled(buf, argb, v1, v2, v3, pitch, height, horizontal_line_blended);
}

extern void DrawTriangleShaded(int* buf,
					ushine::renderer::basic3d_vertex & v1,
					ushine::renderer::basic3d_vertex & v2,
					ushine::renderer::basic3d_vertex & v3,
							  int pitch, int height);

extern void DrawTriangleTextured(int* buf,
					ushine::renderer::basic3d_vertex & v1,
					ushine::renderer::basic3d_vertex & v2,
					ushine::renderer::basic3d_vertex & v3,
							  int pitch, int height,
								int* tex_buf, int tex_w, int tex_h,
								int * depth_buffer
					);


#endif
