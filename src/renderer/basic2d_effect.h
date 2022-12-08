

#ifndef USHINE_RENDERER_EFFECT
#define USHINE_RENDERER_EFFECT


namespace ushine
{
	namespace renderer
	{

		class basic2d_effect {
		public:
			basic2d_effect();
			virtual ~basic2d_effect();

			virtual bool init(ushine::core::application* app, int textures, int w, int h);
			virtual bool release();

			virtual ushine::core::texture& get_texture(int index);

			virtual bool render_frame(float timestamp) = 0;

			virtual bool present(int index);

			class pimpl;
			std::unique_ptr<pimpl> impl;			
		};


		class basic2d_effect_tunnel : public basic2d_effect
		{
		public:
			basic2d_effect_tunnel();
			virtual ~basic2d_effect_tunnel();
			
			virtual bool init(ushine::core::application* app);
			virtual bool set_texture(core::texture* tex);
			virtual bool release();

			virtual bool render_frame(float timestamp);

		private:
			class pimpl;
			std::unique_ptr<pimpl> impl;
		};


	};

};


#endif