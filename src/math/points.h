#ifndef USHINE_MATH_POINTS
#define USHINE_MATH_POINTS


namespace ushine
{

	namespace math
	{

		/*
		forward declarations of all important items
		*/
		class point_2i;
		class point_2f;
		class point_3i;
		class point_3f;

		/*
		templated definitions for baseline points (up to 4 dimensions)
		*/
		template <class T> class base_point_2
		{
		public:		
			base_point_2(T x0=0, T y0=0) : x(x0), y(y0) {}

			T x, y;
			void zero() { x=y=0; }

		};

		template <class T> class base_point_3
		{
		public:		
			base_point_3(T x0=0, T y0=0, T z0=0) : x(x0), y(y0), z(z0) {}
			T x, y, z;
			void zero() { x=y=z=0; }
		};		

		template <class T> class base_point_4
		{
		public:		
			base_point_4(T x0=0, T y0=0, T z0=0, T w0=0) : x(x0), y(y0), z(z0), w(w0) {}
			T x, y, z, w;
			void zero() { x=y=z=w=0; }			
		};		

		class point_2i : public base_point_2<int> {
		public:
		};
		class point_2f : public base_point_2<double> {
		public:
		};
		class point_3i : public base_point_3<int> {
		public:
		};
		class point_3f : public base_point_3<double> {
		public:
		};	
		class point_4i : public base_point_4<int> {
		public:
		};
		class point_4f : public base_point_4<double> {
		public:
		};	

};
};

#endif