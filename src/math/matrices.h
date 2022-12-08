#ifndef USHINE_MATH_MATRICES
#define USHINE_MATH_MATRICES

#include <cmath>

namespace ushine::math {
		enum matrix_index_2
		{
			mi2_i1 = 0,
			mi2_i2 = 1
		};


		template <class T> class base_matrix_2
		{
		public:

			base_vector_2<T> row1;
			base_vector_2<T> row2;

			base_matrix_2()
			{
				zero();
			}

			base_matrix_2(base_vector_2<T> l, base_vector_2<T> r)
			{
				//std::cout << l.x << "+" << l.y << std::endl;
				row1 = l;
				row2 = r;
			}

			inline void zero()
			{
				row1.set(0,0);
				row2.set(0,0);
			}

			inline void identity()
			{
				row1.set(1,0);
				row2.set(0,1);
			}

			inline base_matrix_2<T> modulo(float moduloBy)
			{
				base_matrix_2<T> out(
					row1.modulo(moduloBy),
					row2.modulo(moduloBy)
					);
				return out;
			}

			inline base_matrix_2<T> abs()
			{
				base_matrix_2<T> out(
					row1.abs(),
					row2.abs()
					);
				return out;
			}

			inline base_matrix_2<T> integral()
			{
				base_matrix_2<T> out(
					row1.integral(),
					row2.integral()
					);
				return out;
			}

			inline base_matrix_2<T> fractional()
			{
				base_matrix_2<T> out(
					row1.fractional(),
					row2.fractional()
					);
				return out;
			}

		    class index_proxy {
		    public:
		        index_proxy(base_vector_2<T>& column) : item(column) { }

		        T& operator[](int index) {
		            return item[index];
		        }
		    private:
		        base_vector_2<T>& item;
		    };

			inline index_proxy operator[] (int i)
			{
				if (i==0)
					return index_proxy(row1);
				if (i==1)
					return index_proxy(row2);
				return index_proxy(row1); //error
			}

			friend base_matrix_2<T> operator+ (base_matrix_2<T> l, base_matrix_2<T> r)
			{
				base_matrix_2<T> out(
					l.row1+r.row1,
					l.row2+r.row2
					);
				return out;
			}

			static base_matrix_2<T> rotate(float angle)
			{
				ushine::math::base_matrix_2<T> rotation;

				rotation.row1.set(std::cos(angle),-std::sin(angle));
				rotation.row2.set(std::sin(angle),std::cos(angle));
				return rotation;
			}

			static base_matrix_2<T> scale(float size_x, float size_y)
			{
				ushine::math::base_matrix_2<T> scale;

				scale.row1.set(size_x,0);
				scale.row2.set(0,size_y);

				return scale;
			}

			friend base_matrix_2<T> operator- (base_matrix_2<T>& left, base_matrix_2<T>& right)
			{
				base_matrix_2<T> out(
					left.row1-right.row1,
					left.row2-right.row2
					);
				return out;
			}
			friend base_matrix_2<T> operator-= (base_matrix_2<T> l, base_matrix_2<T> r)
			{
				return l-r;
			}

			friend base_matrix_2<T> operator* (base_matrix_2<T> left, base_matrix_2<T> right)
			{
				base_matrix_2<T> out(
					base_vector_2<T>(
					left[0][0]*right[0][0] + left[0][1]*right[1][0], left[0][0]*right[0][1] + left[0][1]*right[1][1]),
					base_vector_2<T>(
					left[1][0]*right[0][0] + left[1][1]*right[1][0], left[1][0]*right[0][1] + left[1][1]*right[1][1])
					);
				return out;
			}
			friend base_matrix_2<T> operator* (base_matrix_2<T> m, T constant)
			{
				base_matrix_2<T> out(m.row1*constant, m.row2*constant);
				return out;
			}

			friend base_matrix_2<T> operator* (T constant, base_matrix_2<T> m)
			{
				base_matrix_2<T> out(constant*m.row1, constant*m.row2);
				return out;
			}


			friend base_vector_2<T> operator* (base_matrix_2<T> m, base_vector_2<T> v)
			{
				base_vector_2<T> out;

				out.set(
					m[0][0]*v[0]+m[0][1]*v[1],
					m[1][0]*v[0]+m[1][1]*v[1]
					);
				return out;
			}
			friend base_vector_2<T> operator* (base_vector_2<T> v, base_matrix_2<T> m)
			{
				base_vector_2<T> out;

				out.set(
					m[0][0]*v[0]+m[0][1]*v[1],
					m[1][0]*v[0]+m[1][1]*v[1]
					);
				return out;
			}


			friend base_matrix_2<T> operator*= (base_matrix_2<T>& left, base_matrix_2<T>& right)
			{
				return left*right;
			}
			friend base_matrix_2<T> operator*=(base_matrix_2<T>& left, T constant)
			{
				return left*constant;
			}

			inline base_matrix_2<T> transpose()
			{
				base_matrix_2<T> out;
				base_matrix_2<T>& src = *this;

				out = src;
				out[0][1] = src[1][0];
				out[1][0] = src[0][1];

				return out;
			}

			inline base_matrix_2<T> inverse()
			{
				base_matrix_2<T>& src = *this;
				base_matrix_2<T> out = src;

				out[1][0]*=-1;
				out[0][1]*=-1;

				T determinant = (src[0][0]*src[1][1] - src[1][0]*src[0][1]);

				if (determinant!=0)
					return 1/determinant * out;
				else {
					out.zero();
					return out;
				}
			}
		}; //class base_matrix_2<T>

