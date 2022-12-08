#ifndef USHINE_MATH_VECTORS
#define USHINE_MATH_VECTORS

#include "vector_2.h"
#include "vector_3.h"

namespace ushine
{

	namespace math
	{

		enum vector_index_2
		{
			vi2_i1 = 0,
			vi2_i2 = 1
		};

		enum vector_index_3
		{
			vi3_i1 = 0,
			vi3_i2 = 1,
			vi3_i3 = 2
		};

		enum vector_index_4
		{
			vi4_i1 = 0,
			vi4_i2 = 1,
			vi4_i3 = 2,
			vi4_i4 = 3
		};

		template <class T> class base_vector_4 : public base_point_4<T>
		{
		public:
			base_point_4<T> origin;

			base_vector_4() {}

			base_vector_4(base_point_4<T>& p0, base_point_4<T> p1)
			{
				origin = p0;
				base_point_4<T>::x = p1.x - p0.x;
				base_point_4<T>::y = p1.y - p0.y;
				base_point_4<T>::z = p1.z - p0.z;
				base_point_4<T>::w = p1.w - p0.w;
			}

			base_vector_4(base_point_4<T>& p1)
			{
				origin.zero();
				base_point_4<T>::x = p1.x;
				base_point_4<T>::y = p1.y;
				base_point_4<T>::z = p1.z;
				base_point_4<T>::w = p1.w;
			}

			base_vector_4(base_vector_4<T>& v1)
			{
				origin.zero();
				base_point_4<T>::x = v1.x;
				base_point_4<T>::y = v1.y;
				base_point_4<T>::z = v1.z;
				base_point_4<T>::w = v1.w;
			}



			base_vector_4(T p) : base_vector_4(p, p, p, p){}
			base_vector_4(T p0, T p1, T p2, T p3)
			{
				origin.zero();
				base_point_4<T>::x = p0;
				base_point_4<T>::y = p1;
				base_point_4<T>::z = p2;
				base_point_4<T>::w = p3;
			}

     		inline bool is_zero() { this->x == this->y == this->z == this->w == 0; }

			inline bool non_zero()
			{
				return 	base_point_4<T>::x != 0 &&
						base_point_4<T>::y != 0 &&
						base_point_4<T>::z != 0 &&
						base_point_4<T>::w != 0;
			}

			void set(T x0=0, T y0=0, T z0=0, T w0=0)
			{
				origin.zero();
				base_point_4<T>::x = x0;
				base_point_4<T>::y = y0;
				base_point_4<T>::z = z0;
				base_point_4<T>::w = w0;
			}

			inline T dot(base_vector_4<T> right)
			{
				//to be implemented: l.len() * r.len * cos(alpha)
				return this->x * right.x + this->y*right.y + this->z*right.z +this->w*right.w;
			}


			inline base_vector_4<T> cross(base_vector_4<T> right)
			{
				base_vector_4<T> out;

				out.x = this->y * right.z - right.y * this->z;
				out.y = this->z * right.x - right.z * this->x;
				out.z = this->x * right.y - right.x * this->y;

				return out;
			}


			inline T& operator[] (int i)
			{
				if (i==0)
					return this->x;
				if (i==1)
					return this->y;
				if (i==2)
					return this->z;
				if (i==3)
					return this->w;
				return this->x; //error
			}

			inline T length()
			{
				return std::sqrt(
					base_point_4<T>::x*base_point_4<T>::x +
					base_point_4<T>::y*base_point_4<T>::y +
					base_point_4<T>::z*base_point_4<T>::z +
					base_point_4<T>::w*base_point_4<T>::w
					);
			}

			inline base_vector_4<T> normalize()
			{
				T len = length();
				base_vector_4<T> normalized_v;

				if (len!=0)
				{
					normalized_v.x = this->x/len;
					normalized_v.y = this->y/len;
					normalized_v.z = this->z/len;
					normalized_v.w = this->z/len;
				}

				return normalized_v;
			}

			inline base_vector_2<T> xy()
			{
				base_vector_2<T> out;
				out.x = this->x;
				out.y = this->y;
				return out;
			}
			inline base_vector_2<T> xz()
			{
				base_vector_2<T> out;
				out.x = this->x;
				out.y = this->z;
				return out;
			}
			inline base_vector_2<T> yz()
			{
				base_vector_2<T> out;
				out.x = this->y;
				out.y = this->z;
				return out;
      }

      base_vector_4<T>& operator-=(const base_vector_4<T>& rhs) { this->x -= rhs.x; this->y -= rhs.y; this->z -= rhs.z; this->w -= rhs.w; return *this; }
			base_vector_4<T>& operator-=(const T& rhs) { this->x -= rhs; this->y -= rhs; this->z -= rhs; this->w -= rhs; return *this; }
      base_vector_4<T>& operator+=(const base_vector_4<T>& rhs) { this->x += rhs.x; this->y += rhs.y; this->z += rhs.z; this->w += rhs.w; return *this; }
			base_vector_4<T>& operator+=(const T& rhs) { this->x += rhs; this->y += rhs; this->z += rhs; this->w += rhs; return *this; }
      base_vector_4<T>& operator*=(const base_vector_4<T>& rhs) { this->x *= rhs.x; this->y *= rhs.y; this->z *= rhs.z; this->w *= rhs.w; return *this; }
			base_vector_4<T>& operator*=(const T& rhs) { this->x *= rhs; this->y *= rhs; this->z *= rhs; this->w *= rhs; return *this; }
			base_vector_4<T>& operator/=(const base_vector_4<T>& rhs) {
        if (rhs.is_zero()) {
          this->zero();
          return *this;
        }; // per section 5.6 of c++ standard, division by zero is undefined.
        this->x /= rhs.x;
        this->y /= rhs.y;
        this->z /= rhs.z;
        this->w /= rhs.w;
        return *this;
			}
			base_vector_4<T>& operator/=(const T& rhs) {
        if (rhs == 0) {
          this->zero();
          return *this;
        }; // per section 5.6 of c++ standard, division by zero is undefined.
        this->x /= rhs;
        this->y /= rhs;
        this->z /= rhs;
        this->w /= rhs;
        return *this;
			}
		};

