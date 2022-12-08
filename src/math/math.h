

#ifndef USHINE_MATH
#define USHINE_MATH

#include <cmath>

#include "constants.h"
#include "points.h"
#include "vectors.h"
#include "vector_math.h"
#include "matrices.h"

namespace ushine
{

	namespace math
	{

		template<class T> T to_radians(T angle)
		{
			return angle * pi / 180;
		}

		template<class T> T clamp(T x, T min, T max)
		{   
		    if (x < min) 
		        x = min;
		    else if (x > max) 
		        x = max;
		    
		    return x;
		}

		template<class T> T smoothstep(T edge0, T edge1, T x)
		{
		    // Scale, bias and saturate x to 0..1 range
		    x = clamp<T>((x - edge0)/(edge1 - edge0), 0.0, 1.0); 
		    // Evaluate polynomial
		    return x*x*(3 - 2*x);
		}

    	template<class T> base_vector_2<T> smoothstep(T edge0, T edge1, const base_vector_2<T>& v) 
		{ return base_vector_2<T>(smoothstep(edge0, edge1, v.x), smoothstep(edge0, edge1, v.y)); } 

		template<class T> T modulo(T base, T mod)
		{
		    return base - mod * std::floor(base/mod);
		}

		template<class T> T sign(T base)
		{
			if (base < 0.0)
				return -1.0;
			if (base == 0.0)
				return 0.0;
			return 1.0;
		}

		template<class T> T mix(T x, T y, T a)
		{
			return x*(1.0-a)+y*a;
		}

		template<class T> base_vector_3<T> mix(base_vector_3<T> x, base_vector_3<T> y, float a) { 
		return base_vector_3<T>(
			mix(x.x, y.x, a),
			mix(x.y, y.y, a),
			mix(x.z, y.z, a)
		);
		} 
		

	};
};



#endif