		template <class T> class base_matrix_3
		{
		public:
			base_vector_3<T> c[3];

			base_matrix_3() {
				c[0] = vec3(0.0f, 0.0f, 0.0f);
				c[1] = vec3(0.0f, 0.0f, 0.0f);
				c[2] = vec3(0.0f, 0.0f, 0.0f);        
			}

			inline void zero()
			{
				c[0]=c[1]=c[2]=0;
			}

			base_matrix_3(base_vector_3<T> c1, base_vector_3<T> c2, base_vector_3<T> c3) {
						c[0] = c1;
						c[1] = c2;
						c[2] = c3;
					}
			base_matrix_3(T c11, T c12, T c13, T c21, T c22, T c23, T c31, T c32, T c33) {
						c[0] = base_vector_3<T>(c11, c12, c13);
						c[1] = base_vector_3<T>(c21, c22, c23);
						c[2] = base_vector_3<T>(c31, c32, c33);
					}
    	};

		template <class T> class base_matrix_22
		{
		public:
			base_vector_2<T> c[2];

			base_matrix_22() {
				c[0] = base_vector_2<T>(0.0f, 0.0f);
				c[1] = base_vector_2<T>(0.0f, 0.0f);
			}

			inline void zero()
			{
				c[0]=c[1]=0;
			}

			base_matrix_22(base_vector_2<T> c1, base_vector_2<T> c2) {
						c[0] = c1;
						c[1] = c2;
					}
			base_matrix_22(T c11, T c12, T c21, T c22) {
						c[0] = base_vector_2<T>(c11, c12);
						c[1] = base_vector_2<T>(c21, c22);
					}
    	};

