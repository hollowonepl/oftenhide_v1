#ifndef USHINE_MATH_VECTOR_MATH
#define USHINE_MATH_VECTOR_MATH

#include "vectors.h"
#include <cmath>

namespace ushine::math {
  template<class T> T distance(const base_vector_2<T>& u, const base_vector_2<T>& v) { return std::sqrt((u.x-v.x)*(u.x-v.x) + (u.y-v.y)*(u.y-v.y)); }
  template<class T> T distance_sq(const base_vector_2<T>& u, const base_vector_2<T>& v) { return (u.x-v.x)*(u.x-v.x) + (u.y-v.y)*(u.y-v.y); }

  template<class T> base_vector_2<T> cos(base_vector_2<T> v) { return base_vector_2<T>(std::cos(v.x), std::cos(v.y)); }
  template<class T> base_vector_3<T> cos(base_vector_3<T> v) { return base_vector_3<T>(std::cos(v.x), std::cos(v.y), std::cos(v.z)); }
  template<class T> base_vector_2<T> sin(base_vector_2<T> v) { return base_vector_2<T>(std::sin(v.x), std::sin(v.y)); }
  template<class T> base_vector_3<T> sin(base_vector_3<T> v) { return base_vector_3<T>(std::sin(v.x), std::sin(v.y), std::sin(v.z)); }
  template<class T> base_vector_3<T> sqrt(base_vector_3<T> v) { return base_vector_3<T>(std::sqrt(v.x), std::sqrt(v.y), std::sqrt(v.z)); }
  template<class T> base_vector_2<T> min(base_vector_2<T> v, T a) {
    return base_vector_2<T>(
      v.x < a ? a : v.x,
      v.y < a ? a : v.y
    );
  }
  template<class T> base_vector_3<T> min(base_vector_3<T> v, T a) {
    return base_vector_3<T>(
      v.x < a ? a : v.x,
      v.y < a ? a : v.y,
      v.z < a ? a : v.z
    );
  }
  template<class T> T min(T v, T a) {
    return v < a ? a : v;
  }

  template<class T> base_vector_2<T> max(base_vector_2<T> v, T a) {
    return base_vector_2<T>(
      v.x > a ? a : v.x,
      v.y > a ? a : v.y
    );
  }
  template<class T> base_vector_3<T> max(base_vector_3<T> v, T a) {
    return base_vector_3<T>(
      v.x > a ? a : v.x,
      v.y > a ? a : v.y,
      v.z > a ? a : v.z
    );
  }
  template<class T> base_vector_4<T> max(base_vector_4<T> v, T a) {
    return base_vector_4<T>(
      v.x > a ? a : v.x,
      v.y > a ? a : v.y,
      v.z > a ? a : v.z,
      v.w > a ? a : v.w
    );
  }
  template<class T> base_vector_4<T> max(base_vector_4<T> v, base_vector_4<T> a) {
    return base_vector_4<T>(
      v.x > a.x ? a.x : v.x,
      v.y > a.y ? a.y : v.y,
      v.z > a.z ? a.z : v.z,
      v.w > a.w ? a.w : v.w
    );
  }
  template<class T> T max(T v, T a) {
    return v > a ? a : v;
  }


