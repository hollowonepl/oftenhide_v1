#include <iostream>
#include <fstream>

#include "lwob_file.h"

namespace ushine
{

	namespace io
	{

		class lwob_file::pimpl
		{
		public:
			std::fstream file_handle;

		};



		lwob_file::lwob_file() : impl(new pimpl)
		{

		}
		lwob_file::~lwob_file()
		{

		}

		int lwob_file::open(const char* filename)
		{
				impl->file_handle.open(filename, std::ios::in | std::ios::out);

				if (impl->file_handle.good() == true)
					return 1;

				return 0;
		}
		int lwob_file::close()
		{
				impl->file_handle.close();
				return 1;
		}

		int lwob_file::read_all()
		{
				return 1;

		}

		int lwob_file::show_stats()
		{
				return 1;
		}

  }
}