    template<class T> base_vector_4<T> operator+ (base_vector_4<T> lhs, const base_vector_4<T>& rhs) { lhs += rhs; return lhs; }
    template<class T> base_vector_4<T> operator- (base_vector_4<T> lhs, const base_vector_4<T>& rhs) { lhs -= rhs; return lhs; }
    template<class T> base_vector_4<T> operator- (base_vector_4<T> lhs, T rhs) { lhs -= rhs; return lhs; }
    template<class T> base_vector_4<T> operator- (float lhs, base_vector_4<T> rhs) { rhs -= lhs; return lhs; }
    template<class T> base_vector_4<T> operator* (base_vector_4<T> lhs, const base_vector_4<T>& rhs) { lhs *= rhs; return lhs; }
    template<class T> base_vector_4<T> operator/ (base_vector_4<T> lhs, const base_vector_4<T>& rhs) { lhs *= rhs; return lhs; }
    template<class T> base_vector_4<T> operator/ (base_vector_4<T> lhs, float rhs) { lhs *= rhs; return lhs; }


		#define  	vector_2i base_vector_2<int>
		#define  	vector_2f base_vector_2<float>

		#define 	vector_3i base_vector_3<int>
		#define 	vector_3f base_vector_3<float>

		#define  	vector_4i base_vector_4<int>
		#define  	vector_4f base_vector_4<float>

    typedef  base_vector_2<float> vec2;
    typedef  base_vector_2<int> ivec2;
    typedef  base_vector_2<unsigned> uvec2;

    typedef  base_vector_3<float> vec3;
    typedef  base_vector_3<int> ivec3;
    typedef  base_vector_3<unsigned> uvec3;
    typedef  base_vector_3<bool> bvec3;

    typedef  base_vector_4<float> vec4;
    typedef  base_vector_4<int> ivec4;
    typedef  base_vector_4<unsigned> uvec4;
	};
};

#endif
