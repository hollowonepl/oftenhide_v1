#ifndef USHINE_MATH_VECTOR_3
#define USHINE_MATH_VECTOR_3

namespace ushine::math {
  	template <class T> class base_vector_3 : public base_point_3<T> {
		public:
			base_point_3<T> origin;

			base_vector_3() {}

      base_vector_3(T v) : base_vector_3(v, v, v) {}

			base_vector_3(base_point_3<T>& p0, base_point_3<T> p1)
			{
				origin = p0;
				base_point_3<T>::x = p1.x - p0.x;
				base_point_3<T>::y = p1.y - p0.y;
				base_point_3<T>::z = p1.z - p0.z;
			}
			base_vector_3(base_point_3<T> p1)
			{
				origin.zero();
				base_point_3<T>::x = p1.x;
				base_point_3<T>::y = p1.y;
				base_point_3<T>::z = p1.z;
			}
			base_vector_3(T p0, T p1, T p2)
			{
				origin.zero();
				base_point_3<T>::x = p0;
				base_point_3<T>::y = p1;
				base_point_3<T>::z = p2;
			}

			void set(base_point_3<T> p1)
			{
				origin.zero();
				base_point_3<T>::x = p1.x;
				base_point_3<T>::y = p1.y;
				base_point_3<T>::z = p1.z;
			}
			void set(T x0=0, T y0=0, T z0=0)
			{
				origin.zero();
				base_point_3<T>::x = x0;
				base_point_3<T>::y = y0;
				base_point_3<T>::z = z0;
			}

			inline base_vector_2<T> xy()
			{
				base_vector_2<T> out;
				out.x = this->x;
				out.y = this->y;
				return out;
			}
			inline void xy(const base_vector_2<T>& v)
			{
				this->x = v.x;
				this->y = v.y;
			}
			inline base_vector_2<T> xz()
			{
				base_vector_2<T> out;
				out.x = this->x;
				out.y = this->z;
				return out;
			}
      inline base_vector_2<T> zx()
			{
				base_vector_2<T> out;
				out.x = this->z;
				out.y = this->x;
				return out;
			}
			inline void xz(const base_vector_2<T>& v)
			{
				this->x = v.x;
				this->z = v.y;
			}
			inline base_vector_2<T> yz()
			{
				base_vector_2<T> out;
				out.x = this->y;
				out.y = this->z;
				return out;
			}

			void swap(base_vector_3<T> & right)
			{
				base_vector_3<T> temp = right;

				right.x = this->x;
				right.y = this->y;
				right.z = this->z;
				this->x = temp.x;
				this->y = temp.y;
				this->z = temp.z;
			}

      inline bool is_zero() const { return std::abs(this->x) <= std::numeric_limits<T>::epsilon() && std::abs(this->y) <= std::numeric_limits<T>::epsilon() && std::abs(this->z) <= std::numeric_limits<T>::epsilon(); }
			inline bool non_zero()
			{
				return base_point_3<T>::x != 0 && base_point_3<T>::y != 0 && base_point_3<T>::z != 0;
			}
			inline T length()
			{
				return sqrt(
					base_point_3<T>::x*base_point_3<T>::x +
					base_point_3<T>::y*base_point_3<T>::y +
					base_point_3<T>::z*base_point_3<T>::z
					);
			}

			inline base_vector_3<T> normalize()
			{
				T len = length();
				base_vector_3<T> normalized_v;

				if (len!=0)
				{
					normalized_v.x = this->x/len;
					normalized_v.y = this->y/len;
					normalized_v.z = this->z/len;
				}

				return normalized_v;
			}

			inline base_vector_3<T> reflect(base_vector_3<T> i, base_vector_3<T> n)
			{
				//I - 2.0 * dot(N, I) * N.
				float ri = 2.0 * n.dot(i);
				return	(i - ri*n);
			}

			inline base_vector_3<T> modulo(float moduloBy)
			{
				//x - y * floor(x/y).
				base_vector_3<T> out(
					this->x - moduloBy * floor(this->x/moduloBy),
					this->y - moduloBy * floor(this->y/moduloBy),
					this->z - moduloBy * floor(this->z/moduloBy)
					);
				return out;
			}

			base_vector_3<T> abs()
			{
				//x - y * floor(x/y).
				base_vector_3<T> out(
					fabs(this->x),
					fabs(this->y),
					fabs(this->z)
					);
				return out;
			}

			base_vector_3<T> integral()
			{
				base_vector_3<T> out(
					floor(this->x),
					floor(this->y),
					floor(this->z)
					);
				return out;
			}

			base_vector_3<T> fractional()
			{
				//x - y * floor(x/y).
				base_vector_3<T> out(
					this->x-floor(this->x),
					this->x-floor(this->y),
					this->x-floor(this->z)
					);
				return out;
			}

			inline T dot(base_vector_3<T> right)
			{
				return this->x * right.x + this->y*right.y + this->z*right.z;
			}

			inline base_vector_3<T> cross(base_vector_3<T> right)
			{
				base_vector_3<T> out;

				out.x = this->y * right.z - right.y * this->z;
				out.y = this->z * right.x - right.z * this->x;
				out.z = this->x * right.y - right.x * this->y;

				return out;
			}

			inline base_vector_3<T> interpolate_to( const base_vector_3<T>& dest,T alpha ) const
			{
				base_vector_3<T> out;

				out.x = (dest.x - this->x) * alpha;
				out.y = (dest.y - this->y) * alpha;
				out.z = (dest.z - this->z) * alpha;

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
				return this->x; //error
			}

    	base_vector_3<T>& operator+=(const base_vector_3<T>& rhs) { this->x += rhs.x; this->y += rhs.y; this->z += rhs.z; return *this; }
			base_vector_3<T>& operator+=(const T& rhs) { this->x += rhs; this->y += rhs; this->z += rhs; return *this; }
			base_vector_3<T>& operator-=(const base_vector_3<T>& rhs) { this->x -= rhs.x; this->y -= rhs.y; this->z -= rhs.z; return *this; }
			base_vector_3<T>& operator-=(T& rhs) { this->x -= rhs; this->y -= rhs; this->z -= rhs; return *this; }
			base_vector_3<T>& operator*=(const base_vector_3<T>& rhs) { this->x *= rhs.x; this->y *= rhs.y; this->z *= rhs.z; return *this; }
			base_vector_3<T>& operator*=(const T& rhs) { this->x *= rhs; this->y *= rhs; this->z *= rhs; return *this; }
			base_vector_3<T>& operator/=(const base_vector_3<T>& rhs) {
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
        if (std::abs(rhs.z) <= std::numeric_limits<T>::epsilon()) {
          this->z /= std::copysign(std::numeric_limits<T>::epsilon(), rhs.z);
        } else {
          this->z /= rhs.z;
        }
        return *this;
			}
			base_vector_3<T>& operator/=(const T& rhs) {
        // if rhs is zero, clamp it to near zero.
        if (std::abs(rhs) <= std::numeric_limits<T>::epsilon()) {
          const auto rhs_ = std::copysign(std::numeric_limits<T>::epsilon(), rhs);
          this->x /= rhs_;
          this->y /= rhs_;
          this->z /= rhs_;
        } else {
          this->x /= rhs;
          this->y /= rhs;
          this->z /= rhs;
        }
        return *this;
			}
		};


