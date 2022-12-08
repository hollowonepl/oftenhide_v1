
#ifndef USHINE_RENDERER_COLOR
#define USHINE_RENDERER_COLOR

#include "../math/math.h"
#include "../core/common.h"

namespace ushine::renderer {
    struct DYNLIB_EXPORT color_u {
      uint8_t a, r, g, b;

			color_u() : a(0), r(0), g(0), b(0) {}
      color_u(uint32_t packed) { from_argb(packed); }
      color_u(uint8_t _a, uint8_t _r, uint8_t _g, uint8_t _b) : a(_a), r(_r), g(_g), b(_b) {}

			void from_argb(uint32_t packed) {
				a = ((packed >> 24) & 0xFF);
				r = ((packed >> 16) & 0xFF);
				g = ((packed >> 8)  & 0xFF);
				b = ((packed >> 0)  & 0xFF);
			}

      uint32_t to_argb() const {
				return (a << 24) + (r << 16) + (g << 8) + b;
			};
    };

  	struct DYNLIB_EXPORT color_f {
			float a,r,g,b;

			color_f() : a(0), r(0), g(0), b(0) {}
			color_f(float a0, float r0, float g0, float b0) : a(a0), r(r0), g(g0), b(b0) {}
			color_f(uint32_t packed) { from_argb(packed); }

      void from_argb(uint32_t packed) {
        auto [_a, _r, _g, _b] = color_u(packed);
        a = _a / 255.0f;
        r = _r / 255.0f;
        g = _g / 255.0f;
        b = _b / 255.0f;
      }

			void swap(color_f& right)
			{
				color_f temp = right;

				right.r = this->r;
				right.g = this->g;
				right.b = this->b;
				right.a = this->a;

				this->r = temp.r;
				this->g = temp.g;
				this->b = temp.b;
				this->a = temp.a;
			}


      // FIXME: deprecated. left for backwards compatibility only. for now.
      uint32_t to_argb(bool manual) {
				if (manual) {
          return color_f(1.0f, r*a, g*a, b*a).to_argb();
				} else {
          return to_argb();
        }
      }
  
			uint32_t to_argb() {
        uint8_t _a = static_cast<uint8_t>(math::clamp(a, 0.0f, 1.0f) * 255);
        uint8_t _r = static_cast<uint8_t>(math::clamp(r, 0.0f, 1.0f) * 255);
        uint8_t _g = static_cast<uint8_t>(math::clamp(g, 0.0f, 1.0f) * 255);
        uint8_t _b = static_cast<uint8_t>(math::clamp(b, 0.0f, 1.0f) * 255);
        
        return color_u(_a, _r, _g, _b).to_argb();
			};

			void apply_blend_alpha(float alpha)
			{
				r*=alpha;
				g*=alpha;
				b*=alpha;
				a*=alpha;
			}

			color_f modulo(float moduloBy)
			{
				//x - y * floor(x/y).
				color_f out(
					r - moduloBy * floor(r/moduloBy),
					g - moduloBy * floor(g/moduloBy),
					b - moduloBy * floor(b/moduloBy),
					a - moduloBy * floor(a/moduloBy)
					);
				return out;
			}

			inline color_f interpolate_to( const color_f& dest,float alpha )
			{
				color_f out;

				out.r = (dest.r - this->r) * alpha;
				out.g = (dest.g - this->g) * alpha;
				out.b = (dest.b - this->b) * alpha;

				return out;
			}
		};

    inline float blend_color_channel(float a, float b, float blend) { return sqrt(((1.0f - blend) * (a * a)) + (blend * (b * b))); }
    inline float blend_alpha_channel(float a, float b, float blend) { return ((1.0f - blend) * a) + (blend * b); }
    inline color_f blend(const color_f& a, const color_f& b, float blend) {
      return color_f(
        blend_alpha_channel(a.a, b.a, blend),
        blend_color_channel(a.r, b.r, blend),
        blend_color_channel(a.g, b.g, blend),
        blend_color_channel(a.b, b.b, blend)
      );
    }

    struct DYNLIB_EXPORT color_i {
      int a,r,g,b;
			color_i() : a(0), r(0), g(0), b(0) {}
			color_i(int a0, int r0, int g0, int b0) : a(a0), r(r0), g(g0), b(b0) {}
			color_i(int argb)
			{
				from_argb(argb);
			}

			void from_argb(int argb)
			{
				a = (argb >> 24) & 255;
				r = (argb >> 16) & 255;
				g = (argb >> 8) & 255;
				b = argb & 255;
			}

			virtual int to_argb()
			{
				return (a << 24) + (r << 16) + (g << 8) + b;
			};
		};
}

#endif
