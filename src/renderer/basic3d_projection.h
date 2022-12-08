#ifndef BASIC3D_PROJECTION
#define BASIC3D_PROJECTION


namespace ushine
{
	namespace renderer
	{

////////////////////////////////////////////////////////////////////////
////					3D TRANSLATION MATRIX
////////////////////////////////////////////////////////////////////////
		inline ushine::math::base_matrix_4<float> matrix_translate(float x, float y, float z)
		{
			ushine::math::base_matrix_4<float> out;
			out.row1.set(1,0,0,x);
			out.row2.set(0,1,0,y);
			out.row3.set(0,0,1,z);
			out.row4.set(0,0,0,1);

			return out;
		}
		////////////////////////////////////////////////////////////////////////
		////					3D SCALING MATRIX
		////////////////////////////////////////////////////////////////////////
		inline ushine::math::base_matrix_4<float> matrix_scale(float x, float y, float z)
		{
			ushine::math::base_matrix_4<float> out;
			out.row1.set(x,0,0,0);
			out.row2.set(0,y,0,0);
			out.row3.set(0,0,z,0);
			out.row4.set(0,0,0,1);

			return out;
		}
		////////////////////////////////////////////////////////////////////////
		////					3D ROTATION(X) MATRIX
		////////////////////////////////////////////////////////////////////////
		inline ushine::math::base_matrix_4<float> matrix_rotate_x(float angle)
		{
			ushine::math::base_matrix_4<float> out;
			float cos_value = cosf(angle);
			float sin_value = sinf(angle);

			out.row1.set(1,0,0,0);
			out.row2.set(0,cos_value,-sin_value,0);
			out.row3.set(0,sin_value,cos_value,0);
			out.row4.set(0,0,0,1);

			return out;
		}
		////////////////////////////////////////////////////////////////////////
		////					3D ROTATION(Y) MATRIX
		////////////////////////////////////////////////////////////////////////
		inline ushine::math::base_matrix_4<float> matrix_rotate_y(float angle)
		{
			ushine::math::base_matrix_4<float> out;
			float cos_value = cosf(angle);
			float sin_value = sinf(angle);

			out.row1.set(cos_value,0,sin_value,0);
			out.row2.set(0,1,0,0);
			out.row3.set(-sin_value,0,cos_value,0);
			out.row4.set(0,0,0,1);

			return out;
		}
		////////////////////////////////////////////////////////////////////////
		////					3D ROTATION(Z) MATRIX
		////////////////////////////////////////////////////////////////////////
		inline ushine::math::base_matrix_4<float> matrix_rotate_z(float angle)
		{
			ushine::math::base_matrix_4<float> out;
			float cos_value = cosf(angle);
			float sin_value = sinf(angle);

			out.row1.set(cos_value,-sin_value,0,0);
			out.row2.set(sin_value,cos_value,0,0);
			out.row3.set(0,0,1,0);
			out.row4.set(0,0,0,1);

			return out;
		}

		////////////////////////////////////////////////////////////////////////
		////					3D PROJECTION MATRIX (FOV based)
		////////////////////////////////////////////////////////////////////////

 		inline ushine::math::base_matrix_4<float> matrix_projection_perspective_fovlh(
			float fov, float screen_w, float screen_h,
			float near_z, float far_z)
		{

			ushine::math::base_matrix_4<float> out;

			auto height = 1.0 / tan(fov/2);
			auto width = screen_w/screen_h;

			out.row1.set(width,0,0,0);
			out.row2.set(0,height,0,0);
			out.row3.set(0,0,far_z/(far_z - near_z),1.0);
			out.row4.set(0,0,(near_z*far_z)/(near_z-far_z),0);


			return out;
		}


/*
Vector axisZ = (target - eye).normalize();
Vector axisX = up.cross(axisZ).normalize();
Vector axisY = axisZ.cross(axisX).normalize();

float eyeX = -axisX.dot(eye);
float eyeY = -axisY.dot(eye);
float eyeZ = -axisZ.dot(eye);

float values[16] = {
		axisX.x, axisY.x, axisZ.x, 0,
		axisX.y, axisY.y, axisZ.y, 0,
		axisX.z, axisY.z, axisZ.z, 0,
		eyeX,    eyeY,    eyeZ,    1,
};
		*/

		inline ushine::math::base_matrix_4<float> matrix_view(
				math::vector_4f position,
				math::vector_4f look_at,
				math::vector_4f look_up
		)

		{

			auto zaxis = (look_at - position).normalize();
			auto xaxis = look_up.cross(zaxis).normalize();
			auto yaxis = zaxis.cross(xaxis).normalize();

			ushine::math::base_matrix_4<float> out;

			out.row1.set(xaxis.x,yaxis.x,zaxis.x,0);
			out.row2.set(xaxis.y,yaxis.y,zaxis.y,0);
			out.row3.set(xaxis.z,yaxis.z,zaxis.z,0);
			out.row4.set(-xaxis.dot(position), -yaxis.dot(position), -zaxis.dot(position),1);

			return out;
		}



		class camera
		{
		public:
			math::vector_4f position;
			math::vector_4f look_at;
			math::vector_4f look_up;

			math::vector_3f rotation; //pitch, yaw, roll
			math::vector_2f z_cliping; //z_near, z_far

			float screen_width, screen_height;

			inline float aspect_ratio()
			{
				return screen_width/screen_height;
			}

			math::base_matrix_4<float> view_matrix;
			math::base_matrix_4<float> world_matrix;
			math::base_matrix_4<float> projection_matrix;

			void generate_projection_matrix()
			{

				projection_matrix =
					 matrix_projection_perspective_fovlh(
					 	0.78, screen_width,screen_height,
					 	z_cliping[0], z_cliping[1]);
			}

			void generate_world_matrix()
			{
				world_matrix = matrix_rotate_x(rotation.x)
							 * matrix_rotate_y(rotation.y)
							 * matrix_rotate_z(rotation.z)
							 * matrix_translate(-position.x, -position.y, -position.z);
			}

			void generate_up_at_vectors()
			{
//				look_at.set(0,0,1);
//				look_up.set(0,1,0);
				look_at = world_matrix*look_at;
				look_up = world_matrix*look_up;
			}


			math::vector_2i project_to_screen(basic3d_vertex& vertex)
			{
				math::base_matrix_4<float> conversion_matrix;
				math::base_vector_4<float> determined;
				conversion_matrix = world_matrix*projection_matrix;


				std::cout << "conversion matrix =" <<
						  std::endl << conversion_matrix[0][0] << "; "
							 				  << conversion_matrix[0][1] << "; "
												<< conversion_matrix[0][2] << "; "
												<< conversion_matrix[0][3] << "; "
						  << std::endl;

				determined.set(vertex.vector.x, vertex.vector.y, vertex.vector.z, .0);

				math::vector_4f converted_point = conversion_matrix * determined;

				std::cout << "original point 3d = {"
						  << determined.x << ","
						  << determined.y << ","
						  << determined.z << ","
						  << determined.w << "}"
						  << std::endl;

				math::vector_2i out;

				out.x =  converted_point.x * screen_height + screen_width /2;
				out.y =  -converted_point.y * screen_width + screen_height/2;

				std::cout << "projected point 2d = {"
						  << out.x / converted_point.w << ","
						  << out.y / converted_point.w << "}"
							<< std::endl
							<< "--------------------------------"
							<< std::endl;

				return out;
			}

		};

	};
};





#endif