    template<class T> base_vector_3<T> operator- (const base_vector_3<T>& v) { return base_vector_3<T>(-v.x, -v.y, -v.z); }
    template<class T> base_vector_3<T> operator+ (base_vector_3<T> lhs, const base_vector_3<T>& rhs) { lhs += rhs; return lhs; }
    template<class T> base_vector_3<T> operator+ (base_vector_3<T> lhs, const T& rhs) { lhs += rhs; return lhs; }
    template<class T> base_vector_3<T> operator+ (const T& lhs, base_vector_3<T> rhs) { rhs += lhs; return rhs; }
    template<class T> base_vector_3<T> operator- (base_vector_3<T> lhs, const base_vector_3<T>& rhs) { lhs -= rhs; return lhs; }
    template<class T> base_vector_3<T> operator- (base_vector_3<T> lhs, T& rhs) { lhs -= rhs; return lhs; }
    template<class T> base_vector_3<T> operator- (float lhs, base_vector_3<T> rhs) { 
      base_vector_3<T> out;

			out.x = lhs-rhs.x;
			out.y = lhs-rhs.y;
      out.z = lhs-rhs.z;

			return out; 
    }
    template<class T> base_vector_3<T> operator* (base_vector_3<T> lhs, const base_vector_3<T>& rhs) { lhs *= rhs; return lhs; }
    template<class T> base_vector_3<T> operator* (base_vector_3<T> lhs, const T& rhs) { lhs *= rhs; return lhs; }
    template<class T> base_vector_3<T> operator* (const T& lhs, const base_vector_3<T>& rhs) { return rhs * lhs; }
    template<class T> base_vector_3<T> operator/ (base_vector_3<T> lhs, const base_vector_3<T>& rhs) { lhs /= rhs; return lhs; }
    template<class T> base_vector_3<T> operator/ (base_vector_3<T> lhs, const T& rhs) { lhs /= rhs; return lhs; }
    template<class T> base_vector_3<T> operator/ (float lhs, base_vector_3<T> rhs) { 
			base_vector_3<T> out;
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
      if (std::abs(rhs.z) <= std::numeric_limits<T>::epsilon()) {
        out.z = lhs / std::copysign(std::numeric_limits<T>::epsilon(), rhs.z);
      } else {
        out.z = lhs / rhs.z;
      }
			return out; 
    }
}

#endif