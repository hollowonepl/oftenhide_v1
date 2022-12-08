

#ifndef USHINE_CORE_TEXTURE_PNG
#define USHINE_CORE_TEXTURE_PNG


namespace ushine
{
	namespace core
	{

		class texture_loader_png : public texture_loader
		{
		public:
			texture_loader_png();
			virtual ~texture_loader_png();

			bool load(texture& tex, std::string& path);
		private:

			class pimpl;
			std::unique_ptr<pimpl> impl;
		};

	};
};


#endif