  template<class T> base_vector_2<T> clamp(base_vector_2<T> v, T _min, T _max) { return min(max(v, _max), _min); }
  template<class T> base_vector_3<T> clamp(base_vector_3<T> v, T _min, T _max) { return min(max(v, _max), _min); }
  template<class T> T dot(base_vector_2<T> r, base_vector_2<T> l) { return r.x*l.x + r.y*l.y; }
  template<class T> T dot(base_vector_3<T> r, base_vector_3<T> l) { return r.x*l.x + r.y*l.y + r.z*l.z; }
  template<class T> base_vector_3<T> cross(base_vector_3<T> x, base_vector_3<T> y) { 
    return base_vector_3<T>(
      x.y*y.z - y.y*x.z,
      x.z*y.x - y.z*x.x,
      x.x*y.y - y.x*x.y
    );
  }
  template<class T> base_vector_2<T> floor(base_vector_2<T> v) { return base_vector_2<T>(std::floor(v.x), std::floor(v.y)); }
  template<class T> base_vector_3<T> floor(base_vector_3<T> v) { return base_vector_3<T>(std::floor(v.x), std::floor(v.y), std::floor(v.z)); }
  template<class T> base_vector_2<T> normalize(const base_vector_2<T>& v) {
    float length_of_v = std::sqrt((v.x * v.x) + (v.y * v.y));
    return base_vector_2<T>(v.x / length_of_v, v.y / length_of_v);
  }
  template<class T> base_vector_3<T> normalize(const base_vector_3<T>& v) {
    float length_of_v = std::sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
    return base_vector_3<T>(v.x / length_of_v, v.y / length_of_v, v.z / length_of_v);
  }
  template<class T> base_vector_3<T> sign(base_vector_3<T> v) {
    const T m1 = static_cast<T>(-1);
    const T p1 = static_cast<T>(+1);
    const T z  = static_cast<T>(0);

    return base_vector_3<T>(
      v.x < z ? m1 : (v.x > 0 ? p1 : z),
      v.y < z ? m1 : (v.y > 0 ? p1 : z),
      v.z < z ? m1 : (v.z > 0 ? p1 : z)
    ); 
  }
  template<class T> base_vector_2<T> sign(base_vector_2<T> v) {
    const T m1 = static_cast<T>(-1);
    const T p1 = static_cast<T>(+1);
    const T z  = static_cast<T>(0);

    return base_vector_2<T>(
      v.x < z ? m1 : (v.x > 0 ? p1 : z),
      v.y < z ? m1 : (v.y > 0 ? p1 : z)
    ); 
  }
  template<class T> T length(base_vector_2<T> v) { return std::sqrt(v.x*v.x + v.y*v.y); }
  template<class T> T length(base_vector_3<T> v) { return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z); }
  template<class T, class F> base_vector_2<T> vector_cast(base_vector_2<F> v) { return base_vector_2<T>(static_cast<T>(v.x), static_cast<T>(v.y)); }
  template<class T, class F> base_vector_3<T> vector_cast(base_vector_3<F> v) { return base_vector_3<T>(static_cast<T>(v.x), static_cast<T>(v.y), static_cast<T>(v.z)); }
  template<class T> base_vector_2<T> abs(base_vector_2<T> v) { return base_vector_2<T>(std::abs(v.x), std::abs(v.y)); }
  template<class T> base_vector_3<T> abs(base_vector_3<T> v) { return base_vector_3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z)); }
  template<class T> base_vector_3<T> step(base_vector_3<T> edge, base_vector_3<T> v) {
    return base_vector_3<T>(
      v.x < edge.x ? 0.0f : 1.0f,
      v.y < edge.y ? 0.0f : 1.0f,
      v.z < edge.z ? 0.0f : 1.0f
    );
  }
  template<class T> base_vector_4<T> step(base_vector_4<T> edge, base_vector_4<T> v) {
    return base_vector_4<T>(
      v.x < edge.x ? 0.0f : 1.0f,
      v.y < edge.y ? 0.0f : 1.0f,
      v.z < edge.z ? 0.0f : 1.0f,
      v.w < edge.w ? 0.0f : 1.0f
    );
  }
  inline float step(float edge, float v) { return v < edge ? 0.0f : 1.0f; }
  template<class T> base_vector_3<T> fract(base_vector_3<T> v) {
    return base_vector_3<T>(
      v.x - std::floor(v.x),
      v.y - std::floor(v.y),
      v.z - std::floor(v.z)
    );
  }

  template<class T> base_vector_2<T> fract(base_vector_2<T> v) {
    return base_vector_2<T>(
      v.x - std::floor(v.x),
      v.y - std::floor(v.y)
    );
  }

  template<class T> T fract(T v) {
    return v- std::floor(v);
  }
  
  template<class T> T max3(base_vector_3<T> v) {
    return std::max(std::max (v.x, v.y), v.z);
  }
  template<class T> T max3(T x, T y, T z) {
    return std::max(std::max (x, y), z);
  }
  template<class T> base_vector_3<T> reflect(base_vector_3<T> I, base_vector_3<T> N) {
    return I-2.0f * dot(N, I) * N;
  }
}

#endif