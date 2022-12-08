
#include <iostream>

#include "core/core.h"
#include "math/math.h"
#include "renderer/renderer.h"

using namespace ushine::math;
using namespace ushine::renderer;

void print_matrix_2(base_matrix_2<float> m, std::string desc)
{
	std::cout << desc << std::endl;
	std::cout << m[0][0] << "; " << m[0][1] << std::endl;
	std::cout << m[1][0] << "; " << m[1][1] << std::endl;
}

void print_matrix_4(base_matrix_4<float> m, std::string desc)
{
	std::cout << desc << std::endl;
	std::cout << m[0][0] << "; " << m[0][1] << "; " << m[0][2] << "; " << m[0][3] << std::endl;
	std::cout << m[1][0] << "; " << m[1][1] << "; " << m[1][2] << "; " << m[1][3] << std::endl;
	std::cout << m[2][0] << "; " << m[2][1] << "; " << m[2][2] << "; " << m[2][3] << std::endl;
	std::cout << m[3][0] << "; " << m[3][1] << "; " << m[3][2] << "; " << m[3][3] << std::endl;
}

void print_vertex(basic3d_vertex& v , std::string desc)
{
	std::cout << desc << std::endl;
	std::cout << "vertex_data => " << std::endl
			  << "\tvector.x: " << v.vector.x 
			  << "\tvector.y: " << v.vector.y 
			  << "\tvector.z: " << v.vector.z 
			  << "\targb: " << v.argb.to_argb()
			  << "\tcoords2d.x: " << v.coords2d.x 
			  << "\tcoords2d.y: " << v.coords2d.y
			  << "\tcoords2d.z: " << v.coords2d.z
			  << std::endl; 
}

void print_vector(ushine::math::base_vector_4<float>& v, std::string desc)
{
	std::cout << desc << std::endl;
	std::cout << "vector_data => " << std::endl
			  << "\tvector.x: " << v.x 
			  << "\tvector.y: " << v.y 
			  << "\tvector.z: " << v.z 
			  << std::endl; 
}


ushine::math::base_vector_4<float> project(
	ushine::math::base_vector_4<float> origin, 
	float view_w, float view_h, float fov, float view_dist)
{
	ushine::math::base_vector_4<float> out;

	auto factor = fov/(view_dist+origin.z);

	out.x = origin.x * factor + view_w/2.0f;
	out.y = origin.y * factor + view_h/2.0f;
	//out.z = origin.z;

	return out;
}


int main(int argc, char** argv)
{
	basic3d_vertex points[3];
	basic3d_vertex transformed[3];

	points[0].vector.set(-1, 1, -1,1);
	points[0].argb = ushine::renderer::color_f(1.0,1.0,0.0,0.0);
	points[0].uv.set(0,0);

	points[1].vector.set(1, -1, -1,1);	
	points[1].argb = ushine::renderer::color_f(1.0,0.0,1.0,0.0);	
	points[1].uv.set(1,0);

	points[2].vector.set(-1, -1, -1,1);
	points[2].argb = ushine::renderer::color_f(1.0,0.0,0.0,1.0);
	points[2].uv.set(1,1);

	auto m = ushine::renderer::matrix_translate(
				0,0,
				10.0);

	for (auto i=0; i<3; i++)
	{
		std::cout << "FOR_START =================" <<std::endl;
		transformed[i]=points[i];

		base_vector_4<float> out;

		out=points[i].vector*m;

		print_vector(points[i].vector, std::string("points[i].vector"));
		print_vector(out, std::string("out"));

		std::cout << "FOR_END =================" <<std::endl;

		//transformed[i].vector = m*transformed[i].vector;
		//transformed[i].coords2d = project(points[i].vector,320,240,256,10.0);
	}

/*
	for (auto i=0; i<3; i++)
	{
		print_vertex(points[i], std::string("points"));
		print_vertex(transformed[i], std::string("transformed"));
	}
*/


	return 0;
}