		template <class T> class base_matrix_4
		{
		public:

			T matData[4][4];
			base_vector_4<T> row1, row2, row3, row4;

			base_matrix_4()
			{
				zero();
			}

			base_matrix_4(base_matrix_4<T>& source)
			{
				row1 = source.row1;
				row2 = source.row2;
				row3 = source.row3;
				row4 = source.row4;				
			}			

			base_matrix_4(base_vector_4<T> r1, base_vector_4<T> r2, base_vector_4<T> r3, base_vector_4<T> r4)
			{
				row1 = r1;
				row2 = r2;
				row3 = r3;
				row4 = r4;
			}

			inline void zero()
			{
				row1.set(0,0,0,0);
				row2.set(0,0,0,0);
				row3.set(0,0,0,0);
				row4.set(0,0,0,0);
			}

			inline void identity()
			{
				row1.set(1,0,0,0);
				row2.set(0,1,0,0);
				row3.set(0,0,1,0);
				row4.set(0,0,0,1);
			}

			inline base_matrix_4<T> modulo(float moduloBy)
			{
				base_matrix_4<T> out(
					row1.modulo(moduloBy),
					row2.modulo(moduloBy),
					row3.modulo(moduloBy),
					row4.modulo(moduloBy)
					);
				return out;
			}

			inline base_matrix_4<T> abs()
			{
				base_matrix_4<T> out(
					row1.abs(),
					row2.abs(),
					row3.abs(),
					row4.abs()
					);
				return out;
			}

			inline base_matrix_4<T> integral()
			{
				base_matrix_4<T> out(
					row1.integral(),
					row2.integral(),
					row3.integral(),
					row4.integral()
					);
				return out;
			}

			inline base_matrix_4<T> fractional()
			{
				base_matrix_4<T> out(
					row1.fractional(),
					row2.fractional(),
					row3.fractional(),
					row4.fractional()
					);
				return out;
			}

		    class index_proxy {
		    public:
		        index_proxy(base_vector_4<T>& row) : item(row) { }

		        T& operator[](int index) {
		            return item[index];
		        }
				operator base_vector_4<T>()
		        {
		        	return item;
		        }

		    private:
		        base_vector_4<T>& item;
		    };

			inline index_proxy operator[] (int i)
			{
				if (i==0)
					return index_proxy(row1);
				if (i==1)
					return index_proxy(row2);
				if (i==2)
					return index_proxy(row3);
				if (i==3)
					return index_proxy(row4);
				return index_proxy(row1); //error
			}

			friend base_matrix_4<T> operator+ (base_matrix_4<T> left, base_matrix_4<T> right)
			{
				base_matrix_4<T> out(
					left.row1+right.row1,
					left.row2+right.row2,
					left.row3+right.row3,
					left.row4+right.row4
					);
				return out;
			}

			/*
			friend base_matrix_4<T> operator+= (base_matrix_4<T> left, base_matrix_4<T> right)
			{
				std::cout<<"i'm here" << std::endl;
				base_matrix_4<T> out(
					left.row1+right.row1,
					left.row2+right.row2,
					left.row3+right.row3,
					left.row4+right.row4
					);
				return out;
			}
			*/

			friend base_matrix_4<T> operator- (base_matrix_4<T> left, base_matrix_4<T> right)
			{
				base_matrix_4<T> out(
					left.row1-right.row1,
					left.row2-right.row2,
					left.row3-right.row3,
					left.row4-right.row4
					);
				return out;
			}


			friend base_matrix_4<T> operator* (base_matrix_4<T> left, base_matrix_4<T> right)
			{
				base_matrix_4<T> out;

				out[0][0] = left[0][0] * right[0][0] +
						    left[0][1] * right[1][0] +
						    left[0][2] * right[2][0] +
						    left[0][3] * right[3][0];
				out[1][0] = left[1][0] * right[0][0] +
						    left[1][1] * right[1][0] +
						    left[1][2] * right[2][0] +
						    left[1][3] * right[3][0];
				out[2][0] = left[2][0] * right[0][0] +
						    left[2][1] * right[1][0] +
						    left[2][2] * right[2][0] +
						    left[2][3] * right[3][0];
				out[3][0] = left[3][0] * right[0][0] +
						    left[3][1] * right[1][0] +
						    left[3][2] * right[2][0] +
						    left[3][3] * right[3][0];

				out[0][1] = left[0][0] * right[0][1] +
						    left[0][1] * right[1][1] +
						    left[0][2] * right[2][1] +
						    left[0][3] * right[3][1];
				out[1][1] = left[1][0] * right[0][1] +
						    left[1][1] * right[1][1] +
						    left[1][2] * right[2][1] +
						    left[1][3] * right[3][1];
				out[2][1] = left[2][0] * right[0][1] +
						    left[2][1] * right[1][1] +
						    left[2][2] * right[2][1] +
						    left[2][3] * right[3][1];
				out[3][1] = left[3][0] * right[0][1] +
						    left[3][1] * right[1][1] +
						    left[3][2] * right[2][1] +
						    left[3][3] * right[3][1];

				out[0][2] = left[0][0] * right[0][2] +
						    left[0][1] * right[1][2] +
						    left[0][2] * right[2][2] +
						    left[0][3] * right[3][2];
				out[1][2] = left[1][0] * right[0][2] +
						    left[1][1] * right[1][2] +
						    left[1][2] * right[2][2] +
						    left[1][3] * right[3][2];
				out[2][2] = left[2][0] * right[0][2] +
						    left[2][1] * right[1][2] +
						    left[2][2] * right[2][2] +
						    left[2][3] * right[3][2];
				out[3][2] = left[3][0] * right[0][2] +
						    left[3][1] * right[1][2] +
						    left[3][2] * right[2][2] +
						    left[3][3] * right[3][2];

				out[0][3] = left[0][0] * right[0][3] +
						    left[0][1] * right[1][3] +
						    left[0][2] * right[2][3] +
						    left[0][3] * right[3][3];
				out[1][3] = left[1][0] * right[0][3] +
						    left[1][1] * right[1][3] +
						    left[1][2] * right[2][3] +
						    left[1][3] * right[3][3];
				out[2][3] = left[2][0] * right[0][3] +
						    left[2][1] * right[1][3] +
						    left[2][2] * right[2][3] +
						    left[2][3] * right[3][3];
				out[3][3] = left[3][0] * right[0][3] +
						    left[3][1] * right[1][3] +
						    left[3][2] * right[2][3] +
						    left[3][3] * right[3][3];

				return out;
			}


			friend base_matrix_4<T> operator* (base_matrix_4<T> m, T constant)
			{
				base_matrix_4<T> out(m.row1*constant, m.row2*constant, m.row3*constant, m.row4*constant);
				return out;
			}

			friend base_matrix_4<T> operator* (T constant, base_matrix_4<T> m)
			{
				//base_matrix_4<T> out(m.row1*constant, m.row2*constant, m.row3*constant, m.row4*constant);
				return m*constant;
			}

			friend base_vector_4<T> operator* (base_matrix_4<T> m, base_vector_4<T> v)
			{
				base_vector_4<T> out;

				T rowmath[4] = {0,0,0,0};

				for (auto i=0; i<4; i++)
					for (auto j=0; j<4; j++)
					rowmath[i]+= m[i][j]*v[j];

				return base_vector_4<T>(rowmath[0],rowmath[1],rowmath[2],rowmath[3]);


			}

			friend base_vector_4<T> operator* (base_vector_4<T> v, base_matrix_4<T> m)
			{
				return m*v;
			}

			inline base_matrix_4<T> transpose()
			{
				base_matrix_4<T> out;
				base_matrix_4<T>& src = *this;

				out[0][0] = src[0][0];
				out[1][0] = src[0][1];
				out[2][0] = src[0][2];
				out[3][0] = src[0][3];

				out[0][1] = src[1][0];
				out[1][1] = src[1][1];
				out[2][1] = src[1][2];
				out[3][1] = src[1][3];

				out[0][2] = src[2][0];
				out[1][2] = src[2][1];
				out[2][2] = src[2][2];
				out[3][2] = src[2][3];

				out[0][3] = src[3][0];
				out[1][3] = src[3][1];
				out[2][3] = src[3][2];
				out[3][3] = src[3][3];

				return out;
			}

			/*
			inline base_matrix_2<T> inverse()
			{
				base_matrix_2<T>& src = *this;
				base_matrix_2<T> out = src;

				out[1][0]*=-1;
				out[0][1]*=-1;

				T determinant = (src[0][0]*src[1][1] - src[1][0]*src[0][1]);

				if (determinant!=0)
					return 1/determinant * out;
				else {
					out.zero();
					return out;
				}
			}
			*/

		};


