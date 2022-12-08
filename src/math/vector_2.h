#ifndef USHINE_MATH_VECTOR_2
#define USHINE_MATH_VECTOR_2

#include "points.h"

namespace ushine::math {
  	template <class T> class base_vector_2 : public base_point_2<T> {
		public:
			base_point_2<T> origin;

			base_vector_2()
			{
				base_point_2<T>::x = 0;
				base_point_2<T>::y = 0;
			}

			base_vector_2(base_point_2<T>& p0, base_point_2<T> p1)
			{
				origin = p0;
				base_point_2<T>::x = p1.x - p0.x;
				base_point_2<T>::y = p1.y - p0.y;
			}
/*
			base_vector_2(base_vector_2<T>& v0)
			{
				origin = v0.origin;
				base_point_2<T>::x = v0.x;
				base_point_2<T>::y = v0.y;
			}
*/
			base_vector_2(T p0, T p1)
			{
				origin.zero();
				base_point_2<T>::x = p0;
				base_point_2<T>::y = p1;
			}

			void set(T x0=0, T y0=0)
			{
				origin.zero();
				base_point_2<T>::x = x0;
				base_point_2<T>::y = y0;
			}

      inline bool is_zero() const { return std::abs(this->x) <= std::numeric_limits<T>::epsilon() && std::abs(this->y) <= std::numeric_limits<T>::epsilon(); }
			inline bool non_zero() const { return !this->is_zero(); }

			inline T length() const
			{
				return std::sqrt(
					this->x*this->x +
					this->y*this->y
					);
			}

			void swap(base_vector_2<T> & right)
			{
				base_vector_2<T> temp = right;

				right.x = this->x;
				right.y = this->y;
				this->x = temp.x;
				this->y = temp.y;
			}

			base_vector_2<T> modulo(float moduloBy)
			{
				//x - y * floor(x/y).
				base_vector_2<T> out(
					this->x - moduloBy * floor(this->x/moduloBy),
					this->y - moduloBy * floor(this->y/moduloBy)
					);
				return out;
			}

			base_vector_2<T> abs()
			{
				//x - y * floor(x/y).
				base_vector_2<T> out(
					fabs(this->x),
					fabs(this->y)
					);
				return out;
			}

			base_vector_2<T> integral()
			{
				//x - y * floor(x/y).
				base_vector_2<T> out(
					std::floor(this->x),
					std::floor(this->y)
					);
				return out;
			}

			base_vector_2<T> fractional()
			{
				//x - y * floor(x/y).
				base_vector_2<T> out(
					this->x-std::floor(this->x),
					this->y-std::floor(this->y)
					);
				return out;
			}


			inline T dot(base_vector_2<T> right)
			{
				return this->x * right.x + this->y*right.y;
			}

			inline base_vector_2<T> interpolate_to( const base_vector_2<T>& dest,T alpha ) const
			{
				base_vector_2<T> out(
					(dest.x - this->x) * alpha,
					(dest.y - this->y) * alpha
					);
				return out;
			}


			inline T& operator[] (int i)
			{
				if (i==0)
					return this->x;
				if (i==1)
					return this->y;
				return this->x; //error
			}

			base_vector_2<T>& operator+=(const base_vector_2<T>& rhs) { this->x += rhs.x; this->y += rhs.y; return *this; }
			base_vector_2<T>& operator+=(T rhs) { this->x += rhs; this->y += rhs; return *this; }
			base_vector_2<T>& operator-=(const base_vector_2<T>& rhs) { this->x -= rhs.x; this->y -= rhs.y; return *this; }
			base_vector_2<T>& operator-=(T rhs) { this->x -= rhs; this->y -= rhs; return *this; }
			base_vector_2<T>& operator*=(const base_vector_2<T>& rhs) { this->x *= rhs.x; this->y *= rhs.y; return *this; }
			base_vector_2<T>& operator*=(const T& rhs) { this->x *= rhs; this->y *= rhs; return *this; }
			base_vector_2<T>& operator/=(const base_vector_2<T>& rhs) {
        // if rhs is zero, clamp it to near zero.
        if (std::abs(rhs.x) <= std::numeric_limits<T>::epsilon()) {
          this->x /= std::copysign(std::numeric_limits<T>::epsilon(), rhs.x);
        } else {
          this->x /= rhs.x;
        }
        if (std::abs(rhs.y) <= std::numeric_limits<T>::epsilon()) {
          this->y /= std::copysign(std::numeric_limits<T>::epsilon(), rhs.y);
        } else {
          this->y /= rhs.y;
        }
        return *this;
			}
			base_vector_2<T>& operator/=(const T& rhs) {
        // if rhs is zero, clamp it to near zero.
        if (std::abs(rhs) <= std::numeric_limits<T>::epsilon()) {
          const auto rhs_ = std::copysign(std::numeric_limits<T>::epsilon(), rhs);
          this->x /= rhs_;
          this->y /= rhs_;
        } else {
          this->x /= rhs;
          this->y /= rhs;
        }
        return *this;
			}
		};


    template<class T> base_vector_2<T> operator- (const base_vector_2<T>& v) { return base_vector_2<T>(-v.x, -v.y); }
    template<class T> base_vector_2<T> operator+ (base_vector_2<T> lhs, const base_vector_2<T>& rhs) { lhs += rhs; return lhs; }
    template<class T> base_vector_2<T> operator+ (base_vector_2<T> lhs, T rhs) { lhs += rhs; return lhs; }
    template<class T> base_vector_2<T> operator+ (T lhs, base_vector_2<T> rhs) { rhs += lhs; return rhs; }
    template<class T> base_vector_2<T> operator- (base_vector_2<T> lhs, const base_vector_2<T>& rhs) { lhs -= rhs; return lhs; }
    template<class T> base_vector_2<T> operator- (base_vector_2<T> lhs, T rhs) { lhs -= rhs; return lhs; }

    template<class T> base_vector_2<T> operator- (T lhs,base_vector_2<T> rhs)
		{ 
			base_vector_2<T> out;

			out.x=lhs-rhs.x;
			out.y=lhs-rhs.y;
			
			return out; 
		}

    template<class T> base_vector_2<T> operator* (base_vector_2<T> lhs, const base_vector_2<T>& rhs) { lhs *= rhs; return lhs; }
    template<class T> base_vector_2<T> operator* (base_vector_2<T> lhs, const T& rhs) { lhs *= rhs; return lhs; }
    template<class T> base_vector_2<T> operator* (const T& lhs, const base_vector_2<T>& rhs) { return rhs * lhs; }
    template<class T> base_vector_2<T> operator/ (base_vector_2<T> lhs, const base_vector_2<T>& rhs) { lhs /= rhs; return lhs; }
    template<class T> base_vector_2<T> operator/ (base_vector_2<T> lhs, const T& rhs) { lhs /= rhs; return lhs; }
    template<class T> base_vector_2<T> operator/ (T lhs,base_vector_2<T> rhs) { 
			base_vector_2<T> out;
      // if rhs is zero, clamp it to near zero.
      if (std::abs(rhs.x) <= std::numeric_limits<T>::epsilon()) {
        out.x = lhs / std::copysign(std::numeric_limits<T>::epsilon(), rhs.x);
      } else {
        out.x = lhs / rhs.x;
      }
      if (std::abs(rhs.y) <= std::numeric_limits<T>::epsilon()) {
        out.y = lhs / std::copysign(std::numeric_limits<T>::epsilon(), rhs.y);
      } else {
        out.y = lhs / rhs.y;
      }
			return out; 
		}
}

#endif