  typedef  base_matrix_3<float> mat3;
  typedef  base_matrix_22<float> mat2;

  template<class T> base_vector_3<T> operator* (const base_matrix_3<T>& m, const base_vector_3<T>& v) {
    T r[3];

    for (size_t i = 0; i < 3; i++) {
       r[i] = m.c[i].x * v.x + m.c[i].y * v.y + m.c[i].z * v.z;
    }

    return base_vector_3<T>(r[0], r[1], r[2]);
  }

  template<class T> base_vector_3<T> operator* (const base_vector_3<T>& v, const base_matrix_3<T>& m) {
    T r[3];

    for (size_t i = 0; i < 3; i++) {
       r[i] = m.c[i].x * v.x + m.c[i].y * v.y + m.c[i].z * v.z;
    }

    return base_vector_3<T>(r[0], r[1], r[2]);
  }

  template<class T> base_vector_2<T> operator* (const base_matrix_22<T>& m, const base_vector_2<T>& v) {
    T r[3];

    for (size_t i = 0; i < 2; i++) {
       r[i] = m.c[i].x * v.x + m.c[i].y * v.y;
    }

    return base_vector_2<T>(r[0], r[1]);
  }

//added by h1: unj: check if I follow your code style and logic
  template<class T> base_matrix_22<T> operator* (const base_matrix_22<T>& m, const T c) {
    
	base_matrix_22<T> r;

    for (size_t i = 0; i < 2; i++) {
       r.c[i]*=c;
    }

    return r;
  }


};

#endif
